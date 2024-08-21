#include "amount_getters.hpp"

namespace config{
    namespace kingdom {
        auto none_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::NONE_TWO>(t);
        }

        auto none_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::NONE_THREE>(t);
        }

        auto none_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::NONE_FOUR>(t);
        }

        auto none_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::NONE_FIVE>(t);
        }

        auto none_other(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::NONE_OTHER>(t);
        }

        auto action_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_TWO>(t);
        }

        auto action_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_THREE>(t);
        }

        auto action_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_FOUR>(t);
        }

        auto action_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_FIVE>(t);
        }

        auto action_other(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_OTHER>(t);
        }

        auto action_attack_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_TWO>(t);
        }

        auto action_attack_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_THREE>(t);
        }

        auto action_attack_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_FOUR>(t);
        }

        auto action_attack_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_FIVE>(t);
        }

        auto action_attack_other(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_OTHER>(t);
        }

        auto action_attack_doom_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_DOOM_FOUR>(t);
        }

        auto action_attack_doom_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_DOOM_FIVE>(t);
        }

        auto action_attack_duration_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_DURATION_THREE>(t);
        }

        auto action_attack_duration_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_DURATION_FOUR>(t);
        }

        auto action_attack_duration_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_DURATION_FIVE>(t);
        }

        auto action_attack_duration_other(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_DURATION_OTHER>(t);
        }

        auto action_attack_duration_loot_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_DURATION_LOOT_FIVE>(t);
        }

        auto action_attack_looter_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_LOOTER_FOUR>(t);
        }

        auto action_attack_looter_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_LOOTER_FIVE>(t);
        }

        auto action_attack_omen_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_ATTACK_OMEN_FIVE>(t);
        }

        auto action_doom_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_DOOM_THREE>(t);
        }

        auto action_doom_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_DOOM_FIVE>(t);
        }

        auto action_duration_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_DURATION_TWO>(t);
        }

        auto action_duration_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_DURATION_THREE>(t);
        }

        auto action_duration_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_DURATION_FOUR>(t);
        }

        auto action_duration_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_DURATION_FIVE>(t);
        }

        auto action_duration_other(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_DURATION_OTHER>(t);
        }

        auto action_duration_liaison_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_DURATION_LIAISON_THREE>(t);
        }

        auto action_duration_loot_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_DURATION_LOOT_TWO>(t);
        }

        auto action_fate_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_FATE_TWO>(t);
        }

        auto action_fate_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_FATE_THREE>(t);
        }

        auto action_fate_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_FATE_FOUR>(t);
        }

        auto action_fate_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_FATE_FIVE>(t);
        }

        auto action_liaison_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_LIAISON_TWO>(t);
        }

        auto action_liaison_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_LIAISON_THREE>(t);
        }

        auto action_liaison_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_LIAISON_FOUR>(t);
        }

        auto action_liaison_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_LIAISON_FIVE>(t);
        }

        auto action_loot_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_LOOT_FIVE>(t);
        }

        auto action_looter_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_LOOTER_FOUR>(t);
        }

        auto action_omen_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_OMEN_FOUR>(t);
        }

        auto action_omen_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_OMEN_FIVE>(t);
        }

        auto action_omen_other(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_OMEN_OTHER>(t);
        }

        auto action_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ACTION_TREASURE_FIVE>(t);
        }

        auto attack_doom_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ATTACK_DOOM_FIVE>(t);
        }

        auto attack_duration_other(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ATTACK_DURATION_OTHER>(t);
        }

        auto attack_fate_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ATTACK_FATE_TREASURE_FIVE>(t);
        }

        auto attack_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::ATTACK_TREASURE_FIVE>(t);
        }

        auto duration_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::DURATION_TWO>(t);
        }

        auto duration_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::DURATION_THREE>(t);
        }

        auto duration_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::DURATION_FIVE>(t);
        }

        auto duration_liaison_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::DURATION_LIAISON_TREASURE_FIVE>(t);
        }

        auto duration_treasure_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::DURATION_TREASURE_TWO>(t);
        }

        auto duration_treasure_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::DURATION_TREASURE_THREE>(t);
        }

        auto duration_treasure_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::DURATION_TREASURE_FOUR>(t);
        }

        auto duration_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::DURATION_TREASURE_FIVE>(t);
        }

        auto liaison_treasure_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::LIAISON_TREASURE_TWO>(t);
        }

        auto loot_treasure_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::LOOT_TREASURE_TWO>(t);
        }

        auto loot_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::LOOT_TREASURE_FIVE>(t);
        }

        auto loot_treasure_other(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::LOOT_TREASURE_OTHER>(t);
        }

        auto treasure_two(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::TREASURE_TWO>(t);
        }

        auto treasure_three(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::TREASURE_THREE>(t);
        }

        auto treasure_four(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::TREASURE_FOUR>(t);
        }

        auto treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::TREASURE_FIVE>(t);
        }

        auto treasure_other(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return tuple_enum_get<AmountIndex::TREASURE_OTHER>(t);
        }

    }

}
