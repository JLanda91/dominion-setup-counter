#pragma once

#include <cuda/std/cstdint>

namespace card_data::extra_setup {

enum class Card : cuda::std::uint8_t {
    YoungWitch = 0,
    ApproachingArmy = 1,
    WayOfTheMouse = 2,
    Ferryman = 3,
    Riverboat = 4,
    Obelisk = 5
};

inline constexpr cuda::std::uint8_t kNumExtraSetupCards = 6;

}

