#pragma once

#include "base_stream_launcher.cuh"

namespace kernels {
    class AggregateByTypeMaskAndNumActionOrTreasure : public BaseStreamLauncher {
    public:
        explicit AggregateByTypeMaskAndNumActionOrTreasure(cudaStream_t stream);

        float Launch(
            std::size_t num_blocks,
            std::size_t block_size,
            const data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution* gInECSegmentDistribution,
            data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult* gOutPhase1);

        static int GetMaxActiveBlocksPerMultiprocessor(int blockSize);
    };
}