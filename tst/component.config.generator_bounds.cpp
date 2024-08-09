#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "generator_bounds.hpp"
#include "default_expansion_editions.hpp"

namespace {
    using config::coefficient_t;
    using config::GeneratorBounds;
    using config::kAllExpansionEditions;
    using testing::FieldsAre;

    TEST(generator_bounds, all_expansion_edition_bounds){
        const auto actual = from_expansions_editions(kAllExpansionEditions);
        ASSERT_THAT(actual.kingdom_, FieldsAre(
            (coefficient_t)32,     // 00 action_regular_two
            (coefficient_t)58,     // 01 action_regular_three
            (coefficient_t)83,     // 02 action_regular_four
            (coefficient_t)142,    // 03 action_regular_other
            (coefficient_t)2,      // 04 action_looter_four
            (coefficient_t)1,      // 05 action_looter_other
            (coefficient_t)2,      // 06 action_fate_two
            (coefficient_t)1,      // 07 action_fate_three
            (coefficient_t)2,      // 08 action_fate_four
            (coefficient_t)1,      // 09 action_fate_other
            (coefficient_t)1,      // 10 action_doom_three
            (coefficient_t)1,      // 11 action_doom_four
            (coefficient_t)3,      // 12 action_doom_other
            (coefficient_t)1,      // 13 action_liaison_two
            (coefficient_t)3,      // 14 action_liaison_three
            (coefficient_t)1,      // 15 action_liaison_four
            (coefficient_t)2,      // 16 action_liaison_other
            (coefficient_t)5,      // 17 treasure_regular_two
            (coefficient_t)7,      // 18 treasure_regular_three
            (coefficient_t)9,      // 19 treasure_regular_four
            (coefficient_t)37,     // 20 treasure_regular_other
            (coefficient_t)1,      // 21 treasure_fate_other
            (coefficient_t)1,      // 22 treasure_liaison_two
            (coefficient_t)1,      // 23 treasure_liaison_other
            (coefficient_t)2,      // 24 other_regular_two
            (coefficient_t)6,      // 25 other_regular_three
            (coefficient_t)7,      // 26 other_regular_four
            (coefficient_t)12,     // 27 other_regular_other
            (coefficient_t)1,      // 28 other_doom_other
            
            (coefficient_t)1,      // 29 knights
            (coefficient_t)1,      // 30 ferryman
            (coefficient_t)1       // 31 druid
        ));

        ASSERT_THAT(actual.landscapes_, FieldsAre(
            (coefficient_t)113,     // 00 setup

            (coefficient_t)1,       // obelisk
            (coefficient_t)1        // way of the mouse
        ));
    }

    TEST(generator_bounds, knights_ferryman_obelisk){
        using config::Expansion;
        using config::kingdom::EditionModifier;
        using config::kingdom::ExpansionEditionFilter;

        ExpansionEditionFilter filter{{
            { Expansion::DARK_AGES, EditionModifier::NONE },    // KNIGHTS
            { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK },    // FERRYMAN
            { Expansion::EMPIRES, EditionModifier::NONE } // obelisk
        }};

        const auto actual = from_expansions_editions(filter);
        ASSERT_THAT(actual.kingdom_, FieldsAre(
            (coefficient_t)7,     // 00 action_regular_two
            (coefficient_t)13,     // 01 action_regular_three
            (coefficient_t)11,     // 02 action_regular_four
            (coefficient_t)26,    // 03 action_regular_other
            (coefficient_t)2,      // 04 action_looter_four
            (coefficient_t)1,      // 05 action_looter_other
            (coefficient_t)0,      // 06 action_fate_two
            (coefficient_t)0,      // 07 action_fate_three
            (coefficient_t)0,      // 08 action_fate_four
            (coefficient_t)0,      // 09 action_fate_other
            (coefficient_t)0,      // 10 action_doom_three
            (coefficient_t)0,      // 11 action_doom_four
            (coefficient_t)0,      // 12 action_doom_other
            (coefficient_t)0,      // 13 action_liaison_two
            (coefficient_t)0,      // 14 action_liaison_three
            (coefficient_t)0,      // 15 action_liaison_four
            (coefficient_t)0,      // 16 action_liaison_other
            (coefficient_t)0,      // 17 treasure_regular_two
            (coefficient_t)0,      // 18 treasure_regular_three
            (coefficient_t)0,      // 19 treasure_regular_four
            (coefficient_t)3,     // 20 treasure_regular_other
            (coefficient_t)0,      // 21 treasure_fate_other
            (coefficient_t)0,      // 22 treasure_liaison_two
            (coefficient_t)0,      // 23 treasure_liaison_other
            (coefficient_t)0,      // 24 other_regular_two
            (coefficient_t)1,      // 25 other_regular_three
            (coefficient_t)1,      // 26 other_regular_four
            (coefficient_t)0,     // 27 other_regular_other
            (coefficient_t)0,      // 28 other_doom_other

            (coefficient_t)1,      // 29 knights
            (coefficient_t)1,      // 30 ferryman
            (coefficient_t)0       // 31 druid
        ));

        ASSERT_THAT(actual.landscapes_, FieldsAre(
            (coefficient_t)33,     // 00 setup

            (coefficient_t)1,       // obelisk
            (coefficient_t)0        // way of the mouse
        ));
    }
}