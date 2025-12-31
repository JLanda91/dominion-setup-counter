#pragma once

#include <cstdint>

#include "data_definitions/reduce_type_and_is_action_or_treasure.cuh"
#include "array_type.hpp"

namespace data {
    inline constexpr uint32_t kNumIndexPairs = 66;
    inline constexpr uint8_t kMaxIndexPairContinuation = 2;

    using reduce_type_and_is_action_or_treasure = array<data_definitions::reduce_type_and_is_action_or_treasure::IndexPair, kNumIndexPairs>;

    extern const reduce_type_and_is_action_or_treasure::host_type kIndexPairs;
}
