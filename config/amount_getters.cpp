#include "amount_getters.hpp"

namespace config::kingdom {
    auto looter_total(const kingdom_amounts_t& t) noexcept -> coefficient_t {
        return sum_tuple_on_enum_index<
            AmountIndex::ACTION_ATTACK_LOOTER_FOUR,
			AmountIndex::ACTION_ATTACK_LOOTER_FIVE,
			AmountIndex::ACTION_LOOTER_FOUR
        >(t);
    }

    auto fate_total(const kingdom_amounts_t& t) noexcept -> coefficient_t {
        return sum_tuple_on_enum_index<
            AmountIndex::ACTION_FATE_TWO,
			AmountIndex::ACTION_FATE_THREE,
			AmountIndex::ACTION_FATE_FOUR,
			AmountIndex::ACTION_FATE_FIVE,
			AmountIndex::ATTACK_FATE_TREASURE_FIVE
        >(t);
    }

    auto doom_total(const kingdom_amounts_t& t) noexcept -> coefficient_t {
        return sum_tuple_on_enum_index<
            AmountIndex::ACTION_ATTACK_DOOM_FOUR,
			AmountIndex::ACTION_ATTACK_DOOM_FIVE,
			AmountIndex::ACTION_DOOM_THREE,
			AmountIndex::ACTION_DOOM_FIVE,
			AmountIndex::ATTACK_DOOM_FIVE
        >(t);
    }

    auto liaison_total(const kingdom_amounts_t& t) noexcept -> coefficient_t {
        return sum_tuple_on_enum_index<
            AmountIndex::ACTION_DURATION_LIAISON_THREE,
			AmountIndex::ACTION_LIAISON_TWO,
			AmountIndex::ACTION_LIAISON_THREE,
			AmountIndex::ACTION_LIAISON_FOUR,
			AmountIndex::ACTION_LIAISON_FIVE,
			AmountIndex::DURATION_LIAISON_TREASURE_FIVE,
			AmountIndex::LIAISON_TREASURE_TWO
        >(t);
    }

    auto loot_total(const kingdom_amounts_t& t) noexcept -> coefficient_t {
        return sum_tuple_on_enum_index<
            AmountIndex::ACTION_ATTACK_DURATION_LOOT_FIVE,
			AmountIndex::ACTION_DURATION_LOOT_TWO,
			AmountIndex::ACTION_LOOT_FIVE,
			AmountIndex::LOOT_TREASURE_TWO,
			AmountIndex::LOOT_TREASURE_FIVE,
			AmountIndex::LOOT_TREASURE_OTHER
        >(t);
    }

    auto two_three_total(const kingdom_amounts_t& t) noexcept -> coefficient_t {
        return sum_tuple_on_enum_index<
            AmountIndex::NONE_TWO,
			AmountIndex::NONE_THREE,
			AmountIndex::ACTION_TWO,
			AmountIndex::ACTION_THREE,
			AmountIndex::ACTION_ATTACK_TWO,
			AmountIndex::ACTION_ATTACK_THREE,
			AmountIndex::ACTION_ATTACK_DURATION_THREE,
			AmountIndex::ACTION_DOOM_THREE,
			AmountIndex::ACTION_DURATION_TWO,
			AmountIndex::ACTION_DURATION_THREE,
			AmountIndex::ACTION_DURATION_LIAISON_THREE,
			AmountIndex::ACTION_DURATION_LOOT_TWO,
			AmountIndex::ACTION_FATE_TWO,
			AmountIndex::ACTION_FATE_THREE,
			AmountIndex::ACTION_LIAISON_TWO,
			AmountIndex::ACTION_LIAISON_THREE,
			AmountIndex::DURATION_TWO,
			AmountIndex::DURATION_THREE,
			AmountIndex::DURATION_TREASURE_TWO,
			AmountIndex::DURATION_TREASURE_THREE,
			AmountIndex::LIAISON_TREASURE_TWO,
			AmountIndex::LOOT_TREASURE_TWO,
			AmountIndex::TREASURE_TWO,
			AmountIndex::TREASURE_THREE
        >(t);
    }

