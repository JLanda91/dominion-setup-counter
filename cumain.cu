#include <boost/multiprecision/integer.hpp>

#include <cstdint>
#include <algorithm>
#include <barrier>
#include <chrono>
#include <thread>
#include <omp.h>

#include <card_data/kingdom/card_type.hpp>
#include <card_data/kingdom/membership_mask.hpp>

#define CUDA_CHECK(expr, msg, ...) if(const cudaError_t err = expr; err != cudaSuccess) { fmt::println(msg, ##__VA_ARGS__); exit(-1); }

static constexpr std::size_t kBlockSize = 256;
// static constexpr std::size_t kBatchSize = 128'000'000;
// static constexpr std::size_t kBlockCount = (kBatchSize + kBlockSize - 1) / kBlockSize;
// static constexpr std::size_t kNumStreams = 2;

template<class T>
class ManagedVector {
    std::size_t n_ = 0;
    T* data_ = nullptr;

public:
    explicit ManagedVector(std::size_t n) : n_(n) {
        CUDA_CHECK(cudaMallocManaged(&data_, n * sizeof(T)), "Failed to allocate {} unified memory elements of type {} on the device", n, typeid(T).name());
        std::fill_n(data_, n, T{});
        fmt::println("Allocated {} elements of type {}", n, typeid(T).name());
    }

    explicit ManagedVector(const std::vector<T>& v) : n_(v.size()) {
        CUDA_CHECK(cudaMallocManaged(&data_, n_ * sizeof(T)), "Failed to allocate {} unified memory elements of type {} on the device", n_, typeid(T).name());
        std::copy(v.begin(), v.end(), data_);
        fmt::println("Allocated {} elements of type {}", n_, typeid(T).name());
    }

    ManagedVector(std::initializer_list<T> init) : n_(init.size()) {
        CUDA_CHECK(cudaMallocManaged(&data_, n_ * sizeof(T)), "Failed to allocate {} unified memory elements of type {} on the device", n_, typeid(T).name());
        std::copy(init.begin(), init.end(), data_);
        fmt::println("Allocated {} elements of type {}", n_, typeid(T).name());
    }

    ~ManagedVector() {
        cudaFree(data_);
    }

    inline auto begin() -> T* { return data_; }
    inline auto begin() const -> const T* { return data_; }
    inline auto end() -> T* { return data_ + n_;  }
    inline auto end() const -> const T* { return data_ + n_;  }
    inline auto back() -> T& { return data_[n_ - 1]; }
    inline auto back() const -> const T& { return data_[n_ - 1]; }
    inline auto data() -> T* { return data_; }
    inline auto data() const -> const T* { return data_; }
    inline std::size_t size() const noexcept { return n_; }
    __host__ __device__ inline auto operator[](std::size_t i) -> T& { return data_[i]; }
    __host__ __device__ inline auto operator[](std::size_t i) const -> const T& { return data_[i]; }

    inline void device_prefetch(int device, cudaStream_t s) const { cudaMemPrefetchAsync(data_, n_ * sizeof(T), device, s); }
    inline void host_prefetch(cudaStream_t s) const { cudaMemPrefetchAsync(data_, n_ * sizeof(T), cudaCpuDeviceId, s);  }
};

__host__ __device__ uint64_t binomial(uint8_t n, uint8_t k){
    if(k > n){
        return 0;
    }
    if(k == n || k == 0) {
        return 1;
    }
    uint64_t result = 1;
    const int max_j = k > n/2 ? n-k: k;
    for(auto j = 0; j < max_j; ++j){
        result *= (n - j);
        result /= j+1;
    }
    return result;
}

using namespace card_data;

struct Nonzero{
    kingdom::MembershipMask mask;
    kingdom::CardType card_type;
    uint8_t amount;
};

using nnz_vec_t = std::vector<Nonzero>;
using search_table_t = std::vector<uint64_t>;

