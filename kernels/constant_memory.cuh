#pragma once

#include "data/equivalence_classes.hpp"
#include "data/type_at_reduction_pairs.cuh"

namespace data {
    extern __device__ __constant__ kingdom_equivalence_classes::device_type cECData;
    extern __device__ __constant__ kingdom_equivalence_class_segments::device_type cECSegments;
    extern __device__ __constant__ reduce_type_and_is_action_or_treasure::device_type cIndexPairs;
}

namespace kernels {
    void initialize_cmem(cudaStream_t stream);
}