#pragma once

#include "card.cuh"

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

        constexpr State with_added_picker(const Card extra_setup_card_type, const int8_t index) const {
            State result = *this;
            if (extra_setup_card_type == Card::YoungWitch){ // young witch
                result.add_to_supply.young_witch = index;
            } else if (extra_setup_card_type == Card::ApproachingArmy){ // approaching army
                result.add_to_supply.approaching_army = index;
            } else if (extra_setup_card_type == Card::WayOfTheMouse){ // way of the mouse
                result.set_aside.way_of_the_mouse = index;
            } else if (extra_setup_card_type == Card::Ferryman){ // ferryman
                result.set_aside.ferryman = index;
            } else if (extra_setup_card_type == Card::Riverboat){ // riverboat
                result.set_aside.riverboat = index;
            }
            return result;
        }
    };

    static_assert(sizeof(State) == 5);
    static_assert(alignof(State) == 1);
}
