#pragma once

#include "typedefs.hpp"

namespace config {
    namespace kingdom {
        template<AmountIndex ... E>
        coefficient_t tuple_enum_get(const kingdom_amounts_t& t){
            return (... + (std::get<std::to_underlying(E)>(t)));
        }

        coefficient_t action_regular_two(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_regular_three(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_regular_four(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_regular_other(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_looter_four(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_looter_other(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_fate_two(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_fate_three(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_fate_four(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_fate_other(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_doom_three(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_doom_four(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_doom_other(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_liaison_two(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_liaison_three(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_liaison_four(const kingdom_amounts_t& t) noexcept;
        coefficient_t action_liaison_other(const kingdom_amounts_t& t) noexcept;
        coefficient_t treasure_regular_two(const kingdom_amounts_t& t) noexcept;
        coefficient_t treasure_regular_three(const kingdom_amounts_t& t) noexcept;
        coefficient_t treasure_regular_four(const kingdom_amounts_t& t) noexcept;
        coefficient_t treasure_regular_other(const kingdom_amounts_t& t) noexcept;
        coefficient_t treasure_fate_other(const kingdom_amounts_t& t) noexcept;
        coefficient_t treasure_liaison_two(const kingdom_amounts_t& t) noexcept;
        coefficient_t treasure_liaison_other(const kingdom_amounts_t& t) noexcept;
        coefficient_t other_regular_two(const kingdom_amounts_t& t) noexcept;
        coefficient_t other_regular_three(const kingdom_amounts_t& t) noexcept;
        coefficient_t other_regular_four(const kingdom_amounts_t& t) noexcept;
        coefficient_t other_regular_other(const kingdom_amounts_t& t) noexcept;
        coefficient_t other_doom_other(const kingdom_amounts_t& t) noexcept;
        coefficient_t knights(const kingdom_amounts_t& t) noexcept;
        coefficient_t ferryman(const kingdom_amounts_t& t) noexcept;
        coefficient_t druid(const kingdom_amounts_t& t) noexcept;
    }

    namespace landscapes {
        template<AmountIndex ... E>
        coefficient_t tuple_enum_get(const landscapes_amounts_t& t){
            return (... + (std::get<std::to_underlying(E)>(t)));
        }

        coefficient_t setup(const landscapes_amounts_t& t) noexcept;
        coefficient_t obelisk(const landscapes_amounts_t& t) noexcept;
        coefficient_t way_of_the_mouse(const landscapes_amounts_t& t) noexcept;
    }

}