    auto three_four_unused(const kingdom_amounts_t& t, const kingdom_amounts_t& max) noexcept -> coefficient_t {
        return diff_tuple_on_enum_index<
            AmountIndex::NONE_THREE,
			AmountIndex::NONE_FOUR,
			AmountIndex::ACTION_THREE,
			AmountIndex::ACTION_FOUR,
			AmountIndex::ACTION_ATTACK_THREE,
			AmountIndex::ACTION_ATTACK_FOUR,
			AmountIndex::ACTION_ATTACK_DOOM_FOUR,
			AmountIndex::ACTION_ATTACK_DURATION_THREE,
			AmountIndex::ACTION_ATTACK_DURATION_FOUR,
			AmountIndex::ACTION_ATTACK_LOOTER_FOUR,
			AmountIndex::ACTION_DOOM_THREE,
			AmountIndex::ACTION_DURATION_THREE,
			AmountIndex::ACTION_DURATION_FOUR,
			AmountIndex::ACTION_DURATION_LIAISON_THREE,
			AmountIndex::ACTION_FATE_THREE,
			AmountIndex::ACTION_FATE_FOUR,
			AmountIndex::ACTION_LIAISON_THREE,
			AmountIndex::ACTION_LIAISON_FOUR,
			AmountIndex::ACTION_LOOTER_FOUR,
			AmountIndex::ACTION_OMEN_FOUR,
			AmountIndex::DURATION_THREE,
			AmountIndex::DURATION_TREASURE_THREE,
			AmountIndex::DURATION_TREASURE_FOUR,
			AmountIndex::TREASURE_THREE,
			AmountIndex::TREASURE_FOUR
        >(t, max);
    }
    
    auto action_nonduration_five_unused(const kingdom_amounts_t& t, const kingdom_amounts_t& max) noexcept -> coefficient_t {
        return diff_tuple_on_enum_index<
            AmountIndex::ACTION_FIVE,
			AmountIndex::ACTION_ATTACK_FIVE,
			AmountIndex::ACTION_ATTACK_DOOM_FIVE,
			AmountIndex::ACTION_ATTACK_LOOTER_FIVE,
			AmountIndex::ACTION_ATTACK_OMEN_FIVE,
			AmountIndex::ACTION_DOOM_FIVE,
			AmountIndex::ACTION_FATE_FIVE,
			AmountIndex::ACTION_LIAISON_FIVE,
			AmountIndex::ACTION_LOOT_FIVE,
			AmountIndex::ACTION_OMEN_FIVE,
			AmountIndex::ACTION_TREASURE_FIVE
        >(t, max);
    }
    
