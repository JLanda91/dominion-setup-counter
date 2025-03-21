#pragma once

#include "card_type.hpp"

namespace card_data::extra_setup {

    struct State{
        struct AddToSupply {
            int8_t young_witch = -1;
            int8_t approaching_army = -1;
        } add_to_supply ;
        
        struct SetAside {
            int8_t way_of_the_mouse = -1;
            int8_t ferryman = -1;
            int8_t riverboat = -1;
        } set_aside;

        constexpr uint8_t num_unused_added_from(const int8_t index) const {
            return
                (add_to_supply.young_witch == index) +
                (add_to_supply.approaching_army == index) +
                (set_aside.way_of_the_mouse == index) +
                (set_aside.ferryman == index) +
                (set_aside.riverboat == index);
        }

        constexpr uint8_t num_used_added_at(const int8_t index) const {
            return
                (add_to_supply.young_witch == index) +
                (add_to_supply.approaching_army == index);
        }

        constexpr State with_added_picker(const CardType extra_setup_card_type, const int8_t index) const {
            State result = *this;
            if (extra_setup_card_type == CardType::YoungWitch){ // young witch
                result.add_to_supply.young_witch = index;
            } else if (extra_setup_card_type == CardType::ApproachingArmy){ // approaching army
                result.add_to_supply.approaching_army = index;
            } else if (extra_setup_card_type == CardType::WayOfTheMouse){ // way of the mouse
                result.set_aside.way_of_the_mouse = index;
            } else if (extra_setup_card_type == CardType::Ferryman){ // ferryman
                result.set_aside.ferryman = index;
            } else if (extra_setup_card_type == CardType::Riverboat){ // riverboat
                result.set_aside.riverboat = index;
            }
            return result;
        }
    };

    static_assert(sizeof(State) == 5);
    static_assert(alignof(State) == 1);
}
