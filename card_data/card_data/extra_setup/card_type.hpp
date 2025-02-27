#pragma once

#include <cstdint>
#include <vector>
#include <ranges>

#include "../kingdom/card_type.hpp"

namespace card_data::extra_setup {
    enum class CardType : uint8_t {
        YoungWitch,
        ApproachingArmy,
        WayOfTheMouse,
        Ferryman,
        Riverboat,
        Obelisk
    };

    using dispatch_vector_t = std::vector<CardType>;

    template<kingdom::CardType C, bool AddApproachingArmy>
    dispatch_vector_t new_dispatch(const dispatch_vector_t& dispatch){
        static_assert((C == kingdom::CardType::Omen) || !AddApproachingArmy);

        std::vector<CardType> result = dispatch | std::views::drop(1) | std::ranges::to<std::vector>();
        if constexpr(AddApproachingArmy){
            result.push_back(CardType::ApproachingArmy); // approaching army picker card index
        }
        if constexpr(C == kingdom::CardType::YoungWitch){ // young witch column
            result.push_back(CardType::YoungWitch); // young witch picker card index
        } else if constexpr(C == kingdom::CardType::Ferryman) { // ferryman column
            result.push_back(CardType::Ferryman); // ferryman picker card index
        } else if constexpr(C == kingdom::CardType::Riverboat) { // riverboat column
            result.push_back(CardType::Riverboat); // riverboat picker card index
        }
        std::ranges::sort(result);
        return result;
    }
}