    auto action_total(const kingdom_amounts_t& t) noexcept -> coefficient_t {
        return sum_tuple_on_enum_index<
            AmountIndex::ACTION_TWO,
			AmountIndex::ACTION_THREE,
			AmountIndex::ACTION_FOUR,
			AmountIndex::ACTION_FIVE,
			AmountIndex::ACTION_OTHER,
			AmountIndex::ACTION_ATTACK_TWO,
			AmountIndex::ACTION_ATTACK_THREE,
			AmountIndex::ACTION_ATTACK_FOUR,
			AmountIndex::ACTION_ATTACK_FIVE,
			AmountIndex::ACTION_ATTACK_OTHER,
			AmountIndex::ACTION_ATTACK_DOOM_FOUR,
			AmountIndex::ACTION_ATTACK_DOOM_FIVE,
			AmountIndex::ACTION_ATTACK_DURATION_THREE,
			AmountIndex::ACTION_ATTACK_DURATION_FOUR,
			AmountIndex::ACTION_ATTACK_DURATION_FIVE,
			AmountIndex::ACTION_ATTACK_DURATION_OTHER,
			AmountIndex::ACTION_ATTACK_DURATION_LOOT_FIVE,
			AmountIndex::ACTION_ATTACK_LOOTER_FOUR,
			AmountIndex::ACTION_ATTACK_LOOTER_FIVE,
			AmountIndex::ACTION_ATTACK_OMEN_FIVE,
			AmountIndex::ACTION_DOOM_THREE,
			AmountIndex::ACTION_DOOM_FIVE,
			AmountIndex::ACTION_DURATION_TWO,
			AmountIndex::ACTION_DURATION_THREE,
			AmountIndex::ACTION_DURATION_FOUR,
			AmountIndex::ACTION_DURATION_FIVE,
			AmountIndex::ACTION_DURATION_OTHER,
			AmountIndex::ACTION_DURATION_LIAISON_THREE,
			AmountIndex::ACTION_DURATION_LOOT_TWO,
			AmountIndex::ACTION_FATE_TWO,
			AmountIndex::ACTION_FATE_THREE,
			AmountIndex::ACTION_FATE_FOUR,
			AmountIndex::ACTION_FATE_FIVE,
			AmountIndex::ACTION_LIAISON_TWO,
			AmountIndex::ACTION_LIAISON_THREE,
			AmountIndex::ACTION_LIAISON_FOUR,
			AmountIndex::ACTION_LIAISON_FIVE,
			AmountIndex::ACTION_LOOT_FIVE,
			AmountIndex::ACTION_LOOTER_FOUR,
			AmountIndex::ACTION_OMEN_FOUR,
			AmountIndex::ACTION_OMEN_FIVE,
			AmountIndex::ACTION_OMEN_OTHER,
			AmountIndex::ACTION_TREASURE_FIVE
        >(t);
    }

    auto action_two_three_unused(const kingdom_amounts_t& t, const kingdom_amounts_t& max) noexcept -> coefficient_t {
        return diff_tuple_on_enum_index<
            AmountIndex::ACTION_TWO,
			AmountIndex::ACTION_THREE,
			AmountIndex::ACTION_ATTACK_TWO,
			AmountIndex::ACTION_ATTACK_THREE,
			AmountIndex::ACTION_ATTACK_DURATION_THREE,
			AmountIndex::ACTION_DOOM_THREE,
			AmountIndex::ACTION_DURATION_TWO,
			AmountIndex::ACTION_DURATION_THREE,
			AmountIndex::ACTION_DURATION_LIAISON_THREE,
			AmountIndex::ACTION_DURATION_LOOT_TWO,
			AmountIndex::ACTION_FATE_TWO,
			AmountIndex::ACTION_FATE_THREE,
			AmountIndex::ACTION_LIAISON_TWO,
			AmountIndex::ACTION_LIAISON_THREE
        >(t, max);
    }
    
    auto attack_unused(const kingdom_amounts_t& t, const kingdom_amounts_t& max) noexcept -> coefficient_t {
        return diff_tuple_on_enum_index<
            AmountIndex::ACTION_ATTACK_TWO,
			AmountIndex::ACTION_ATTACK_THREE,
			AmountIndex::ACTION_ATTACK_FOUR,
			AmountIndex::ACTION_ATTACK_FIVE,
			AmountIndex::ACTION_ATTACK_OTHER,
			AmountIndex::ACTION_ATTACK_DOOM_FOUR,
			AmountIndex::ACTION_ATTACK_DOOM_FIVE,
			AmountIndex::ACTION_ATTACK_DURATION_THREE,
			AmountIndex::ACTION_ATTACK_DURATION_FOUR,
			AmountIndex::ACTION_ATTACK_DURATION_FIVE,
			AmountIndex::ACTION_ATTACK_DURATION_OTHER,
			AmountIndex::ACTION_ATTACK_DURATION_LOOT_FIVE,
			AmountIndex::ACTION_ATTACK_LOOTER_FOUR,
			AmountIndex::ACTION_ATTACK_LOOTER_FIVE,
			AmountIndex::ACTION_ATTACK_OMEN_FIVE,
			AmountIndex::ATTACK_DOOM_FIVE,
			AmountIndex::ATTACK_DURATION_OTHER,
			AmountIndex::ATTACK_FATE_TREASURE_FIVE,
			AmountIndex::ATTACK_TREASURE_FIVE
        >(t, max);
    }
    
}
