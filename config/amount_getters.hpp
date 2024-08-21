#pragma once

#include "typedefs.hpp"

namespace config {
    namespace kingdom {
        template<AmountIndex ... E>
        auto tuple_enum_get(const kingdom_amounts_t& t) noexcept -> coefficient_t {
            return (... + (std::get<std::to_underlying(E)>(t)));
        }

        auto none_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto none_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto none_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto none_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto none_other(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_other(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_other(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_doom_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_doom_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_duration_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_duration_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_duration_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_duration_other(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_duration_loot_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_looter_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_looter_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_attack_omen_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_doom_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_doom_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_duration_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_duration_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_duration_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_duration_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_duration_other(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_duration_liaison_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_duration_loot_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_fate_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_fate_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_fate_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_fate_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_liaison_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_liaison_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_liaison_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_liaison_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_loot_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_looter_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_omen_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_omen_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_omen_other(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto action_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto attack_doom_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto attack_duration_other(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto attack_fate_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto attack_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto duration_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto duration_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto duration_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto duration_liaison_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto duration_treasure_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto duration_treasure_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto duration_treasure_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto duration_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto liaison_treasure_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto loot_treasure_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto loot_treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto loot_treasure_other(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto treasure_two(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto treasure_three(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto treasure_four(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto treasure_five(const kingdom_amounts_t& t) noexcept -> coefficient_t;
        auto treasure_other(const kingdom_amounts_t& t) noexcept -> coefficient_t;
    }

}