inline search_table_t make_search_table(const ManagedVector<Nonzero>& nnz) {
    auto result = search_table_t(nnz.size() * 11uz);
    result.back() = 1ul;
    for (int i = nnz.size() - 2; i >= 0; --i) {
        const auto max_amount = std::min(nnz[i+1].amount, static_cast<uint8_t>(10u));
        for (auto j = 0; j < 11; ++j) {
            auto v = 0ul;
            for (auto k = 0uz; k <= max_amount && j + k < 11uz; ++k) {
                v += result[11 * (i + 1) + j + k];
            }
            result[11 * i + j] = v;
        }
    }
    return result;
}

template<std::size_t NNZ_SIZE>
__device__ inline uint64_t binom_product(std::size_t n, const Nonzero* nnz, const uint64_t* search_table) {
    // uint8_t tuple[NNZ_SIZE];
    uint8_t s = 0;
    uint64_t result = 1ul;
    // for (std::size_t j = 0; j < NNZ_SIZE; ++j) {
    //     tuple[j] = 0ul;
    // }
    for (auto i = 0u; i < NNZ_SIZE - 1u; ++i) {
        const auto max_amount = nnz[i].amount;
        if (s == 10u) {
            break;
        }
        for (uint8_t j = 0; (j <= max_amount) && (s + j <= 10u); ++j) {
            const auto& v = search_table[11 * i + j + s];
            if (v > n) {
                result *= binomial(nnz[i].amount, j);
                s += j;
                break;
            }
            n -= v;
        }
    }
    // tuple[NNZ_SIZE - 1] = 10u - s;
    result *= binomial(nnz[NNZ_SIZE - 1].amount, static_cast<uint8_t>(10u - s));

    // uint64_t result = 1ul;
    // for (auto i = 0uz; i < NNZ_SIZE; ++i) {
    //     result *= binomial<uint64_t>(nnz[i].amount, tuple[i]);
    // }
    return result;
}

template<std::size_t NNZ_SIZE>
__global__ void binom_product_kernel(std::size_t batch_num, std::size_t n, const Nonzero* nnz, const uint64_t* search_table, uint64_t* result) {
    const std::size_t offset = blockIdx.x * blockDim.x + threadIdx.x;
    const std::size_t stride = blockDim.x * gridDim.x;

    for (std::size_t i = offset; i < n; i += stride) {
        // const auto j = i % NNZ_SIZE;
        // result[i] = static_cast<uint64_t>(nnz[j].amount);
        // result[i] = i;
        result[i] = binom_product<NNZ_SIZE>(batch_num * kBatchSize + i, nnz, search_table);
    }
}

static constexpr std::size_t kNumNonzeros = 53uz;

class Executor {
public:
    Executor(std::size_t num_iter, std::size_t batch_size, std::size_t num_streams)
    : num_iter_(num_iter)
    , batch_size_(batch_size)
    , num_streams_(num_streams)
    , streams_(num_streams)
    , output_buffers_(num_streams, ManagedVector<uint64_t>(batch_size))
    , result_(omp_get_max_threads())
    , num_batches_(num_iter / batch_size)
    , remainder_batch_size_(num_iter % batch_size)
    , grid_size_((batch_size + kBlockSize - 1) / kBlockSize)
    , remainder_grid_size_((remainder_batch_size_ + kBlockSize - 1) / kBlockSize) {
        fmt::println("Executor constructed:");
        int device_count = 0;
        CUDA_CHECK(cudaGetDeviceCount(&device_count), "\tcudaDeviceCount failed");
        fmt::println("\tCUDA Devices: {}", device_count);
        CUDA_CHECK(cudaGetDevice(&dev_), "\tcudaGetDevice failed");
        CUDA_CHECK(cudaSetDevice(dev_), "\tcudaSetDevice failed");
        fmt::println("\tCurrent CUDA device: {}", dev_);
        for (auto& stream : streams_) {
            CUDA_CHECK(cudaStreamCreate(&stream), "\tcudaStreamCreate failed: {}", cudaGetErrorString(err));
        }
        fmt::println("\tNumber of iterations: {} (max {})", num_iter_, max_iter);
        fmt::println("\tBatch size: {} ({} blocks of {} threads)", batch_size_, grid_size_, kBlockSize);
        fmt::println("\tRemainder batch size: {} ({} blocks of {} threads)", remainder_batch_size_, remainder_grid_size_, kBlockSize);
        fmt::println("\tNumber of streams: {} (constructed a vector of {} streams)", num_streams_, streams_.size());
        fmt::println("\tNumber of output buffers: {} (constructed a vector of {} buffers each size {})", num_streams_, output_buffers_.size(), output_buffers_.back().size());
        fmt::println("\tConstructed reduction array of size {}", result_.size());
        fmt::println("\tNumber of batches: {}", num_batches_);

    }

