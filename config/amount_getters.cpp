#include "amount_getters.hpp"

namespace config{
    namespace kingdom {
        coefficient_t action_regular_two(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_REGULAR_TWO>(t);
        }

        coefficient_t action_regular_three(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_REGULAR_THREE>(t);
        }

        coefficient_t action_regular_four(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_REGULAR_FOUR>(t);
        }

        coefficient_t action_regular_other(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_REGULAR_OTHER>(t);
        }

        coefficient_t action_looter_four(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_LOOTER_FOUR>(t);
        }

        coefficient_t action_looter_other(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_LOOTER_OTHER>(t);
        }

        coefficient_t action_fate_two(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_FATE_TWO>(t);
        }

        coefficient_t action_fate_three(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_FATE_THREE>(t);
        }

        coefficient_t action_fate_four(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_FATE_FOUR>(t);
        }

        coefficient_t action_fate_other(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_FATE_OTHER>(t);
        }

        coefficient_t action_doom_three(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_DOOM_THREE>(t);
        }

        coefficient_t action_doom_four(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_DOOM_FOUR>(t);
        }

        coefficient_t action_doom_other(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_DOOM_OTHER>(t);
        }

        coefficient_t action_liaison_two(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_LIAISON_TWO>(t);
        }

        coefficient_t action_liaison_three(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_LIAISON_THREE>(t);
        }

        coefficient_t action_liaison_four(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_LIAISON_FOUR>(t);
        }

        coefficient_t action_liaison_other(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::ACTION_LIAISON_OTHER>(t);
        }

        coefficient_t treasure_regular_two(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::TREASURE_REGULAR_TWO>(t);
        }

        coefficient_t treasure_regular_three(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::TREASURE_REGULAR_THREE>(t);
        }

        coefficient_t treasure_regular_four(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::TREASURE_REGULAR_FOUR>(t);
        }

        coefficient_t treasure_regular_other(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::TREASURE_REGULAR_OTHER>(t);
        }

        coefficient_t treasure_fate_other(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::TREASURE_FATE_OTHER>(t);
        }

        coefficient_t treasure_liaison_two(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::TREASURE_LIAISON_TWO>(t);
        }

        coefficient_t treasure_liaison_other(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::TREASURE_LIAISON_OTHER>(t);
        }

        coefficient_t other_regular_two(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::OTHER_REGULAR_TWO>(t);
        }

        coefficient_t other_regular_three(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::OTHER_REGULAR_THREE>(t);
        }

        coefficient_t other_regular_four(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::OTHER_REGULAR_FOUR>(t);
        }

        coefficient_t other_regular_other(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::OTHER_REGULAR_OTHER>(t);
        }

        coefficient_t other_doom_other(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::OTHER_DOOM_OTHER>(t);
        }

        coefficient_t knights(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::KNIGHTS>(t);
        }

        coefficient_t ferryman(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::FERRYMAN>(t);
        }

        coefficient_t druid(const kingdom_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::DRUID>(t);
        }

    }

    namespace landscapes {
        coefficient_t setup(const landscapes_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::SETUP>(t);
        }

        coefficient_t obelisk(const landscapes_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::OBELISK>(t);
        }

        coefficient_t way_of_the_mouse(const landscapes_amounts_t& t) noexcept{
            return tuple_enum_get<AmountIndex::WAY_OF_THE_MOUSE>(t);
        }

    }

}
