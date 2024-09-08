#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "generator_bounds.hpp"

namespace {
    using testing::Eq;

    using namespace config;

    using kingdom::EditionModifier;
    using kingdom::TrackedType;
    using kingdom::CostGroup;
    using kingdom::ExpansionEditionFilter;

    using landscapes::Type;
    using landscapes::ExpansionFilter;

    constexpr kingdom::table_t kingdom_table{{
        {{Expansion::DOMINION, EditionModifier::NONE}, { TrackedType::ACTION }, CostGroup::TWO, 2},
        {{Expansion::DOMINION, EditionModifier::NONE}, { TrackedType::ACTION }, CostGroup::THREE, 3},
        {{Expansion::DOMINION, EditionModifier::NONE}, { TrackedType::ACTION }, CostGroup::OTHER, 5},
        {{Expansion::DOMINION, EditionModifier::NONE}, { TrackedType::ACTION, TrackedType::LIAISON }, CostGroup::TWO, 7},
        {{Expansion::DOMINION, EditionModifier::NONE}, { TrackedType::ACTION, TrackedType::LOOTER }, CostGroup::THREE, 11},
        {{Expansion::DOMINION, EditionModifier::NONE}, { TrackedType::ACTION, TrackedType::FATE }, CostGroup::OTHER, 13},
        {{Expansion::DOMINION, EditionModifier::NONE}, { TrackedType::TREASURE, }, CostGroup::TWO, 17},
        {{Expansion::DOMINION, EditionModifier::NONE}, { TrackedType::TREASURE, }, CostGroup::THREE, 19},
        {{Expansion::DOMINION, EditionModifier::NONE}, { }, CostGroup::OTHER, 23},
        {{Expansion::DOMINION, EditionModifier::NONE}, { TrackedType::LIAISON }, CostGroup::TWO, 29},
        {{Expansion::DOMINION, EditionModifier::NONE}, { TrackedType::TREASURE, TrackedType::LOOTER }, CostGroup::THREE, 31},
        {{Expansion::DOMINION, EditionModifier::NONE}, { TrackedType::FATE }, CostGroup::OTHER, 37},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, { TrackedType::ACTION }, CostGroup::TWO, 1},
        {{Expansion::DOMINION, EditionModifier::REMOVED}, { TrackedType::ACTION }, CostGroup::THREE, 4},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, { TrackedType::ACTION }, CostGroup::OTHER, 6},
        {{Expansion::DOMINION, EditionModifier::REMOVED}, { TrackedType::ACTION, TrackedType::LIAISON }, CostGroup::TWO, 9},
        {{Expansion::DOMINION, EditionModifier::REMOVED}, { TrackedType::ACTION, TrackedType::LOOTER }, CostGroup::THREE, 10},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, { TrackedType::ACTION, TrackedType::FATE }, CostGroup::OTHER, 12},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, { TrackedType::TREASURE }, CostGroup::TWO, 20},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, { TrackedType::TREASURE }, CostGroup::THREE, 16},
        {{Expansion::DOMINION, EditionModifier::REMOVED}, { }, CostGroup::OTHER, 18},
        {{Expansion::DOMINION, EditionModifier::REMOVED}, { TrackedType::LIAISON }, CostGroup::TWO, 1},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, { TrackedType::TREASURE, TrackedType::LOOTER }, CostGroup::THREE, 22},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, { TrackedType::FATE }, CostGroup::OTHER, 41},
        {{Expansion::ADVENTURES, EditionModifier::NONE}, { TrackedType::ACTION }, CostGroup::TWO, 15},
        {{Expansion::ADVENTURES, EditionModifier::NONE}, { TrackedType::ACTION }, CostGroup::THREE, 8},
        {{Expansion::ADVENTURES, EditionModifier::NONE}, { TrackedType::ACTION }, CostGroup::OTHER, 5},
        {{Expansion::ADVENTURES, EditionModifier::NONE}, { TrackedType::ACTION, TrackedType::LIAISON }, CostGroup::TWO, 7},
        {{Expansion::ADVENTURES, EditionModifier::NONE}, { TrackedType::ACTION, TrackedType::LOOTER }, CostGroup::THREE, 11},
        {{Expansion::INTRIGUE, EditionModifier::NONE}, { TrackedType::ACTION, TrackedType::FATE }, CostGroup::OTHER, 13},
        {{Expansion::INTRIGUE, EditionModifier::NONE}, { TrackedType::TREASURE }, CostGroup::TWO, 17},
        {{Expansion::GUILDS, EditionModifier::NONE}, { TrackedType::TREASURE }, CostGroup::THREE, 19},
        {{Expansion::EMPIRES, EditionModifier::NONE}, { }, CostGroup::OTHER, 23},
        {{Expansion::GUILDS, EditionModifier::NONE}, { TrackedType::LIAISON }, CostGroup::TWO, 29},
        {{Expansion::INTRIGUE, EditionModifier::NONE}, { TrackedType::TREASURE, TrackedType::LOOTER }, CostGroup::THREE, 31},
        {{Expansion::DARK_AGES, EditionModifier::NONE}, { TrackedType::FATE }, CostGroup::OTHER, 37},
        {{Expansion::RENAISSANCE, EditionModifier::UPDATE_PACK}, { TrackedType::ACTION }, CostGroup::TWO, 2},
        {{Expansion::INTRIGUE, EditionModifier::REMOVED}, { TrackedType::ACTION }, CostGroup::THREE, 3},
        {{Expansion::GUILDS, EditionModifier::UPDATE_PACK}, { TrackedType::ACTION }, CostGroup::OTHER, 5},
        {{Expansion::EMPIRES, EditionModifier::REMOVED}, { TrackedType::ACTION, TrackedType::LIAISON }, CostGroup::TWO, 7},
        {{Expansion::INTRIGUE, EditionModifier::REMOVED}, { TrackedType::ACTION, TrackedType::LOOTER }, CostGroup::THREE, 11},
        {{Expansion::RENAISSANCE, EditionModifier::UPDATE_PACK}, { TrackedType::ACTION, TrackedType::FATE }, CostGroup::OTHER, 13},
        {{Expansion::GUILDS, EditionModifier::UPDATE_PACK}, { TrackedType::TREASURE }, CostGroup::TWO, 17},
        {{Expansion::EMPIRES, EditionModifier::UPDATE_PACK}, { TrackedType::TREASURE }, CostGroup::THREE, 19},
        {{Expansion::DARK_AGES, EditionModifier::REMOVED}, { }, CostGroup::OTHER, 23},
        {{Expansion::EMPIRES, EditionModifier::REMOVED}, { TrackedType::LIAISON }, CostGroup::TWO, 29},
        {{Expansion::INTRIGUE, EditionModifier::UPDATE_PACK}, { TrackedType::TREASURE, TrackedType::LOOTER }, CostGroup::THREE, 31},
        {{Expansion::DARK_AGES, EditionModifier::UPDATE_PACK}, { TrackedType::FATE }, CostGroup::OTHER, 37},
    }};

    TEST(kingdom_table, all){
        const auto all_expansion_editions = kingdom::ExpansionEditionFilter{}.set();
        const auto no_types_cost_other = kingdom::TableQuery{{}, CostGroup::OTHER};
        ASSERT_THAT(filter_kingdom_table(kingdom_table, all_expansion_editions, no_types_cost_other), Eq(87u));
    }