    Executor(const Executor&) = delete;
    Executor& operator=(const Executor&) = delete;
    Executor(Executor&&) = delete;
    Executor& operator=(Executor&&) = delete;

    ~Executor() {
        CUDA_CHECK(cudaDeviceSynchronize(), "cudaDeviceSynchronize failed: {}", cudaGetErrorString(err));
        for (auto& stream : streams_) {
            CUDA_CHECK(cudaStreamDestroy(stream), "cudaStreamDestroy failed: {}", cudaGetErrorString(err));
        }
    }

    void schedule_kernel() {
        if (schedule_batch_i_ < num_batches_) {
            binom_product_kernel<kNumNonzeros><<<grid_size_, kBlockSize, dev_, streams_[schedule_stream_i]>>>(schedule_batch_i_, batch_size_, nnz_.data(), search_table_.data(), output_buffers_[schedule_stream_i].data());
        } else if (remainder_batch_size_ > 0){
            binom_product_kernel<kNumNonzeros><<<remainder_grid_size_, kBlockSize, dev_, streams_[schedule_stream_i]>>>(schedule_batch_i_, remainder_batch_size_, nnz_.data(), search_table_.data(), output_buffers_[schedule_stream_i].data());
        }
        increase_schedule_index();
    }

    void reduce_result() {
        fmt::println("Reducing batch {} on stream {}...", reduce_batch_i_, reduce_stream_i_);
        CUDA_CHECK(cudaStreamSynchronize(streams_[reduce_stream_i_]), "cudaStreamSynchronize failed on stream {}: {}\n", reduce_stream_i_, cudaGetErrorString(err));

        if (reduce_batch_i_ < num_batches_) {
            #pragma omp parallel for default(none) shared(result_, output_buffers_, batch_size_) schedule(dynamic,128'000)
            for (auto j = 0u; j < batch_size_; ++j) {
                result_[omp_get_thread_num()] += output_buffers_[reduce_stream_i_][j];
            }
        } else if (remainder_batch_size_ > 0) {
            #pragma omp parallel for default(none) shared(result_, output_buffers_, remainder_batch_size_) schedule(dynamic,remainder_batch_size_/1000)
            for (auto j = 0u; j < remainder_batch_size_; ++j) {
                result_[omp_get_thread_num()] += output_buffers_[reduce_stream_i_][j];
            }
        }

        increase_reduce_index();
    }

