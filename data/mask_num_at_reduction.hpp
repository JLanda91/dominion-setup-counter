#pragma once

#include <cstdint>
#include <array>

#include "data_definitions/reduce_mask_and_num_action_or_treasure.hpp"

#include "array_type.hpp"

namespace data {
    inline constexpr uint32_t kNumECSegmentDistributions = 69'928;
    inline constexpr uint8_t kMaxECSegmentContinuation = 13;

    using reduce_mask_and_num_action_or_treasure = array<data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution, kNumECSegmentDistributions>;
    using ec_segment_continuations = array<uint8_t, 256u>;

    extern const reduce_mask_and_num_action_or_treasure::host_type kECSegmentDistributions;
    extern const ec_segment_continuations::host_type kECSegmentContinuations;

}
