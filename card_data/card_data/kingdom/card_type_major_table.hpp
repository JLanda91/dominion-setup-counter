// GENERATED CODE - DO NOT MODIFY

#pragma once

#include <utils/column_major_table.hpp>
#include <utils/filtered_index_sequence.hpp>

#include "card_type.hpp"
#include "membership_mask.hpp"

namespace card_data::kingdom {
    inline constexpr std::size_t kNumVennRegions = 20uz;
    using card_type_major_table_t = utils::ColumnMajorTable<MembershipMask, CardType, kNumVennRegions, kNumCardTypes>;

    inline constexpr auto card_type_major_table() -> const card_type_major_table_t& {
        static constexpr card_type_major_table_t singleton = []{
            return card_type_major_table_t{
                card_type_major_table_t::row_label_t { MembershipMask::FromUnsigned(0u),MembershipMask::FromUnsigned(1u),MembershipMask::FromUnsigned(2u),MembershipMask::FromUnsigned(8u),MembershipMask::FromUnsigned(9u),MembershipMask::FromUnsigned(64u),MembershipMask::FromUnsigned(65u),MembershipMask::FromUnsigned(66u),MembershipMask::FromUnsigned(72u),MembershipMask::FromUnsigned(73u),MembershipMask::FromUnsigned(96u),MembershipMask::FromUnsigned(98u),MembershipMask::FromUnsigned(101u),MembershipMask::FromUnsigned(103u),MembershipMask::FromUnsigned(104u),MembershipMask::FromUnsigned(106u),MembershipMask::FromUnsigned(109u),MembershipMask::FromUnsigned(111u),MembershipMask::FromUnsigned(112u),MembershipMask::FromUnsigned(114u) },
                card_type_major_table_t::column_label_t { CardType::YoungWitch, CardType::Knights, CardType::Druid, CardType::Ferryman, CardType::Riverboat, CardType::Looter, CardType::Fate, CardType::Doom, CardType::Liaison, CardType::Omen, CardType::Loot, CardType::None },
                card_type_major_table_t::data_t { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,2,0,2,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,2,0,0,0,0,0,1,1,0,0,0,0,0,1,0,1,0,3,0,2,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,3,0,0,0,1,1,0,0,0,0,0,2,1,0,0,0,0,1,1,0,0,0,0,0,1,0,8,2,1,6,5,26,5,2,9,5,43,12,37,2,93,13,65,8,88,26 }
            };
        }();
        return singleton;
    }

    template<template<std::size_t> class P>
    using card_table_row_index_sequence_t = utils::filtered_index_sequence_t<kNumVennRegions, P>;

    using card_table_column_index_sequence_t = std::make_index_sequence<kNumCardTypes>;

    template<std::size_t I> using IsYoungWitchRow = std::bool_constant<card_type_major_table().row_labels().at(I).is_young_witch_region>;
    template<std::size_t I> using IsApproachingArmyRow = std::bool_constant<card_type_major_table().row_labels().at(I).is_approaching_army_region>;
    template<std::size_t I> using IsWayOfTheMouseRow = std::bool_constant<card_type_major_table().row_labels().at(I).is_way_of_the_mouse_region>;
    template<std::size_t I> using IsFerrymanRow = std::bool_constant<card_type_major_table().row_labels().at(I).is_ferryman_region>;
    template<std::size_t I> using IsRiverboatRow = std::bool_constant<card_type_major_table().row_labels().at(I).is_riverboat_region>;
    template<std::size_t I> using IsObeliskRow = std::bool_constant<card_type_major_table().row_labels().at(I).is_obelisk_region>;
    template<std::size_t I> using IsActionOrTeasureRow = std::bool_constant<card_type_major_table().row_labels().at(I).is_action_or_treasure_region>;

    using row_index_sequence_young_witch_t = card_table_row_index_sequence_t<IsYoungWitchRow>;
    using row_index_sequence_approaching_army_t = card_table_row_index_sequence_t<IsApproachingArmyRow>;
    using row_index_sequence_way_of_the_mouse_t = card_table_row_index_sequence_t<IsWayOfTheMouseRow>;
    using row_index_sequence_ferryman_t = card_table_row_index_sequence_t<IsFerrymanRow>;
    using row_index_sequence_riverboat_t = card_table_row_index_sequence_t<IsRiverboatRow>;
    using row_index_sequence_obelisk_t = card_table_row_index_sequence_t<IsObeliskRow>;
    using row_index_sequence_action_or_treasure_t = card_table_row_index_sequence_t<IsActionOrTeasureRow>;
}