    [[nodiscard]] auto get_result() const {
        result_t ans{};
        for (const auto& result_per_thread : result_) {
            ans += result_per_thread;
        }
        return ans;
    }

private:
    ManagedVector<Nonzero> nnz_ {
        {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::YoungWitch, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Knights, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Druid, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Ferryman, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Riverboat, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Looter, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::Looter, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Looter, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1000010), kingdom::CardType::Fate, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Fate, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Fate, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Fate, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Fate, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b0000010), kingdom::CardType::Doom, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::Doom, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Doom, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Doom, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Doom, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1000000), kingdom::CardType::Liaison, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1000001), kingdom::CardType::Liaison, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Liaison, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Liaison, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Liaison, 3u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Liaison, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1100000), kingdom::CardType::Omen, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Omen, 3u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Omen, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Omen, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1000000), kingdom::CardType::Loot, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1000001), kingdom::CardType::Loot, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1100010), kingdom::CardType::Loot, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Loot, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Loot, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b0000000), kingdom::CardType::None, 8u},
        {kingdom::MembershipMask::FromUnsigned(0b0000001), kingdom::CardType::None, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b0000010), kingdom::CardType::None, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b0001000), kingdom::CardType::None, 6u},
        {kingdom::MembershipMask::FromUnsigned(0b0001001), kingdom::CardType::None, 5u},
        {kingdom::MembershipMask::FromUnsigned(0b1000000), kingdom::CardType::None, 26u},
        {kingdom::MembershipMask::FromUnsigned(0b1000001), kingdom::CardType::None, 5u},
        {kingdom::MembershipMask::FromUnsigned(0b1000010), kingdom::CardType::None, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1001000), kingdom::CardType::None, 9u},
        {kingdom::MembershipMask::FromUnsigned(0b1001001), kingdom::CardType::None, 5u},
        {kingdom::MembershipMask::FromUnsigned(0b1100000), kingdom::CardType::None, 43u},
        {kingdom::MembershipMask::FromUnsigned(0b1100010), kingdom::CardType::None, 12u},
        {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::None, 37u},
        {kingdom::MembershipMask::FromUnsigned(0b1100111), kingdom::CardType::None, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::None, 93u},
        {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::None, 13u},
        {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::None, 65u},
        {kingdom::MembershipMask::FromUnsigned(0b1101111), kingdom::CardType::None, 8u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::None, 88u},
        {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::None, 26u}
    };
    ManagedVector<uint64_t> search_table_{make_search_table(nnz_)};
    std::size_t schedule_batch_i_ = 0;
    std::size_t schedule_stream_i = 0;
    std::size_t reduce_batch_i_ = 0;
    std::size_t reduce_stream_i_ = 0;
    std::size_t max_iter = [this] {
        std::size_t ans = 0uz;
        for (auto i = 0uz; i <= nnz_[0].amount; ++i) {
            ans += search_table_[i];
        }
        return ans;
    }();
    int dev_ = 0;

    std::size_t num_iter_;
    std::size_t batch_size_;
    std::size_t num_streams_;
    std::vector<cudaStream_t> streams_;
    std::vector<ManagedVector<uint64_t>> output_buffers_;
    std::vector<uint64_t> result_;
    std::size_t num_batches_;
    std::size_t remainder_batch_size_;
    std::size_t grid_size_;
    std::size_t remainder_grid_size_;

    void increase_schedule_index() {
        schedule_batch_i_++;
        schedule_stream_i++;
        if (schedule_stream_i == num_streams_) {
            schedule_stream_i = 0;
        }
    }

    void increase_reduce_index() {
        reduce_batch_i_++;
        reduce_stream_i_++;
        if (reduce_stream_i_ == num_streams_) {
            reduce_stream_i_ = 0;
        }
    }

};

