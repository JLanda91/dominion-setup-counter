#pragma once

#include <cstdint>
#include <array>

#include "data_definitions/reduce_mask_and_num_action_or_treasure.cuh"

#include "array_type.hpp"

namespace data {
    inline constexpr uint32_t kNumECSegmentDistributions = 69'928;

    using reduce_mask_and_num_action_or_treasure = array<data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution, kNumECSegmentDistributions>;

    extern const reduce_mask_and_num_action_or_treasure::host_type kECSegmentDistributions;

}
