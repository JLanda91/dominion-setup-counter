#pragma once

#include "card_type.hpp"

namespace card_data::extra_setup {

    struct State{
        struct TableCoordinate{
            std::size_t row;
            card_data::kingdom::CardType column;
        };
        
        struct AddToSupply {
            std::optional<TableCoordinate> young_witch = std::nullopt;
            std::optional<TableCoordinate> approaching_army = std::nullopt;
        } add_to_supply ;
        
        struct SetAside {
            std::optional<TableCoordinate> way_of_the_mouse = std::nullopt;
            std::optional<TableCoordinate> ferryman = std::nullopt;
            std::optional<TableCoordinate> riverboat = std::nullopt;
        } set_aside;
        
        struct FromSupply {
            std::optional<TableCoordinate> obelisk = std::nullopt;
        } from_supply;

        uint8_t num_added_to_supply() const {
            return 
                (uint8_t)(add_to_supply.young_witch.has_value()) +
                (uint8_t)(add_to_supply.approaching_army.has_value());
        }

        uint8_t num_already_picked_at(uint8_t row, card_data::kingdom::CardType col) const {
            return
                (uint8_t)(add_to_supply.young_witch.has_value() && add_to_supply.young_witch->row == row && add_to_supply.young_witch->column == col) +
                (uint8_t)(add_to_supply.approaching_army.has_value() && add_to_supply.approaching_army->row == row && add_to_supply.approaching_army->column == col) +
                (uint8_t)(set_aside.way_of_the_mouse.has_value() && set_aside.way_of_the_mouse->row == row && set_aside.way_of_the_mouse->column == col) +
                (uint8_t)(set_aside.ferryman.has_value() && set_aside.ferryman->row == row && set_aside.ferryman->column == col) +
                (uint8_t)(set_aside.riverboat.has_value() && set_aside.riverboat->row == row && set_aside.riverboat->column == col) ;
        }
        
        template<CardType P>
        constexpr State with_added_picker(std::size_t i, card_data::kingdom::CardType c) const {
            State result = *this;
            if constexpr (P == CardType::YoungWitch){ // young witch
                result.add_to_supply.young_witch.emplace(i, c);
            } else if constexpr (P == CardType::ApproachingArmy){ // approaching army
                result.add_to_supply.approaching_army.emplace(i, c);
            } else if constexpr (P == CardType::WayOfTheMouse){ // way of the mouse
                result.set_aside.way_of_the_mouse.emplace(i, c);
            } else if constexpr (P == CardType::Ferryman){ // ferryman
                result.set_aside.ferryman.emplace(i, c);
            } else if constexpr (P == CardType::Riverboat){ // riverboat
                result.set_aside.riverboat.emplace(i, c);
            } else if constexpr (P == CardType::Obelisk){ // obelisk
                result.from_supply.obelisk.emplace(i, c);
            }
            return result;
        }
    };
}
