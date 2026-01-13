#include <cooperative_groups.h>
#include <cooperative_groups/memcpy_async.h>

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

static constexpr uint32_t kPointsPerThreadPerStage = 6;
static constexpr uint32_t kPointsPerStage = kPointsPerThreadPerStage * 32;
static constexpr uint32_t kNumStages = (data::kNumECSegmentDistributions + kPointsPerStage - 1)/kPointsPerStage;
static constexpr uint32_t kPointsLastStage = data::kNumECSegmentDistributions % kPointsPerStage;

struct SharedMemStructure {
    data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult reduce_type_and_is_action_or_treasure_result;
    union {
        struct {
            data_definitions::reduce_type_and_is_action_or_treasure::ReductionRow reduction_row[2];
            uint16_t work_item_weights[data::kNumEquivalenceClasses];
        } reduce_type_and_is_action_or_treasure{};
        data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution reduce_mask_and_num_at_buffer[2][kPointsPerStage];

        // other reduction phase
    };
};

__global__ void aggregate_by_type_mask_and_num_action_or_treasure(
    const data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution* gInECSegmentDistribution,
    data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult* gOutPhase1)
{
    namespace cg = cooperative_groups;
    
    auto thread_block = cg::this_thread_block();
    auto warp = cg::tiled_partition<32>(thread_block);
    
    // shared mem
    __shared__ SharedMemStructure sReduce;

    // set work items (prototype: all 1 for complete convolution)
    for (auto i = thread_block.thread_rank(); i < data::kNumEquivalenceClasses; i += thread_block.size()) {
        sReduce.reduce_type_and_is_action_or_treasure.work_item_weights[i] = (1 << 11) - 1;
    }
    thread_block.sync();

    // buf index
    int buf_i = 0;

    for (auto xx = 0; xx < 100000; ++xx)
    // for all ec segments (type, is_at, begin offset, end offset)
    for (auto ec_segment_i = 0; ec_segment_i < data::kNumECSegments; ec_segment_i ++) {
        // set prev reduction result to 1,0,0,0,0,0,...
        for (auto r = thread_block.thread_rank(); r < 11; r += thread_block.size()) {
            sReduce.reduce_type_and_is_action_or_treasure.reduction_row[buf_i].data[r] = data_definitions::u128_t{0ul};
        }
        if (thread_block.thread_rank() == 0) {
            sReduce.reduce_type_and_is_action_or_treasure.reduction_row[buf_i].data[0] = data_definitions::u128_t{1ul};
        }
        thread_block.sync();

        for (auto ec_i = data::cECSegments[ec_segment_i].begin; ec_i < data::cECSegments[ec_segment_i].end; ++ec_i) {
            // set next reduction result to 0
            for (auto m = thread_block.thread_rank(); m < 11; m += thread_block.size()) {
                sReduce.reduce_type_and_is_action_or_treasure.reduction_row[1 - buf_i].data[m] = data_definitions::u128_t{0ul};
            }
            thread_block.sync();

            for (auto pair_i = thread_block.thread_rank(); pair_i < round_up_to_multiple_of(data::kNumIndexPairs, thread_block.size()); pair_i += thread_block.size()) {
                int n = 10; auto contrib = data_definitions::u128_t{0};
                if (pair_i < data::kNumIndexPairs) {
                    int n_ec = 10;
                    data::cIndexPairs[pair_i].unpack(n, n_ec);
                    const unsigned w = (sReduce.reduce_type_and_is_action_or_treasure.work_item_weights[ec_i] >> n_ec) & 1;
                    contrib = sReduce.reduce_type_and_is_action_or_treasure.reduction_row[buf_i].data[n - n_ec] * data::cECData[ec_i].binom_data[n_ec] * w;
                }

                const int continuation_no = data::cIndexPairs[pair_i].continuation_no();
                int boundary_reached = 0;

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

                for (auto c = 1; c <= data::kMaxIndexPairContinuation; c++) {
                    if (c == continuation_no) {
                        sReduce.reduce_type_and_is_action_or_treasure.reduction_row[1 - buf_i].data[n] += contrib;
                    }
                    thread_block.sync();
                }
            }

            // after reduction of one EC, swap double buffer index
            buf_i = 1 - buf_i;
            // thread_block.sync();
            
        }

        // after all is reduced for an EC segment, write reduced row to shared mem
        for (auto m = thread_block.thread_rank(); m < 11; m += thread_block.size()) {
            sReduce.reduce_type_and_is_action_or_treasure_result.data[ec_segment_i][m] = sReduce.reduce_type_and_is_action_or_treasure.reduction_row[buf_i].data[m];
        }
        thread_block.sync();
    }

    // // TEMP: copy to gmem
    // if (blockIdx.x == 0)
    // for (auto out_i = thread_block.thread_rank(); out_i < 11*11; out_i += thread_block.size()) {
    //     (&gOutPhase1->data[0][0])[out_i] = (&sReduce.reduce_type_and_is_action_or_treasure_result.data[0][0])[out_i];
    // }
    //
    //
    // buf_i = 0;
    // uint32_t num_points_to_process = kPointsPerStage;
    // uint32_t num_points_to_buffer = kPointsPerStage;
    //
    // cg::memcpy_async(thread_block, sReduce.reduce_mask_and_num_at_buffer[buf_i], &gInECSegmentDistribution[0], kPointsPerStage * sizeof(data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution));
    //
    // for (auto stage_i = 0u; stage_i < kNumStages; stage_i++) {
    //     // await previous buffer
    //     cg::wait(thread_block);
    //
    //     // set process size to previous buffer size
    //     num_points_to_process = num_points_to_buffer;
    //
    //     // if next stage is last stage, set memcpy size to remainder
    //     if (stage_i + 1 == kNumStages - 1) {
    //         num_points_to_buffer = kPointsLastStage;
    //     }
    //
    //     // if stage is not the last one, schedule next memcpy to next buf at 1 - buf_i
    //     if (stage_i < kNumStages - 1) {
    //         cg::memcpy_async(thread_block, sReduce.reduce_mask_and_num_at_buffer[1 - buf_i], &gInECSegmentDistribution[kPointsPerStage * (stage_i + 1)], num_points_to_buffer * sizeof(data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution));
    //     }
    //
    //
    //     for (auto sbuf_i = thread_block.thread_rank(); sbuf_i < num_points_to_process; sbuf_i += thread_block.size()) {
    //         auto contrib = data_definitions::u128_t{1};
    //         auto continuation_no = sReduce.reduce_mask_and_num_at_buffer[buf_i][sbuf_i].continuation_no;
    //         uint8_t type_mask = 0u;
    //         uint8_t k = 0u;
    //
    //         for (auto i = 0; i < 11; ++i ) {
    //             auto distr_elem = sReduce.reduce_mask_and_num_at_buffer[buf_i][sbuf_i].data[i/2];
    //             auto read_i = i%2 == 0 ? distr_elem & 0xF : distr_elem >> 4;
    //             contrib *= sReduce.reduce_type_and_is_action_or_treasure_result.data[i][read_i];
    //             type_mask |= (read_i > 0) * (1 << static_cast<std::underlying_type_t<data_definitions::kingdom::MultiplierType>>(data::cECSegments[i].effective_card_type) );
    //             k += data::cECSegments[i].is_action_treasure * read_i;
    //         }
    //
    //     }
    //
    //
    //     // end of loop: change bufs
    //     buf_i = 1 - buf_i;
    // }
}

AggregateByTypeMaskAndNumActionOrTreasure::AggregateByTypeMaskAndNumActionOrTreasure(cudaStream_t stream) : BaseStreamLauncher(stream) {}


float AggregateByTypeMaskAndNumActionOrTreasure::Launch(
    std::size_t num_blocks,
    std::size_t block_size,
    const data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution* gInECSegmentDistribution,
    data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult* gOutPhase1)
{
    RecordStart();
    aggregate_by_type_mask_and_num_action_or_treasure<<<num_blocks, block_size, 0, stream_>>>(
        gInECSegmentDistribution,
        gOutPhase1);
    return RecordStopSyncAndGetElapsedTime();
}

int AggregateByTypeMaskAndNumActionOrTreasure::GetMaxActiveBlocksPerMultiprocessor(int blockSize) {
    int num_blocks{};

    cudaOccupancyMaxActiveBlocksPerMultiprocessor(
        &num_blocks,
        aggregate_by_type_mask_and_num_action_or_treasure,
        blockSize,
        0);

    return num_blocks;
}

}