//
//    TEST(kingdom_table, single_selector){
////        kingdom::TableQuery all{};
//        ExpansionEditionFilter expansion_edition_filter = {{Expansion::DOMINION, EditionModifier::NONE}, {Expansion::DOMINION, EditionModifier::UPDATE_PACK} };
//        ASSERT_THAT(filter_kingdom_table(kingdom_table, expansion_edition_filter, {}), Eq(315u));
//
//        kingdom::TableQuery primary_type_selector{};
//        primary_type_selector.primary_type_ = {PrimaryType::ACTION};
//        ASSERT_THAT(filter_kingdom_table(kingdom_table, {}, primary_type_selector), Eq(183u));
//
//        kingdom::TableQuery secondary_type_selector{};
//        secondary_type_selector.secondary_type_ = {SecondaryType::REGULAR, SecondaryType::FATE, SecondaryType::DOOM};
//        ASSERT_THAT(filter_kingdom_table(kingdom_table, {}, secondary_type_selector), Eq(493u));
//
//        kingdom::TableQuery cost_selector_{};
//        cost_selector_.cost_ = {CostGroup::TWO, CostGroup::THREE, CostGroup::DEBT};
//        ASSERT_THAT(filter_kingdom_table(kingdom_table, {}, cost_selector_), Eq(458u));
//    }
//
//    TEST(kingdom_table, multi_selector) {
//        {
//            ExpansionEditionFilter expansion_edition_filter = {{Expansion::DOMINION, EditionModifier::NONE},
//                                                               {Expansion::DOMINION, EditionModifier::UPDATE_PACK},
//                                                               {Expansion::GUILDS,   EditionModifier::NONE}};
//            kingdom::TableQuery query{};
//            query.cost_ = {CostGroup::TWO};
//            ASSERT_THAT(filter_kingdom_table(kingdom_table, expansion_edition_filter, query), Eq(105u));
//        }
//        {
//            ExpansionEditionFilter expansion_edition_filter = {{Expansion::ADVENTURES, EditionModifier::NONE},
//                                                               {Expansion::DARK_AGES,  EditionModifier::UPDATE_PACK}};
//            kingdom::TableQuery query{};
//            query.primary_type_ = {PrimaryType::ACTION, PrimaryType::OTHER};
//            query.secondary_type_ = {SecondaryType::REGULAR, SecondaryType::LIAISON};
//            query.cost_ = {CostGroup::TWO};
//            ASSERT_THAT(filter_kingdom_table(kingdom_table, expansion_edition_filter, query), Eq(22u));
//        }
//    }
//
//    constexpr landscapes::table_t landscapes_table{{
//        {Expansion::DOMINION, Type::ALLY, 2},
//        {Expansion::DOMINION, Type::WAY, 3},
//        {Expansion::DOMINION, Type::ARTIFACT, 5},
//        {Expansion::MENAGERIE, Type::ARTIFACT, 6},
//        {Expansion::MENAGERIE, Type::PROJECT, 15},
//        {Expansion::MENAGERIE, Type::EVENT, 8},
//        {Expansion::SEASIDE, Type::BOON, 7},
//    }};
//
//    TEST(landscapes_table, all){
//        ASSERT_THAT(filter_landscapes_table(landscapes_table, {}, {}), Eq(46u));
//    }
//
//    TEST(landscapes_table, single_selector){
//        {
//            ExpansionFilter expansion_filter = {Expansion::DOMINION, Expansion::RENAISSANCE, Expansion::NOCTURNE,
//                                                Expansion::SEASIDE};
//            ASSERT_THAT(filter_landscapes_table(landscapes_table, expansion_filter, {}), Eq(17u));
//        }
//        {
//            landscapes::TableQuery query{};
//            query.type_ = {Type::HEX, Type::BOON, Type::ARTIFACT};
//            ASSERT_THAT(filter_landscapes_table(landscapes_table, {}, query), Eq(18U));
//        }
//    }
//
//    TEST(landscapes_table, multi_selector){
//        ExpansionFilter expansion_filter = {Expansion::DOMINION, Expansion::RENAISSANCE, Expansion::NOCTURNE, Expansion::SEASIDE};
//        landscapes::TableQuery all_selector{};
//        all_selector.type_ = {Type::ALLY, Type::ARTIFACT};
//        ASSERT_THAT(filter_landscapes_table(landscapes_table, expansion_filter, all_selector), Eq(7u));
//    }
}