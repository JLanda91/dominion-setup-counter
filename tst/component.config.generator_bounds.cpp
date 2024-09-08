#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "generator_bounds.hpp"
#include "default_expansion_editions.hpp"

namespace {
    using config::coefficient_t;
    using config::GeneratorBounds;
    using config::Expansion;
    using config::kAllExpansionEditions;
    using config::kingdom::ExpansionEditionFilter;
    using config::kingdom::EditionModifier;

    using testing::FieldsAre;

    TEST(generator_bounds, all_expansion_editions){
        const auto& expansion_edition_filter = kAllExpansionEditions;

        const auto actual = from_expansions_editions(expansion_edition_filter);
        ASSERT_THAT(actual.kingdom_, FieldsAre(
            (coefficient_t)1,     // NONE_TWO
            (coefficient_t)4,     // NONE_THREE
            (coefficient_t)6,     // NONE_FOUR
            (coefficient_t)2,     // NONE_FIVE
            (coefficient_t)3,     // NONE_OTHER
            (coefficient_t)34,     // ACTION_TWO
            (coefficient_t)53,     // ACTION_THREE
            (coefficient_t)84,     // ACTION_FOUR
            (coefficient_t)87,     // ACTION_FIVE
            (coefficient_t)28,     // ACTION_OTHER
            (coefficient_t)2,     // ACTION_ATTACK_TWO
            (coefficient_t)6,     // ACTION_ATTACK_THREE
            (coefficient_t)12,     // ACTION_ATTACK_FOUR
            (coefficient_t)26,     // ACTION_ATTACK_FIVE
            (coefficient_t)3,     // ACTION_ATTACK_OTHER
            (coefficient_t)1,     // ACTION_ATTACK_DOOM_FOUR
            (coefficient_t)2,     // ACTION_ATTACK_DOOM_FIVE
            (coefficient_t)2,     // ACTION_ATTACK_DURATION_THREE
            (coefficient_t)1,     // ACTION_ATTACK_DURATION_FOUR
            (coefficient_t)8,     // ACTION_ATTACK_DURATION_FIVE
            (coefficient_t)1,     // ACTION_ATTACK_DURATION_OTHER
            (coefficient_t)1,     // ACTION_ATTACK_DURATION_LOOT_FIVE
            (coefficient_t)1,     // ACTION_ATTACK_LOOTER_FOUR
            (coefficient_t)1,     // ACTION_ATTACK_LOOTER_FIVE
            (coefficient_t)1,     // ACTION_ATTACK_OMEN_FIVE
            (coefficient_t)1,     // ACTION_DOOM_THREE
            (coefficient_t)1,     // ACTION_DOOM_FIVE
            (coefficient_t)3,     // ACTION_DURATION_TWO
            (coefficient_t)12,     // ACTION_DURATION_THREE
            (coefficient_t)9,     // ACTION_DURATION_FOUR
            (coefficient_t)12,     // ACTION_DURATION_FIVE
            (coefficient_t)3,     // ACTION_DURATION_OTHER
            (coefficient_t)1,     // ACTION_DURATION_LIAISON_THREE
            (coefficient_t)1,     // ACTION_DURATION_LOOT_TWO
            (coefficient_t)2,     // ACTION_FATE_TWO
            (coefficient_t)1,     // ACTION_FATE_THREE
            (coefficient_t)2,     // ACTION_FATE_FOUR
            (coefficient_t)1,     // ACTION_FATE_FIVE
            (coefficient_t)1,     // ACTION_LIAISON_TWO
            (coefficient_t)2,     // ACTION_LIAISON_THREE
            (coefficient_t)1,     // ACTION_LIAISON_FOUR
            (coefficient_t)2,     // ACTION_LIAISON_FIVE
            (coefficient_t)1,     // ACTION_LOOT_FIVE
            (coefficient_t)1,     // ACTION_LOOTER_FOUR
            (coefficient_t)3,     // ACTION_OMEN_FOUR
            (coefficient_t)1,     // ACTION_OMEN_FIVE
            (coefficient_t)1,     // ACTION_OMEN_OTHER
            (coefficient_t)1,     // ACTION_TREASURE_FIVE
            (coefficient_t)1,     // ATTACK_DOOM_FIVE
            (coefficient_t)1,     // ATTACK_DURATION_OTHER
            (coefficient_t)1,     // ATTACK_FATE_TREASURE_FIVE
            (coefficient_t)2,     // ATTACK_TREASURE_FIVE
            (coefficient_t)1,     // DURATION_TWO
            (coefficient_t)1,     // DURATION_THREE
            (coefficient_t)3,     // DURATION_FIVE
            (coefficient_t)1,     // DURATION_LIAISON_TREASURE_FIVE
            (coefficient_t)1,     // DURATION_TREASURE_TWO
            (coefficient_t)1,     // DURATION_TREASURE_THREE
            (coefficient_t)3,     // DURATION_TREASURE_FOUR
            (coefficient_t)1,     // DURATION_TREASURE_FIVE
            (coefficient_t)1,     // LIAISON_TREASURE_TWO
            (coefficient_t)1,     // LOOT_TREASURE_TWO
            (coefficient_t)1,     // LOOT_TREASURE_FIVE
            (coefficient_t)1,     // LOOT_TREASURE_OTHER
            (coefficient_t)4,     // TREASURE_TWO
            (coefficient_t)4,     // TREASURE_THREE
            (coefficient_t)6,     // TREASURE_FOUR
            (coefficient_t)19,     // TREASURE_FIVE
            (coefficient_t)6,     // TREASURE_OTHER
            (coefficient_t)1,     // YOUNG WITCH
            (coefficient_t)1,     // FERRYMAN
            (coefficient_t)1,     // KNIGHTS
            (coefficient_t)1,     // DRUID
            (coefficient_t)1     // RIVERBOAT
        ));

        ASSERT_THAT(actual.landscapes_, FieldsAre(
            (coefficient_t)79,     // EVENT
            (coefficient_t)15,     // TRAIT
            (coefficient_t)59,     // OTHER_SUPPLY
            (coefficient_t)1,     // OBELISK
            (coefficient_t)1,     // WAY OF THE MOUSE
            (coefficient_t)1     // APPROACHING ARMY
        ));
    }

