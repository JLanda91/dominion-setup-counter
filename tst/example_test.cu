#include <thrust/device_vector.h>
#include <thrust/host_vector.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstdio>

#include "../cuda_utils/auto_cuda_stream.cuh"
#include "../cuda_utils/big_uint.cuh"
#include "../cuda_utils/scalar.cuh"

template<typename Backend>
class FunctorUnderTest {
public:
    using backend_type = Backend;

    explicit FunctorUnderTest(const Backend* backend): backend_(backend) {}

    __device__ int operator()(int i) const {
        return 2 * backend_->operator()(i / 2);
    }

private:
    const Backend* backend_;
};

template<typename InputType, typename OutputType>
class DeviceMockHelper {
public:
    struct DeviceBackend {
        const InputType* d_expected_inputs = nullptr;
        const OutputType* d_mocked_outputs = nullptr;
        std::size_t num_inputs = 0uz;

        __device__ OutputType operator()(const InputType& input) const {
            static std::size_t i = 0;

            if (i >= num_inputs) {
                printf("Device mock called too many times");
                assert(false);
            }
            if (d_expected_inputs[i] != input) {
                printf("Input mismatch at index %ld: %ld vs %ld", i, input, d_expected_inputs[i]);
                assert(false);
            }
            return d_mocked_outputs[i++];
        }
    };

    DeviceMockHelper(const thrust::host_vector<InputType>& inputs,
                     const thrust::host_vector<OutputType>& outputs,
                     cudaStream_t& stream)
    : expected_inputs(inputs)
    , mocked_outputs(outputs)
    , backend_({
        thrust::raw_pointer_cast(expected_inputs.data()),
        thrust::raw_pointer_cast(mocked_outputs.data()),
        inputs.size()})
    {
        assert(inputs.size() == outputs.size());
        backend_.update_device(stream);
    }

    [[nodiscard]] __host__ auto backend() const -> const Scalar<DeviceBackend>&{
        return backend_;
    }

private:
    thrust::device_vector<InputType> expected_inputs;
    thrust::device_vector<OutputType> mocked_outputs;
    Scalar<DeviceBackend> backend_;
};

using DeviceMockHelperIntInt = DeviceMockHelper<int, int>;
using FunctorUnderTestWithMock = FunctorUnderTest<DeviceMockHelperIntInt::DeviceBackend>;

__global__ void test_function(std::size_t n, const int* inputs, int* out, const FunctorUnderTestWithMock::backend_type* f) {
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        for (auto i = 0; i < n; ++i) {
            out[i] = f->operator()(inputs[i]);
        }
    }
}

TEST(main, main)
{
    AutoCudaStream acs{};
    std::cout << "Initializing DeviceMockHelper" << std::endl;
    DeviceMockHelperIntInt mock {
        {-25, -50, 46, -11, 8},
        {40, 17, 69, -45, 16},
        acs()
    };

    std::cout << "Initializing Impl" << std::endl;
    Scalar<FunctorUnderTestWithMock> impl{FunctorUnderTestWithMock{&mock.backend().value()}};

    const thrust::device_vector<int> inputs {-51, -100, 92, -22, 16};
    thrust::device_vector<int> device_outputs(inputs.size(), 0);
    test_function<<<1, 1>>>(inputs.size(), inputs.data().get(), device_outputs.data().get(), mock.backend().dev_ptr());
    cudaDeviceSynchronize();
    thrust::host_vector<int> host_outputs = device_outputs;
    ASSERT_THAT(host_outputs, testing::ElementsAre(24, 32, 40, 80, -16));
}