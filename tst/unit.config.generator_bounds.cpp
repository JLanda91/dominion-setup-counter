#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "generator_bounds.hpp"

namespace {
    using testing::Eq;

    using namespace config;

    using kingdom::EditionModifier;
    using kingdom::PrimaryType;
    using kingdom::SecondaryType;
    using kingdom::Cost;
    using kingdom::ExpansionEditionFilter;

    using landscapes::Type;
    using landscapes::ExpansionFilter;

    constexpr kingdom::table_t kingdom_table{{
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 2},
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 3},
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 5},
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::LIAISON, Cost::TWO, 7},
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::LOOTER, Cost::THREE, 11},
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::FATE, Cost::SIX, 13},
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::TWO, 17},
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::THREE, 19},
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::SIX, 23},
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::OTHER, SecondaryType::LIAISON, Cost::TWO, 29},
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::TREASURE, SecondaryType::LOOTER, Cost::THREE, 31},
        {{Expansion::DOMINION, EditionModifier::NONE}, PrimaryType::OTHER, SecondaryType::FATE, Cost::SIX, 37},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 1},
        {{Expansion::DOMINION, EditionModifier::REMOVED}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 4},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 6},
        {{Expansion::DOMINION, EditionModifier::REMOVED}, PrimaryType::ACTION, SecondaryType::LIAISON, Cost::TWO, 9},
        {{Expansion::DOMINION, EditionModifier::REMOVED}, PrimaryType::ACTION, SecondaryType::LOOTER, Cost::THREE, 10},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, PrimaryType::ACTION, SecondaryType::FATE, Cost::SIX, 12},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::TWO, 20},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::THREE, 16},
        {{Expansion::DOMINION, EditionModifier::REMOVED}, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::SIX, 18},
        {{Expansion::DOMINION, EditionModifier::REMOVED}, PrimaryType::OTHER, SecondaryType::LIAISON, Cost::TWO, 1},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, PrimaryType::TREASURE, SecondaryType::LOOTER, Cost::THREE, 22},
        {{Expansion::DOMINION, EditionModifier::UPDATE_PACK}, PrimaryType::OTHER, SecondaryType::FATE, Cost::SIX, 41},
        {{Expansion::ADVENTURES, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 15},
        {{Expansion::ADVENTURES, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 8},
        {{Expansion::ADVENTURES, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 5},
        {{Expansion::ADVENTURES, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::LIAISON, Cost::TWO, 7},
        {{Expansion::ADVENTURES, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::LOOTER, Cost::THREE, 11},
        {{Expansion::INTRIGUE, EditionModifier::NONE}, PrimaryType::ACTION, SecondaryType::FATE, Cost::SIX, 13},
        {{Expansion::INTRIGUE, EditionModifier::NONE}, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::TWO, 17},
        {{Expansion::GUILDS, EditionModifier::NONE}, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::THREE, 19},
        {{Expansion::EMPIRES, EditionModifier::NONE}, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::SIX, 23},
        {{Expansion::GUILDS, EditionModifier::NONE}, PrimaryType::OTHER, SecondaryType::LIAISON, Cost::TWO, 29},
        {{Expansion::INTRIGUE, EditionModifier::NONE}, PrimaryType::TREASURE, SecondaryType::LOOTER, Cost::THREE, 31},
        {{Expansion::DARK_AGES, EditionModifier::NONE}, PrimaryType::OTHER, SecondaryType::FATE, Cost::SIX, 37},
        {{Expansion::RENAISSANCE, EditionModifier::UPDATE_PACK}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 2},
        {{Expansion::INTRIGUE, EditionModifier::REMOVED}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 3},
        {{Expansion::GUILDS, EditionModifier::UPDATE_PACK}, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 5},
        {{Expansion::EMPIRES, EditionModifier::REMOVED}, PrimaryType::ACTION, SecondaryType::LIAISON, Cost::TWO, 7},
        {{Expansion::INTRIGUE, EditionModifier::REMOVED}, PrimaryType::ACTION, SecondaryType::LOOTER, Cost::THREE, 11},
        {{Expansion::RENAISSANCE, EditionModifier::UPDATE_PACK}, PrimaryType::ACTION, SecondaryType::FATE, Cost::SIX, 13},
        {{Expansion::GUILDS, EditionModifier::UPDATE_PACK}, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::TWO, 17},
        {{Expansion::EMPIRES, EditionModifier::UPDATE_PACK}, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::THREE, 19},
        {{Expansion::DARK_AGES, EditionModifier::REMOVED}, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::SIX, 23},
        {{Expansion::EMPIRES, EditionModifier::REMOVED}, PrimaryType::OTHER, SecondaryType::LIAISON, Cost::TWO, 29},
        {{Expansion::INTRIGUE, EditionModifier::UPDATE_PACK}, PrimaryType::TREASURE, SecondaryType::LOOTER, Cost::THREE, 31},
        {{Expansion::DARK_AGES, EditionModifier::UPDATE_PACK}, PrimaryType::OTHER, SecondaryType::FATE, Cost::SIX, 37},
    }};

    TEST(kingdom_table, all){
        ASSERT_THAT(filter_kingdom_table(kingdom_table, {}, {}), Eq(769u));
    }

    TEST(kingdom_table, single_selector){
//        kingdom::TableQuery all{};
        ExpansionEditionFilter expansion_edition_filter = {{Expansion::DOMINION, EditionModifier::NONE}, {Expansion::DOMINION, EditionModifier::UPDATE_PACK} };
        ASSERT_THAT(filter_kingdom_table(kingdom_table, expansion_edition_filter, {}), Eq(315u));

        kingdom::TableQuery primary_type_selector{};
        primary_type_selector.primary_type_ = {PrimaryType::ACTION};
        ASSERT_THAT(filter_kingdom_table(kingdom_table, {}, primary_type_selector), Eq(183u));

        kingdom::TableQuery secondary_type_selector{};
        secondary_type_selector.secondary_type_ = {SecondaryType::REGULAR, SecondaryType::FATE, SecondaryType::DOOM};
        ASSERT_THAT(filter_kingdom_table(kingdom_table, {}, secondary_type_selector), Eq(493u));

        kingdom::TableQuery cost_selector_{};
        cost_selector_.cost_ = {Cost::TWO, Cost::THREE, Cost::DEBT};
        ASSERT_THAT(filter_kingdom_table(kingdom_table, {}, cost_selector_), Eq(458u));
    }

    TEST(kingdom_table, multi_selector) {
        {
            ExpansionEditionFilter expansion_edition_filter = {{Expansion::DOMINION, EditionModifier::NONE},
                                                               {Expansion::DOMINION, EditionModifier::UPDATE_PACK},
                                                               {Expansion::GUILDS,   EditionModifier::NONE}};
            kingdom::TableQuery query{};
            query.cost_ = {Cost::TWO};
            ASSERT_THAT(filter_kingdom_table(kingdom_table, expansion_edition_filter, query), Eq(105u));
        }
        {
            ExpansionEditionFilter expansion_edition_filter = {{Expansion::ADVENTURES, EditionModifier::NONE},
                                                               {Expansion::DARK_AGES,  EditionModifier::UPDATE_PACK}};
            kingdom::TableQuery query{};
            query.primary_type_ = {PrimaryType::ACTION, PrimaryType::OTHER};
            query.secondary_type_ = {SecondaryType::REGULAR, SecondaryType::LIAISON};
            query.cost_ = {Cost::TWO};
            ASSERT_THAT(filter_kingdom_table(kingdom_table, expansion_edition_filter, query), Eq(22u));
        }
    }

    constexpr landscapes::table_t landscapes_table{{
        {Expansion::DOMINION, Type::ALLY, 2},
        {Expansion::DOMINION, Type::WAY, 3},
        {Expansion::DOMINION, Type::ARTIFACT, 5},
        {Expansion::MENAGERIE, Type::ARTIFACT, 6},
        {Expansion::MENAGERIE, Type::PROJECT, 15},
        {Expansion::MENAGERIE, Type::EVENT, 8},
        {Expansion::SEASIDE, Type::BOON, 7},
    }};

    TEST(landscapes_table, all){
        ASSERT_THAT(filter_landscapes_table(landscapes_table, {}, {}), Eq(46u));
    }

    TEST(landscapes_table, single_selector){
        {
            ExpansionFilter expansion_filter = {Expansion::DOMINION, Expansion::RENAISSANCE, Expansion::NOCTURNE,
                                                Expansion::SEASIDE};
            ASSERT_THAT(filter_landscapes_table(landscapes_table, expansion_filter, {}), Eq(17u));
        }
        {
            landscapes::TableQuery query{};
            query.type_ = {Type::HEX, Type::BOON, Type::ARTIFACT};
            ASSERT_THAT(filter_landscapes_table(landscapes_table, {}, query), Eq(18U));
        }
    }

    TEST(landscapes_table, multi_selector){
        ExpansionFilter expansion_filter = {Expansion::DOMINION, Expansion::RENAISSANCE, Expansion::NOCTURNE, Expansion::SEASIDE};
        landscapes::TableQuery all_selector{};
        all_selector.type_ = {Type::ALLY, Type::ARTIFACT};
        ASSERT_THAT(filter_landscapes_table(landscapes_table, expansion_filter, all_selector), Eq(7u));
    }
}