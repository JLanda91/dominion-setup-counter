#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <fmt/format.h>
#include <fmt/chrono.h>

#include <sparkplug/util/cuda/check_cuda_fn_error.cuh>
#include <sparkplug/util/cuda/stream.cuh>

#include "data_definitions/big_uint.cuh"
#include "data_definitions/kingdom.hpp"
#include "data_definitions/reduce_type_and_is_action_or_treasure.cuh"

#include "data/equivalence_classes.hpp"
#include "data/mask_num_at_reduction.hpp"
#include "data/type_at_reduction_pairs.cuh"
#include "data_definitions/reduce_mask_and_num_action_or_treasure.hpp"


namespace data {
    __constant__ kingdom_equivalence_classes::device_type cECData;
    __constant__ kingdom_equivalence_class_segments::device_type cECSegments;
    __constant__ reduce_type_and_is_action_or_treasure::device_type cIndexPairs;
}

__host__ __device__ constexpr auto round_up_to_multiple_of(auto input, auto m) {
    return ((input + m - 1) / m)*m;
}

static constexpr size_t NUM_SM = 38;
static constexpr uint32_t NUM_BLOCKS_PER_SM = 11;
static constexpr uint32_t BLOCK_SIZE = 128;
static constexpr size_t GRID_SIZE = NUM_SM * NUM_BLOCKS_PER_SM;
// static constexpr size_t GRID_SIZE = 1;


void initialize_cmem(sparkplug::util::cuda::Stream& stream) {
    sparkplug::util::cuda::check_cuda_fn_error(
    "Copy of EC_DATA to cmem failed",
        cudaMemcpyToSymbolAsync<decltype(data::cECData)>,
        data::cECData,
        data::kECData.data(),
        sizeof(data::cECData),
        0,
        cudaMemcpyHostToDevice,
        stream
    );

    sparkplug::util::cuda::check_cuda_fn_error(
    "Copy of EC_OFFSETS to cmem failed",
        cudaMemcpyToSymbolAsync<decltype(data::cECSegments)>,
        data::cECSegments,
        data::kECSegments.data(),
        sizeof(data::cECSegments),
        0,
        cudaMemcpyHostToDevice,
        stream
    );

    sparkplug::util::cuda::check_cuda_fn_error(
    "Copy of REDUCTION_QUADS to cmem failed",
        cudaMemcpyToSymbolAsync<decltype(data::cIndexPairs)>,
        data::cIndexPairs,
        data::kIndexPairs.data(),
        sizeof(data::cIndexPairs),
        0,
        cudaMemcpyHostToDevice,
        stream
    );
}


struct SharedMemStructure {
    data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult reduce_type_and_is_action_or_treasure_result;
    union {
        struct {
            data_definitions::reduce_type_and_is_action_or_treasure::ReductionRow reduction_row1;
            data_definitions::reduce_type_and_is_action_or_treasure::ReductionRow reduction_row2;
            uint16_t work_item_weights[data::kNumEquivalenceClasses];
        } reduce_type_and_is_action_or_treasure{};

        // other reduction phase
    };
};

// static constexpr unsigned NUM_LOOPS_IN_KERNEL = 8250;
static constexpr unsigned NUM_LOOPS_IN_KERNEL = 1;

