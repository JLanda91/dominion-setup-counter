#pragma once

#include "big_uint.cuh"

namespace data_definitions::reduce_type_and_is_action_or_treasure {

struct ReductionRow {
    u128_t data[11]{};
};

struct ReductionResult {
    u128_t data[11][11]{};
};

struct IndexPair {
    uint16_t data{};

    constexpr IndexPair() = default;

    constexpr IndexPair(uint8_t n, uint8_t n_ec, uint8_t cont)
        : data(((n & 0x0F) << 8) | ((n_ec & 0x0F) << 4) | (cont & 0x0F))
    {}

    __host__ __device__ __forceinline__ void unpack(int& n, int& n_ec) const {
        n_ec = (data >> 4) & 0x0F;
        n = (data >> 8) & 0x0F;
    }

    __host__ __device__ __forceinline__ uint8_t continuation_no() const {
        return data & 0x0F;
    }
};
}
