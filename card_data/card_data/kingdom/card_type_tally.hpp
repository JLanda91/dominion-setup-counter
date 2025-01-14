#pragma once

#include <cstdint>

namespace card_data::kingdom {
    struct CardTypeTally {
        uint8_t amount_young_witch;
        uint8_t amount_knights;
        uint8_t amount_druid;
        uint8_t amount_ferryman;
        uint8_t amount_riverboat;
        uint8_t amount_looter;
        uint8_t amount_fate;
        uint8_t amount_doom;
        uint8_t amount_liaison;
        uint8_t amount_omen;
        uint8_t amount_loot;
        uint8_t amount_none;
    };

    inline constexpr std::size_t kNumCardTypes = 12uz;

    template<std::size_t N>
    constexpr auto card_type_name() noexcept -> std::string_view {
        static_assert(N < kNumCardTypes, "Index out of bounds");

    	if constexpr (N == 0) { return "Young Witch"; }
    	else if constexpr (N == 1) { return "Knights"; }
    	else if constexpr (N == 2) { return "Druid"; }
    	else if constexpr (N == 3) { return "Ferryman"; }
    	else if constexpr (N == 4) { return "Riverboat"; }
    	else if constexpr (N == 5) { return "Looter"; }
    	else if constexpr (N == 6) { return "Fate"; }
    	else if constexpr (N == 7) { return "Doom"; }
    	else if constexpr (N == 8) { return "Liaison"; }
    	else if constexpr (N == 9) { return "Omen"; }
    	else if constexpr (N == 10) { return "Loot"; }
    	else { return "None"; }
    }

    template<std::size_t N>
    constexpr auto& card_type_amount(CardTypeTally& tally) noexcept {
        static_assert(N < kNumCardTypes, "Index out of bounds");

    	if constexpr (N == 0) { return tally.amount_young_witch; }
    	else if constexpr (N == 1) { return tally.amount_knights; }
    	else if constexpr (N == 2) { return tally.amount_druid; }
    	else if constexpr (N == 3) { return tally.amount_ferryman; }
    	else if constexpr (N == 4) { return tally.amount_riverboat; }
    	else if constexpr (N == 5) { return tally.amount_looter; }
    	else if constexpr (N == 6) { return tally.amount_fate; }
    	else if constexpr (N == 7) { return tally.amount_doom; }
    	else if constexpr (N == 8) { return tally.amount_liaison; }
    	else if constexpr (N == 9) { return tally.amount_omen; }
    	else if constexpr (N == 10) { return tally.amount_loot; }
    	else { return tally.amount_none; }
    }

    template<std::size_t N>
    constexpr const auto& card_type_amount(const CardTypeTally& tally) noexcept {
        static_assert(N < kNumCardTypes, "Index out of bounds");

    	if constexpr (N == 0) { return tally.amount_young_witch; }
    	else if constexpr (N == 1) { return tally.amount_knights; }
    	else if constexpr (N == 2) { return tally.amount_druid; }
    	else if constexpr (N == 3) { return tally.amount_ferryman; }
    	else if constexpr (N == 4) { return tally.amount_riverboat; }
    	else if constexpr (N == 5) { return tally.amount_looter; }
    	else if constexpr (N == 6) { return tally.amount_fate; }
    	else if constexpr (N == 7) { return tally.amount_doom; }
    	else if constexpr (N == 8) { return tally.amount_liaison; }
    	else if constexpr (N == 9) { return tally.amount_omen; }
    	else if constexpr (N == 10) { return tally.amount_loot; }
    	else { return tally.amount_none; }
    }

    constexpr auto card_type_total(const CardTypeTally& tally) noexcept -> uint8_t {
        return 
    		tally.amount_young_witch +
    		tally.amount_knights +
    		tally.amount_druid +
    		tally.amount_ferryman +
    		tally.amount_riverboat +
    		tally.amount_looter +
    		tally.amount_fate +
    		tally.amount_doom +
    		tally.amount_liaison +
    		tally.amount_omen +
    		tally.amount_loot +
    		tally.amount_none ;
    }

}
