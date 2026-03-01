#pragma once

#include "base_stream_launcher.cuh"

namespace kernels {
    class AggregateByTypeMaskAndNumActionOrTreasure : public BaseStreamLauncher {
    public:
        static constexpr uint32_t kNumEmulatedItemsPerBlock = 5750;

        explicit AggregateByTypeMaskAndNumActionOrTreasure(cudaStream_t stream);

        float Launch(
            std::size_t num_blocks,
            NumThreads num_threads,
            const data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution* gInECSegmentDistribution,
            data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult* gOutPhase1,
            data_definitions::reduce_mask_and_num_action_or_treasure::MultiplierTypeMaskAndNumActionTreasureTable* gOutPhase2);

        static int GetMaxActiveBlocksPerMultiprocessor(NumThreads num_threads);
    };
}