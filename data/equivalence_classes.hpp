#pragma once

#include <cstdint>
#include <array>

#include "data_definitions/kingdom.hpp"

#include "array_type.hpp"

namespace data {
    inline constexpr uint8_t kNumEquivalenceClasses = 53;
    inline constexpr uint8_t kNumECSegments = 11;

    using kingdom_equivalence_classes = array<data_definitions::kingdom::EquivalenceClass, kNumEquivalenceClasses>;
    using kingdom_equivalence_class_segments = array<data_definitions::kingdom::EquivalenceClassSegment, kNumECSegments>;

    extern const kingdom_equivalence_classes::host_type kECData;
    extern const kingdom_equivalence_class_segments::host_type kECSegments;

}