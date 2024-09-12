#pragma once

#include "lib/typedefs.hpp"

namespace config {
    namespace kingdom {
        template<AmountIndex ... E>
        auto enum_index_get_sum(const amounts_t& t) noexcept -> coefficient_t {
            return (... + (std::get<std::to_underlying(E)>(t)));
        }
                
        template<AmountIndex ... E>
        auto enum_index_get_sum_diff(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t {
            return (... + (std::get<std::to_underlying(E)>(max) - std::get<std::to_underlying(E)>(t)));
        }

        auto looter_total(const amounts_t& t) noexcept -> coefficient_t;
        auto fate_total(const amounts_t& t) noexcept -> coefficient_t;
        auto doom_total(const amounts_t& t) noexcept -> coefficient_t;
        auto liaison_total(const amounts_t& t) noexcept -> coefficient_t;
        auto loot_total(const amounts_t& t) noexcept -> coefficient_t;
        auto two_three_total(const amounts_t& t) noexcept -> coefficient_t;
        auto unused_three_four_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t;
        auto unused_action_nonduration_five_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t;
        auto action_total(const amounts_t& t) noexcept -> coefficient_t;
        auto unused_action_two_three_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t;
        auto unused_attack_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t;
        auto action_or_treasure_total(const amounts_t& t) noexcept -> coefficient_t;

        auto has_young_witch(const amounts_t& t) noexcept -> bool;
        auto has_ferryman(const amounts_t& t) noexcept -> bool;
        auto has_knights(const amounts_t& t) noexcept -> bool;
        auto has_druid(const amounts_t& t) noexcept -> bool;
        auto has_riverboat(const amounts_t& t) noexcept -> bool;

    }

    namespace landscapes {
        template<AmountIndex ... E>
        auto enum_index_get_sum(const amounts_t& t) noexcept -> coefficient_t {
            return (... + (std::get<std::to_underlying(E)>(t)));
        }
                
        template<AmountIndex ... E>
        auto enum_index_get_sum_diff(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t {
            return (... + (std::get<std::to_underlying(E)>(max) - std::get<std::to_underlying(E)>(t)));
        }

        auto event_total(const amounts_t& t) noexcept -> coefficient_t;
        auto trait_total(const amounts_t& t) noexcept -> coefficient_t;
        auto other_supply_total(const amounts_t& t) noexcept -> coefficient_t;

        auto has_obelisk(const amounts_t& t) noexcept -> bool;
        auto has_way_of_the_mouse(const amounts_t& t) noexcept -> bool;

    }
}
