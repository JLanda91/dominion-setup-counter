#include <cstdio>

#include "data_definitions/big_uint.cuh"
#include "data_definitions/kingdom.hpp"
#include "data_definitions/reduce_type_and_is_action_or_treasure.cuh"

#include "data/equivalence_classes.hpp"
#include "data/mask_num_at_reduction.hpp"
#include "data/type_at_reduction_pairs.cuh"
#include "data_definitions/reduce_mask_and_num_action_or_treasure.hpp"

#include "aggregate_by_type_mask_and_num_action_or_treasure.cuh"
#include "constant_memory.cuh"


__host__ __device__ constexpr auto round_up_to_multiple_of(auto input, auto m) {
    return ((input + m - 1) / m)*m;
}


namespace kernels {

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

    for (auto xx = 0; xx < 8250; ++xx)
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

            for (auto pair_i = threadIdx.x; pair_i < round_up_to_multiple_of(data::kNumIndexPairs, blockDim.x); pair_i += blockDim.x) {
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

AggregateByTypeMaskAndNumActionOrTreasure::AggregateByTypeMaskAndNumActionOrTreasure(cudaStream_t stream) : BaseStreamLauncher(stream) {}


float AggregateByTypeMaskAndNumActionOrTreasure::Launch(std::size_t num_blocks, std::size_t block_size) {
    RecordStart();
    aggregate_by_type_mask_and_num_action_or_treasure<<<num_blocks, block_size, 0, stream_>>>();
    return RecordStopSyncAndGetElapsedTime();
}

}
