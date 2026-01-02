#include <sparkplug/util/cuda/check_cuda_fn_error.cuh>

#include "constant_memory.cuh"

namespace data {
    __constant__ kingdom_equivalence_classes::device_type cECData{};
    __constant__ kingdom_equivalence_class_segments::device_type cECSegments{};
    __constant__ reduce_type_and_is_action_or_treasure::device_type cIndexPairs{};
}

namespace kernels {
    void initialize_cmem(cudaStream_t stream) {
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
}