#pragma once

#include "base_stream_launcher.cuh"

namespace kernels {
    class AggregateByTypeMaskAndNumActionOrTreasure : public BaseStreamLauncher {
    public:
        explicit AggregateByTypeMaskAndNumActionOrTreasure(cudaStream_t stream);

        float Launch(std::size_t num_blocks, std::size_t block_size);
    };
}