auto main() -> int {
    constexpr auto num_streams = 2u;
    constexpr auto batch_size = 128'000'000uz;
    Executor executor(num_streams * batch_size, batch_size, num_streams);

    // int device_count = 0;
    // CUDA_CHECK(cudaGetDeviceCount(&device_count), "cudaDeviceCount failed");
    //
    // fmt::println("CUDA Devices: {}", device_count);
    // int dev;
    // CUDA_CHECK(cudaGetDevice(&dev), "cudaGetDevice failed");
    // CUDA_CHECK(cudaSetDevice(dev), "cudaSetDevice failed");
    // fmt::println("Current CUDA device: {}", dev);
    //
    // std::vector<cudaStream_t> streams(kNumStreams, {});
    // for (auto& stream : streams) {
    //     CUDA_CHECK(cudaStreamCreate(&stream), "cudaStreamCreate failed: {}", cudaGetErrorString(err));
    // }
    //
    // ManagedVector<Nonzero> nnz {
    //     {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::YoungWitch, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Knights, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Druid, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Ferryman, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Riverboat, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Looter, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::Looter, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Looter, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1000010), kingdom::CardType::Fate, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Fate, 2u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Fate, 2u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Fate, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Fate, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b0000010), kingdom::CardType::Doom, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::Doom, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Doom, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Doom, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Doom, 2u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1000000), kingdom::CardType::Liaison, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1000001), kingdom::CardType::Liaison, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Liaison, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Liaison, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Liaison, 3u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Liaison, 2u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1100000), kingdom::CardType::Omen, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Omen, 3u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Omen, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Omen, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1000000), kingdom::CardType::Loot, 2u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1000001), kingdom::CardType::Loot, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1100010), kingdom::CardType::Loot, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Loot, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Loot, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b0000000), kingdom::CardType::None, 8u},
    //     {kingdom::MembershipMask::FromUnsigned(0b0000001), kingdom::CardType::None, 2u},
    //     {kingdom::MembershipMask::FromUnsigned(0b0000010), kingdom::CardType::None, 1u},
    //     {kingdom::MembershipMask::FromUnsigned(0b0001000), kingdom::CardType::None, 6u},
    //     {kingdom::MembershipMask::FromUnsigned(0b0001001), kingdom::CardType::None, 5u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1000000), kingdom::CardType::None, 26u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1000001), kingdom::CardType::None, 5u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1000010), kingdom::CardType::None, 2u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1001000), kingdom::CardType::None, 9u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1001001), kingdom::CardType::None, 5u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1100000), kingdom::CardType::None, 43u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1100010), kingdom::CardType::None, 12u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::None, 37u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1100111), kingdom::CardType::None, 2u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::None, 93u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::None, 13u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::None, 65u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1101111), kingdom::CardType::None, 8u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::None, 88u},
    //     {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::None, 26u}
    // };
    // ManagedVector<uint64_t> search_table{make_search_table(nnz)};
    // // const auto max_iter = search_table[0] + search_table[1];
    // const auto max_iter = 49'536'000'000uz;
    //
    // fmt::println("Max iter: {}", max_iter);
    //
    // // std::vector<ManagedVector<uint64_t>> output_buffers{};
    // // output_buffers.reserve(kNumStreams);
    // // for (auto i = 0u; i < kNumStreams; ++i) {
    // //     output_buffers.emplace_back(kBatchSize);
    // //     fmt::println("Output buffer {}: {}", i, output_buffers[i].size());
    // // }
    // ManagedVector<uint64_t> output_buffer0(kBatchSize);
    // ManagedVector<uint64_t> output_buffer1(kBatchSize);
    //
    // const auto max_batch_num = max_iter / kBatchSize;
    // const auto remaining_size = max_iter % kBatchSize;
    //
    // std::vector<result_t> results;
    // results.resize(omp_get_max_threads());
    //
    // const auto t1 = std::chrono::steady_clock::now();
    // auto batch_number = 0u;
    // fmt::println("Starting batch {} on stream {}", batch_number, 0);
    // binom_product_kernel<kNumNonzeros><<<kBlockCount, kBlockSize, dev, streams[0]>>>(batch_number++, kBatchSize, managed_nnz.data(), search_table.data(), output_buffer0.data());
    //
    // fmt::println("Starting batch {} on stream {}", batch_number, 1);
    // binom_product_kernel<kNumNonzeros><<<kBlockCount, kBlockSize, dev, streams[1]>>>(batch_number++, kBatchSize, managed_nnz.data(), search_table.data(), output_buffer1.data());
    //
    // fmt::println("Syncing and reducing batch {} on stream {}", batch_number - kNumStreams, 0);
    // CUDA_CHECK(cudaStreamSynchronize(streams[0]), "cudaStreamSynchronize failed on stream {}: {}", 0, cudaGetErrorString(err));
    // #pragma omp parallel for default(none) shared(results, kBatchSize, output_buffer0) schedule(dynamic,128'000)
    // for (auto j = 0u; j < kBatchSize; ++j) {
    //     results[omp_get_thread_num()] += output_buffer0[j];
    // }
    // fmt::println("Starting batch {} on stream {}", batch_number, 0);
    // binom_product_kernel<kNumNonzeros><<<kBlockCount, kBlockSize, dev, streams[0]>>>(batch_number++, kBatchSize, managed_nnz.data(), search_table.data(), output_buffer0.data());
    //
    // fmt::println("Syncing and batch {} on stream {}", batch_number - kNumStreams, 1);
    // CUDA_CHECK(cudaStreamSynchronize(streams[1]), "cudaStreamSynchronize failed on stream {}: {}", 1, cudaGetErrorString(err));
    // #pragma omp parallel for default(none) shared(results, kBatchSize, output_buffer1) schedule(dynamic,128'000)
    // for (auto j = 0u; j < kBatchSize; ++j) {
    //     results[omp_get_thread_num()] += output_buffer1[j];
    // }
    // fmt::println("Starting batch {} on stream {}", batch_number, 1);
    // binom_product_kernel<kNumNonzeros><<<kBlockCount, kBlockSize, dev, streams[1]>>>(batch_number++, kBatchSize, managed_nnz.data(), search_table.data(), output_buffer1.data());
    //
    // fmt::println("Syncing and reducing batch {} on stream {}", batch_number - kNumStreams, 0);
    // CUDA_CHECK(cudaStreamSynchronize(streams[0]), "cudaStreamSynchronize failed on stream {}: {}", 0, cudaGetErrorString(err));
    // #pragma omp parallel for default(none) shared(results, kBatchSize, output_buffer0) schedule(dynamic,128'000)
    // for (auto j = 0u; j < kBatchSize; ++j) {
    //     results[omp_get_thread_num()] += output_buffer0[j];
    // }

    // fmt::println("Syncing and batch {} on stream {}", batch_number - kNumStreams, 1);
    // CUDA_CHECK(cudaStreamSynchronize(streams[1]), "cudaStreamSynchronize failed on stream {}: {}", 1, cudaGetErrorString(err));
    // #pragma omp parallel for default(none) shared(results, kBatchSize, output_buffers) schedule(dynamic,128'000)
    // for (auto j = 0u; j < kBatchSize; ++j) {
    //     results[omp_get_thread_num()] += output_buffers[1][j];
    // }


    // auto batch_number = 0u;
    // for (auto i = 0uz; i < kNumStreams; ++i) {
    //     fmt::println("Starting batch {} on stream {}", i, batch_number);
    //     binom_product_kernel<kNumNonzeros><<<kBlockCount, kBlockSize, dev, streams[i]>>>(batch_number, kBatchSize, managed_nnz.data(), search_table.data(), output_buffers[i].data());
    //     batch_number++;
    // }
    // while (batch_number < max_batch_num - kNumStreams) {
    //     for (auto i = 0uz; i < kNumStreams; ++i) {
    //         fmt::println("Syncing batch {} on stream {}", i, batch_number - kNumStreams);
    //         CUDA_CHECK(cudaStreamSynchronize(streams[i]), "cudaStreamSynchronize failed on stream {}: {}\n", i, cudaGetErrorString(err));
    //         fmt::println("Reducing batch {}", batch_number - kNumStreams);
    //         #pragma omp parallel for default(none) shared(i, results, kBatchSize, output_buffers) schedule(dynamic,128'000)
    //         for (auto j = 0u; j < kBatchSize; ++j) {
    //             results[omp_get_thread_num()] += output_buffers[i][j];
    //         }
    //         fmt::println("Starting batch {} on stream {}", i, batch_number);
    //         binom_product_kernel<kNumNonzeros><<<kBlockCount, kBlockSize, dev, streams[i]>>>(batch_number, kBatchSize, managed_nnz.data(), search_table.data(), output_buffers[i].data());
    //         batch_number++;
    //     }
    // }
    // const auto num_full_batches_left = max_batch_num - batch_number;
    // for (auto i = 0uz; i < kNumStreams; ++i) {
    //
    //     fmt::println("Syncing batch {} on stream {}", i, batch_number - kNumStreams);
    //     CUDA_CHECK(cudaStreamSynchronize(streams[i]), "cudaStreamSynchronize failed on stream {}: {}\n", i, cudaGetErrorString(err));
    //     const auto& output_buffer = output_buffers[i];
    //     // fmt::println("Reducing batch {}", batch_number - kNumStreams);
    //     #pragma omp parallel for default(none) shared(results, kBatchSize, output_buffer) schedule(dynamic,128'000)
    //     for (auto j = 0u; j < kBatchSize; ++j) {
    //         results[omp_get_thread_num()] += output_buffer[j];
    //     }
    //
    //     if (i < num_full_batches_left) {
    //         // fmt::println("Starting batch {} on stream {}", i, batch_number);
    //         binom_product_kernel<kNumNonzeros><<<kBlockCount, kBlockSize, dev, streams[i]>>>(batch_number, kBatchSize, managed_nnz.data(), search_table.data(), output_buffers[i].data());
    //         batch_number++;
    //     }
    //
    //     if (i == num_full_batches_left && remaining_size > 0) {
    //         // fmt::println("Starting last stream {} on batch {}", i, batch_number);
    //         binom_product_kernel<kNumNonzeros><<<kBlockCount, kBlockSize, dev, streams[i]>>>(batch_number, remaining_size, managed_nnz.data(), search_table.data(), output_buffers[i].data());
    //         batch_number++;
    //     }
    // }
    // for (auto i = 0uz; i < num_full_batches_left; ++i) {
    //     CUDA_CHECK(cudaStreamSynchronize(streams[i]), "cudaStreamSynchronize failed on stream {}: {}\n", i, cudaGetErrorString(err));
    //     const auto& output_buffer = output_buffers[i];
    //     #pragma omp parallel for default(none) shared(results, kBatchSize, output_buffer) schedule(dynamic,128'000)
    //     for (auto j = 0u; j < kBatchSize; ++j) {
    //         results[omp_get_thread_num()] += output_buffer[j];
    //     }
    // }
    // if (remaining_size > 0) {
    //     CUDA_CHECK(cudaStreamSynchronize(streams[num_full_batches_left]), "cudaStreamSynchronize failed on stream {}: {}\n", num_full_batches_left, cudaGetErrorString(err));
    //
    //     const auto& output_buffer = output_buffers[num_full_batches_left];
    //     #pragma omp parallel for default(none) shared(results, remaining_size, output_buffer) schedule(dynamic,128'000)
    //     for (auto j = 0u; j < remaining_size; ++j) {
    //         results[omp_get_thread_num()] += output_buffer[j];
    //     }
    // }

    // auto& output_buffer = output_buffers[0];
    // for (auto batch_number = 0u; batch_number < max_batch_num; ++batch_number) {
    //     binom_product_kernel<kNumNonzeros><<<kBlockCount, kBlockSize>>>(batch_number, kBatchSize, managed_nnz.data(), search_table.data(), output_buffer.data());
    //     // CUDA_CHECK(cudaStreamSynchronize(stream), "cudaStreamSynchronize failed: {}\n", cudaGetErrorString(err));
    //     cudaDeviceSynchronize();
    //
    //     #pragma omp parallel for default(none) shared(results, kBatchSize, output_buffer) schedule(dynamic,128'000)
    //     for (auto i = 0u; i < kBatchSize; ++i) {
    //         results[omp_get_thread_num()] += output_buffer[i];
    //     }
    // }
    // if (remaining_size > 0) {
    //     binom_product_kernel<kNumNonzeros><<<kBlockCount, kBlockSize>>>(max_batch_num, remaining_size, managed_nnz.data(), search_table.data(), output_buffer.data());
    //     cudaDeviceSynchronize();
    //     const auto last_error = cudaGetLastError();
    //     if (last_error != cudaSuccess) {
    //         std::cout << "cuda error " << last_error << " at remainder batch: " << cudaGetErrorString(last_error) << '\n';
    //     }
    //
    //     #pragma omp parallel for default(none) shared(results, remaining_size, output_buffer)
    //     for (auto i = 0u; i < remaining_size; ++i) {
    //         results[omp_get_thread_num()] += output_buffer[i];
    //     }
    // }

    // result_t result{0};
    // for (const auto& x : results) {
    //     result += x;
    // }
    //
    // const auto t2 = std::chrono::steady_clock::now();
    //
    // std::cout << "GPU Result = " << result << '\n';
    // std::cout << "Time (ms) = " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << '\n';
    //
    // for (auto& stream : streams) {
    //     CUDA_CHECK(cudaStreamDestroy(stream), "cudaStreamDestroy failed: {}", cudaGetErrorString(err));
    // }
}