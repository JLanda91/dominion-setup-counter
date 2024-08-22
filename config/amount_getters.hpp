
#pragma once

#include "typedefs.hpp"

namespace config::kingdom {

    template<AmountIndex ... E>
    auto sum_tuple_on_enum_index(const kingdom_amounts_t& t) noexcept -> coefficient_t {
        return (... + (std::get<std::to_underlying(E)>(t)));
    }

    template<AmountIndex ... E>
    auto diff_tuple_on_enum_index(const kingdom_amounts_t& t, const kingdom_amounts_t& max) noexcept -> coefficient_t {
        return (... + (std::get<std::to_underlying(E)>(max) - std::get<std::to_underlying(E)>(t)));
    }

    auto looter_total(const kingdom_amounts_t& t) noexcept -> coefficient_t;
    auto fate_total(const kingdom_amounts_t& t) noexcept -> coefficient_t;
    auto doom_total(const kingdom_amounts_t& t) noexcept -> coefficient_t;
    auto liaison_total(const kingdom_amounts_t& t) noexcept -> coefficient_t;
    auto loot_total(const kingdom_amounts_t& t) noexcept -> coefficient_t;
    auto two_three_total(const kingdom_amounts_t& t) noexcept -> coefficient_t;
    auto three_four_unused(const kingdom_amounts_t& t, const kingdom_amounts_t& max) noexcept -> coefficient_t;
    auto action_nonduration_five_unused(const kingdom_amounts_t& t, const kingdom_amounts_t& max) noexcept -> coefficient_t;
    auto action_total(const kingdom_amounts_t& t) noexcept -> coefficient_t;
    auto action_two_three_unused(const kingdom_amounts_t& t, const kingdom_amounts_t& max) noexcept -> coefficient_t;
    auto attack_unused(const kingdom_amounts_t& t, const kingdom_amounts_t& max) noexcept -> coefficient_t;
}