    TEST(generator_bounds, ferryman_knights_way_of_the_mouse){
        const auto expansion_edition_filter = ExpansionEditionFilter{
            { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK },
            { Expansion::DARK_AGES, EditionModifier::NONE },
            { Expansion::MENAGERIE, EditionModifier::NONE },
        };

        const auto actual = from_expansions_editions(expansion_edition_filter);
        ASSERT_THAT(actual.kingdom_, FieldsAre(
            (coefficient_t)0,     // NONE_TWO
            (coefficient_t)0,     // NONE_THREE
            (coefficient_t)1,     // NONE_FOUR
            (coefficient_t)0,     // NONE_FIVE
            (coefficient_t)0,     // NONE_OTHER
            (coefficient_t)5,     // ACTION_TWO
            (coefficient_t)12,     // ACTION_THREE
            (coefficient_t)12,     // ACTION_FOUR
            (coefficient_t)18,     // ACTION_FIVE
            (coefficient_t)6,     // ACTION_OTHER
            (coefficient_t)1,     // ACTION_ATTACK_TWO
            (coefficient_t)1,     // ACTION_ATTACK_THREE
            (coefficient_t)1,     // ACTION_ATTACK_FOUR
            (coefficient_t)4,     // ACTION_ATTACK_FIVE
            (coefficient_t)0,     // ACTION_ATTACK_OTHER
            (coefficient_t)0,     // ACTION_ATTACK_DOOM_FOUR
            (coefficient_t)0,     // ACTION_ATTACK_DOOM_FIVE
            (coefficient_t)0,     // ACTION_ATTACK_DURATION_THREE
            (coefficient_t)0,     // ACTION_ATTACK_DURATION_FOUR
            (coefficient_t)1,     // ACTION_ATTACK_DURATION_FIVE
            (coefficient_t)0,     // ACTION_ATTACK_DURATION_OTHER
            (coefficient_t)0,     // ACTION_ATTACK_DURATION_LOOT_FIVE
            (coefficient_t)1,     // ACTION_ATTACK_LOOTER_FOUR
            (coefficient_t)1,     // ACTION_ATTACK_LOOTER_FIVE
            (coefficient_t)0,     // ACTION_ATTACK_OMEN_FIVE
            (coefficient_t)0,     // ACTION_DOOM_THREE
            (coefficient_t)0,     // ACTION_DOOM_FIVE
            (coefficient_t)0,     // ACTION_DURATION_TWO
            (coefficient_t)0,     // ACTION_DURATION_THREE
            (coefficient_t)1,     // ACTION_DURATION_FOUR
            (coefficient_t)2,     // ACTION_DURATION_FIVE
            (coefficient_t)0,     // ACTION_DURATION_OTHER
            (coefficient_t)0,     // ACTION_DURATION_LIAISON_THREE
            (coefficient_t)0,     // ACTION_DURATION_LOOT_TWO
            (coefficient_t)0,     // ACTION_FATE_TWO
            (coefficient_t)0,     // ACTION_FATE_THREE
            (coefficient_t)0,     // ACTION_FATE_FOUR
            (coefficient_t)0,     // ACTION_FATE_FIVE
            (coefficient_t)0,     // ACTION_LIAISON_TWO
            (coefficient_t)0,     // ACTION_LIAISON_THREE
            (coefficient_t)0,     // ACTION_LIAISON_FOUR
            (coefficient_t)0,     // ACTION_LIAISON_FIVE
            (coefficient_t)0,     // ACTION_LOOT_FIVE
            (coefficient_t)1,     // ACTION_LOOTER_FOUR
            (coefficient_t)0,     // ACTION_OMEN_FOUR
            (coefficient_t)0,     // ACTION_OMEN_FIVE
            (coefficient_t)0,     // ACTION_OMEN_OTHER
            (coefficient_t)0,     // ACTION_TREASURE_FIVE
            (coefficient_t)0,     // ATTACK_DOOM_FIVE
            (coefficient_t)0,     // ATTACK_DURATION_OTHER
            (coefficient_t)0,     // ATTACK_FATE_TREASURE_FIVE
            (coefficient_t)0,     // ATTACK_TREASURE_FIVE
            (coefficient_t)0,     // DURATION_TWO
            (coefficient_t)0,     // DURATION_THREE
            (coefficient_t)0,     // DURATION_FIVE
            (coefficient_t)0,     // DURATION_LIAISON_TREASURE_FIVE
            (coefficient_t)0,     // DURATION_TREASURE_TWO
            (coefficient_t)0,     // DURATION_TREASURE_THREE
            (coefficient_t)0,     // DURATION_TREASURE_FOUR
            (coefficient_t)0,     // DURATION_TREASURE_FIVE
            (coefficient_t)0,     // LIAISON_TREASURE_TWO
            (coefficient_t)0,     // LOOT_TREASURE_TWO
            (coefficient_t)0,     // LOOT_TREASURE_FIVE
            (coefficient_t)0,     // LOOT_TREASURE_OTHER
            (coefficient_t)1,     // TREASURE_TWO
            (coefficient_t)1,     // TREASURE_THREE
            (coefficient_t)0,     // TREASURE_FOUR
            (coefficient_t)1,     // TREASURE_FIVE
            (coefficient_t)0,     // TREASURE_OTHER
            (coefficient_t)0,     // YOUNG_WITCH
            (coefficient_t)1,     // FERRYMAN
            (coefficient_t)1,     // KNIGHTS
            (coefficient_t)0,     // DRUID
            (coefficient_t)0     // RIVERBOAT
        ));

        ASSERT_THAT(actual.landscapes_, FieldsAre(
            (coefficient_t)20,     // EVENT
            (coefficient_t)0,     // TRAIT
            (coefficient_t)19,     // OTHER_SUPPLY
            (coefficient_t)0,     // OBELISK
            (coefficient_t)1,     // WAY_OF_THE_MOUSE
            (coefficient_t)0     // APPROACHING_ARMY
        ));
    }

}