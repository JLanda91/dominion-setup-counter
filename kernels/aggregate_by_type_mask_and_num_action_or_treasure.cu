#include <fmt/format.h>

#include <cooperative_groups.h>
#include <cooperative_groups/memcpy_async.h>
#include <cstdio>

#include "data_definitions/big_uint.cuh"
#include "data_definitions/kingdom.hpp"
#include "data_definitions/reduce_type_and_is_action_or_treasure.cuh"

#include "data/equivalence_classes.hpp"
#include "data/mask_num_at_reduction.cuh"
#include "data/type_at_reduction_pairs.cuh"
#include "data_definitions/reduce_mask_and_num_action_or_treasure.cuh"

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
            data_definitions::reduce_type_and_is_action_or_treasure::ReductionRow reduction_row[2];
            uint16_t work_item_weights[data::kNumEquivalenceClasses];
        } reduce_type_and_is_action_or_treasure{};


        // other reduction phase
    };
};

template<int BlockSize>
__global__ void aggregate_by_type_mask_and_num_action_or_treasure(
    const data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution* gInECSegmentDistribution,
    data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult* gOutPhase1,
    data_definitions::reduce_mask_and_num_action_or_treasure::MultiplierTypeMaskAndNumActionTreasureTable* gOutPhase2)
{
    static_assert(BlockSize == 32);

    namespace cg = cooperative_groups;
    
    auto thread_block = cg::this_thread_block();
    auto warp = cg::tiled_partition<32>(thread_block);
    
    // shared mem
    __shared__ SharedMemStructure sReduce;

    // set work items (prototype: all 1 for complete convolution)
    for (auto i = thread_block.thread_rank(); i < data::kNumEquivalenceClasses; i += BlockSize) {
        sReduce.reduce_type_and_is_action_or_treasure.work_item_weights[i] = (1 << 11) - 1;
    }
    thread_block.sync();

    // buf index
    int buf_i = 0;

    for (auto xx = 0; xx < AggregateByTypeMaskAndNumActionOrTreasure::kNumEmulatedItemsPerBlock; ++xx)
    {
        // for all ec segments (type, is_at, begin offset, end offset)
        for (auto ec_segment_i = 0; ec_segment_i < data::kNumECSegments; ec_segment_i ++) {
            // set prev reduction result to 1,0,0,0,0,0,...
            if (const auto tid = thread_block.thread_rank(); tid < 11) {
                sReduce.reduce_type_and_is_action_or_treasure.reduction_row[buf_i].data[tid] = data_definitions::u128_t{tid > 0 ? 0ul : 1ul};
            }
            thread_block.sync();

            for (auto ec_i = data::cECSegments[ec_segment_i].begin; ec_i < data::cECSegments[ec_segment_i].end; ++ec_i) {
                // set next reduction result to 0
                if (const auto tid = thread_block.thread_rank(); tid < 11) {
                    sReduce.reduce_type_and_is_action_or_treasure.reduction_row[1 - buf_i].data[tid] = data_definitions::u128_t{0ul};
                }
                thread_block.sync();

                // for each convolution index pair
                for (auto pair_i = thread_block.thread_rank(); pair_i < round_up_to_multiple_of(data::kNumIndexPairs, BlockSize); pair_i += BlockSize) {
                    int n = 10; auto contrib = data_definitions::u128_t{0};
                    if (pair_i < data::kNumIndexPairs) {
                        // calculate contribution
                        int n_ec = 10;
                        data::cIndexPairs[pair_i].unpack(n, n_ec);
                        const unsigned w = (sReduce.reduce_type_and_is_action_or_treasure.work_item_weights[ec_i] >> n_ec) & 1;
                        contrib = sReduce.reduce_type_and_is_action_or_treasure.reduction_row[buf_i].data[n - n_ec] * data::cECData[ec_i].binom_data[n_ec] * w;
                    }

                    const int continuation_no = data::cIndexPairs[pair_i].continuation_no();
                    int boundary_reached = 0;

                    // shuffle down warp results and stop shuffling in time
                    for (int lane_offset = 1; lane_offset < 32; lane_offset <<= 1) {
                        const auto add_lo = warp.shfl_down(contrib.limbs[0], lane_offset);
                        const auto add_hi = warp.shfl_down(contrib.limbs[1], lane_offset);
                        const auto shfl_cont_no = warp.shfl_down(continuation_no, lane_offset);
                        const auto shfl_boundary_reached = warp.shfl_down(boundary_reached, lane_offset);

                        // if boundary not yet reached
                        if (!boundary_reached) {
                            // if out of bounds or shuffled continuation is 1 (new (n,k) segment) then stop
                            if (warp.thread_rank() + lane_offset >= warp.size() || shfl_cont_no == 1) {
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

                    if (continuation_no > 0) {
                        sReduce.reduce_type_and_is_action_or_treasure.reduction_row[1 - buf_i].data[n] += contrib;
                    }
                    thread_block.sync();

                }

                // after reduction of one EC, swap double buffer index
                buf_i = 1 - buf_i;

            }

            // after all is reduced for an EC segment, write reduced row to shared mem
            if (const auto tid = thread_block.thread_rank(); tid < 11) {
                sReduce.reduce_type_and_is_action_or_treasure_result.data[ec_segment_i][tid] = sReduce.reduce_type_and_is_action_or_treasure.reduction_row[buf_i].data[tid];
            }
            thread_block.sync();
        }

        // TEMP: copy to gmem
        // if (blockIdx.x == 0)
        //     for (auto out_i = thread_block.thread_rank(); out_i < 11*11; out_i += BlockSize) {
        //         (&gOutPhase1->data[0][0])[out_i] = (&sReduce.reduce_type_and_is_action_or_treasure_result.data[0][0])[out_i];
        //     }


        // Phase 2: For each points in the buffer
        for (auto point_i = thread_block.thread_rank(); point_i < round_up_to_multiple_of(data::kNumECSegmentDistributions, BlockSize); point_i += BlockSize) {
            auto contrib = data_definitions::u128_t{0};
            auto continuation_no = 0;
            uint8_t type_mask = 0u;
            uint8_t k = 0u;

            if (point_i < data::kNumECSegmentDistributions) {
                contrib = data_definitions::u128_t{1};
                continuation_no = gInECSegmentDistribution[point_i].continuation_no;

                // multiply the phase 1 results to get contribution
                for (auto i = 0; i < 11; ++i ) {
                    auto distr_elem = gInECSegmentDistribution[point_i].data[i/2];
                    auto read_i = i%2 == 0 ? (distr_elem & 0xF) : (distr_elem >> 4);
                    contrib *= sReduce.reduce_type_and_is_action_or_treasure_result.data[i][read_i];
                    type_mask |= (read_i > 0) * (1 << static_cast<std::underlying_type_t<data_definitions::kingdom::MultiplierType>>(data::cECSegments[i].effective_card_type) );
                    k += data::cECSegments[i].is_action_treasure * read_i;
                }
            }

            int boundary_reached = 0;



            for (int lane_offset = 1; lane_offset < 32; lane_offset <<= 1) {
                auto incr = data_definitions::u128_t{0};
                incr.limbs[0] = warp.shfl_down(contrib.limbs[0], lane_offset);
                incr.limbs[1] = warp.shfl_down(contrib.limbs[1], lane_offset);
                const auto shfl_cont_no = warp.shfl_down(continuation_no, lane_offset);
                const auto shfl_boundary_reached = warp.shfl_down(boundary_reached, lane_offset);

                // if boundary not yet reached
                if (!boundary_reached) {
                    // if out of bounds or shuffled continuation is 1 (new (n,k) segment) then stop
                    if (warp.thread_rank() + lane_offset >= warp.size() || shfl_cont_no == 1) {
                        boundary_reached = true;
                        continue;
                    }

                    // do addition
                    contrib += incr;

                    // if offset lane has stopped processing, stop processing now
                    if (shfl_boundary_reached) {
                        boundary_reached = true;
                    }
                }
            }

            if (blockIdx.x == 0 && continuation_no > 0) {
                gOutPhase2->data[type_mask][k] += contrib;
            }

        }
    }
}

AggregateByTypeMaskAndNumActionOrTreasure::AggregateByTypeMaskAndNumActionOrTreasure(cudaStream_t stream) : BaseStreamLauncher(stream) {}


float AggregateByTypeMaskAndNumActionOrTreasure::Launch(
    std::size_t num_blocks,
    NumThreads num_threads,
    const data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution* gInECSegmentDistribution,
    data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult* gOutPhase1,
    data_definitions::reduce_mask_and_num_action_or_treasure::MultiplierTypeMaskAndNumActionTreasureTable* gOutPhase2)
{
    fmt::println("Launching aggregate_by_type_mask_and_num_action_or_treasure with block size {}", static_cast<std::underlying_type_t<NumThreads>>(num_threads));
#ifdef DOUBLE_BUFFERED_PHASE_TWO
    fmt::println("Double buffered approach.");
#else
    fmt::println("Naive approach.");
#endif

    if (num_threads == NumThreads::ThirtyTwo) {
        RecordStart();
        aggregate_by_type_mask_and_num_action_or_treasure<32><<<num_blocks, 32, 0, stream_>>>(
            gInECSegmentDistribution,
            gOutPhase1,
            gOutPhase2);
        return RecordStopSyncAndGetElapsedTime();
    }
    // if (num_threads == NumThreads::SixtyFour) {
    //     RecordStart();
    //     aggregate_by_type_mask_and_num_action_or_treasure<64><<<num_blocks, 64, 0, stream_>>>(
    //         gInECSegmentDistribution,
    //         gOutPhase1,
    //         gOutPhase2);
    //     return RecordStopSyncAndGetElapsedTime();
    // }
    throw std::invalid_argument("Kernel launch not implemented for this block size.");

}

int AggregateByTypeMaskAndNumActionOrTreasure::GetMaxActiveBlocksPerMultiprocessor(NumThreads num_threads) {
    fmt::println("Obtaining max number of active blocks per multiprocessor with block size {}", static_cast<std::underlying_type_t<NumThreads>>(num_threads));

    int num_blocks{};

    if (num_threads == NumThreads::ThirtyTwo) {
        cudaOccupancyMaxActiveBlocksPerMultiprocessor(
            &num_blocks,
            aggregate_by_type_mask_and_num_action_or_treasure<32>,
            static_cast<std::underlying_type_t<NumThreads>>(num_threads),
            0);
        return num_blocks;
    }
    // if (num_threads == NumThreads::SixtyFour) {
    //     cudaOccupancyMaxActiveBlocksPerMultiprocessor(
    //         &num_blocks,
    //         aggregate_by_type_mask_and_num_action_or_treasure<64>,
    //         static_cast<std::underlying_type_t<NumThreads>>(num_threads),
    //         0);
    // }
    throw std::invalid_argument("GetMaxActiveBlocksPerMultiprocessor not implemented for this block size.");
}

}