__global__ void aggregate_by_type_mask_and_num_action_or_treasure() {
    // shared mem
    __shared__ SharedMemStructure sReduce;

    // set work items (prototype: all 1 for complete convolution)
    for (auto i = threadIdx.x; i < data::kNumEquivalenceClasses; i += blockDim.x) {
        sReduce.reduce_type_and_is_action_or_treasure.work_item_weights[i] = (1 << 11) - 1;
    }
    __syncthreads();

    // swappable ptrs
    auto* prev_reduction_result = &sReduce.reduce_type_and_is_action_or_treasure.reduction_row1;
    auto* next_reduction_result = &sReduce.reduce_type_and_is_action_or_treasure.reduction_row2;

    // for (auto xx = 0; xx < NUM_LOOPS_IN_KERNEL; ++xx)
    // for all ec segments (type, is_at, begin offset, end offset)
    for (auto ec_segment_i = 0; ec_segment_i < data::kNumECSegments; ec_segment_i ++) {
        // set prev reduction result to 1,0,0,0,0,0,...
        for (auto r = threadIdx.x; r < 11; r += blockDim.x) {
            prev_reduction_result->data[r] = data_definitions::u128_t{0ul};
        }
        if (threadIdx.x == 0) {
            prev_reduction_result->data[0] = data_definitions::u128_t{1ul};
        }
        __syncthreads();

        for (auto ec_i = data::cECSegments[ec_segment_i].begin; ec_i < data::cECSegments[ec_segment_i].end; ++ec_i) {
            // set next reduction result to 0
            for (auto m = threadIdx.x; m < 11; m += blockDim.x) {
                next_reduction_result->data[m] = data_definitions::u128_t{0ul};
            }
            __syncthreads();

            for (auto pair_i = threadIdx.x; pair_i < round_up_to_multiple_of(data::kNumIndexPairs, BLOCK_SIZE); pair_i += blockDim.x) {
                int n = 10; auto contrib = data_definitions::u128_t{0};
                if (pair_i < data::kNumIndexPairs) {
                    int n_ec = 10;
                    data::cIndexPairs[pair_i].unpack(n, n_ec);
                    const unsigned w = (sReduce.reduce_type_and_is_action_or_treasure.work_item_weights[ec_i] >> n_ec) & 1;
                    contrib = prev_reduction_result->data[n - n_ec] * data::cECData[ec_i].binom_data[n_ec] * w;
                }

                const int continuation_no = data::cIndexPairs[pair_i].continuation_no();
                int boundary_reached = 0;

                for (int lane_offset = 1; lane_offset < 32; lane_offset <<= 1) {
                    const auto add_lo = __shfl_down_sync(0xFFFFFFFF, contrib.limbs[0], lane_offset);
                    const auto add_hi = __shfl_down_sync(0xFFFFFFFF, contrib.limbs[1], lane_offset);
                    const auto shfl_cont_no = __shfl_down_sync(0XFFFFFFFF, continuation_no, lane_offset);
                    const auto shfl_boundary_reached = __shfl_down_sync(0XFFFFFFFF, boundary_reached, lane_offset);

                    // if boundary not yet reached
                    if (!boundary_reached) {
                        // if out of bounds or shuffled continuation is 1 (new (n,k) segment) then stop
                        if ((threadIdx.x & 0x1F) + lane_offset >= 32 || shfl_cont_no == 1) {
                            boundary_reached = true;
                            continue;
                        }

                        // do addition
                        contrib += add_lo;
                        contrib.limbs[1] += add_hi;

                        // if offset lane has stopped processing, stop processing now
                        if (shfl_boundary_reached) {
                            boundary_reached = true;
                        }
                    }
                }

                for (auto c = 1; c <= data::kMaxIndexPairContinuation; c++) {
                    if (c == continuation_no) {
                        next_reduction_result->data[n] += contrib;
                    }
                    __syncthreads();
                }
            }

            // after reduction of one EC, swap double buffer ptrs
            {
                auto* tmp = prev_reduction_result;
                prev_reduction_result = next_reduction_result;
                next_reduction_result = tmp;
                __syncthreads();
            }
        }

        // after all is reduced for an EC segment, write reduced row to shared mem
        for (auto m = threadIdx.x; m < 11; m += blockDim.x) {
            sReduce.reduce_type_and_is_action_or_treasure_result.data[ec_segment_i][m] = prev_reduction_result->data[m];
        }
        __syncthreads();
    }

    // // TEMP: copy to gmem
    // for (auto out_i = threadIdx.x; out_i < 11*11; out_i += blockDim.x) {
    //     (&intra_type_reduction_ec53->data[0][0])[out_i] = (&reduce_ec53.reduction_result.data[0][0])[out_i];
    // }
}

void ec53() {
    // Create an auto stream initialize cmem with EC data, EC offsets, and reduction items


    cudaEvent_t init_begin{}, init_end{}, kernel_begin{}, kernel_end{};
    sparkplug::util::cuda::Stream stream{};

    cudaEventCreate(&init_begin);
    cudaEventCreate(&init_end);
    cudaEventCreate(&kernel_begin);
    cudaEventCreate(&kernel_end);

    sparkplug::util::cuda::check_cuda_fn_error("Recording event init_begin", cudaEventRecord, init_begin, stream);
    initialize_cmem(stream);
    sparkplug::util::cuda::check_cuda_fn_error("Recording event init_end", cudaEventRecord, init_end, stream);


    // auto h_intra_result = thrust::host_vector<data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult>(GRID_SIZE, {});
    // thrust::device_vector<data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult> d_intra_result = h_intra_result;

    thrust::device_vector<data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution> ec_segment_distributions(
        std::begin(data::kECSegmentDistributions),
        std::end(data::kECSegmentDistributions));
    sparkplug::util::cuda::check_cuda_fn_error("Recording event kernel_begin", cudaEventRecord, kernel_begin, stream);
    aggregate_by_type_mask_and_num_action_or_treasure<<<GRID_SIZE, BLOCK_SIZE, 0, stream>>>();
    sparkplug::util::cuda::check_cuda_fn_error("Recording event kernel_end", cudaEventRecord, kernel_end, stream);

    sparkplug::util::cuda::check_cuda_fn_error("Event sync", cudaEventSynchronize, kernel_end);
    float cmem_init_time_ms{};
    sparkplug::util::cuda::check_cuda_fn_error("Determine cmem init time", cudaEventElapsedTime, &cmem_init_time_ms, init_begin, init_end);
    float kernel_time_ms{};
    sparkplug::util::cuda::check_cuda_fn_error("Determine kernel time", cudaEventElapsedTime, &kernel_time_ms, kernel_begin, kernel_end);

    fmt::println("Initialized constant memory: {} ms", cmem_init_time_ms, GRID_SIZE);
    fmt::println("Kernel block size: {}", BLOCK_SIZE);
    fmt::println("Blocks per SM: {}", NUM_BLOCKS_PER_SM);
    fmt::println("Kernel time: {} ms for {} work items ({} ms per grid)", kernel_time_ms , GRID_SIZE * NUM_LOOPS_IN_KERNEL, kernel_time_ms / NUM_LOOPS_IN_KERNEL);

    // h_intra_result = d_intra_result;
    // // h_type_subset_result = d_type_subset_result;
    // for (auto & ec_segment_i : h_intra_result[0].data) {
    //     for (auto & n : ec_segment_i) {
    //         fmt::print("{}, ", n);
    //     }
    //     fmt::print("\n");
    // }
}


int main(){
    ec53();
}