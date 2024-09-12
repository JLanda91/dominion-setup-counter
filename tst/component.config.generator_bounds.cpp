#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/generator_bounds.hpp"
#include "lib/default_expansion_editions.hpp"

namespace {
    using config::coefficient_t;
    using config::GeneratorBounds;
    using config::Expansion;
    using config::kAllExpansionEditions;
    using config::kingdom::ExpansionEditionFilter;
    using config::kingdom::EditionModifier;

    using testing::FieldsAre;

    TEST(generator_bounds, all_expansion_editions) {
    	const auto& expansion_edition_filter = kAllExpansionEditions;

    	const auto actual = from_expansions_editions(expansion_edition_filter);
        ASSERT_THAT(actual.kingdom_, FieldsAre(
    		(coefficient_t)1u,	// NONE_TWO
    		(coefficient_t)4u,	// NONE_THREE
    		(coefficient_t)6u,	// NONE_FOUR
    		(coefficient_t)2u,	// NONE_FIVE
    		(coefficient_t)3u,	// NONE_OTHER
    		(coefficient_t)34u,	// ACTION_TWO
    		(coefficient_t)53u,	// ACTION_THREE
    		(coefficient_t)84u,	// ACTION_FOUR
    		(coefficient_t)87u,	// ACTION_FIVE
    		(coefficient_t)28u,	// ACTION_OTHER
    		(coefficient_t)2u,	// ACTION_ATTACK_TWO
    		(coefficient_t)6u,	// ACTION_ATTACK_THREE
    		(coefficient_t)12u,	// ACTION_ATTACK_FOUR
    		(coefficient_t)26u,	// ACTION_ATTACK_FIVE
    		(coefficient_t)3u,	// ACTION_ATTACK_OTHER
    		(coefficient_t)1u,	// ACTION_ATTACK_DOOM_FOUR
    		(coefficient_t)2u,	// ACTION_ATTACK_DOOM_FIVE
    		(coefficient_t)2u,	// ACTION_ATTACK_DURATION_THREE
    		(coefficient_t)1u,	// ACTION_ATTACK_DURATION_FOUR
    		(coefficient_t)8u,	// ACTION_ATTACK_DURATION_FIVE
    		(coefficient_t)1u,	// ACTION_ATTACK_DURATION_OTHER
    		(coefficient_t)1u,	// ACTION_ATTACK_DURATION_LOOT_FIVE
    		(coefficient_t)1u,	// ACTION_ATTACK_LOOTER_FOUR
    		(coefficient_t)1u,	// ACTION_ATTACK_LOOTER_FIVE
    		(coefficient_t)1u,	// ACTION_ATTACK_OMEN_FIVE
    		(coefficient_t)1u,	// ACTION_DOOM_THREE
    		(coefficient_t)1u,	// ACTION_DOOM_FIVE
    		(coefficient_t)3u,	// ACTION_DURATION_TWO
    		(coefficient_t)12u,	// ACTION_DURATION_THREE
    		(coefficient_t)9u,	// ACTION_DURATION_FOUR
    		(coefficient_t)12u,	// ACTION_DURATION_FIVE
    		(coefficient_t)3u,	// ACTION_DURATION_OTHER
    		(coefficient_t)1u,	// ACTION_DURATION_LIAISON_THREE
    		(coefficient_t)1u,	// ACTION_DURATION_LOOT_TWO
    		(coefficient_t)2u,	// ACTION_FATE_TWO
    		(coefficient_t)1u,	// ACTION_FATE_THREE
    		(coefficient_t)2u,	// ACTION_FATE_FOUR
    		(coefficient_t)1u,	// ACTION_FATE_FIVE
    		(coefficient_t)1u,	// ACTION_LIAISON_TWO
    		(coefficient_t)2u,	// ACTION_LIAISON_THREE
    		(coefficient_t)1u,	// ACTION_LIAISON_FOUR
    		(coefficient_t)2u,	// ACTION_LIAISON_FIVE
    		(coefficient_t)1u,	// ACTION_LOOT_FIVE
    		(coefficient_t)1u,	// ACTION_LOOTER_FOUR
    		(coefficient_t)3u,	// ACTION_OMEN_FOUR
    		(coefficient_t)1u,	// ACTION_OMEN_FIVE
    		(coefficient_t)1u,	// ACTION_OMEN_OTHER
    		(coefficient_t)1u,	// ACTION_TREASURE_FIVE
    		(coefficient_t)1u,	// ATTACK_DOOM_FIVE
    		(coefficient_t)1u,	// ATTACK_DURATION_OTHER
    		(coefficient_t)1u,	// ATTACK_FATE_TREASURE_FIVE
    		(coefficient_t)2u,	// ATTACK_TREASURE_FIVE
    		(coefficient_t)1u,	// DURATION_TWO
    		(coefficient_t)1u,	// DURATION_THREE
    		(coefficient_t)3u,	// DURATION_FIVE
    		(coefficient_t)1u,	// DURATION_LIAISON_TREASURE_FIVE
    		(coefficient_t)1u,	// DURATION_TREASURE_TWO
    		(coefficient_t)1u,	// DURATION_TREASURE_THREE
    		(coefficient_t)3u,	// DURATION_TREASURE_FOUR
    		(coefficient_t)1u,	// DURATION_TREASURE_FIVE
    		(coefficient_t)1u,	// LIAISON_TREASURE_TWO
    		(coefficient_t)1u,	// LOOT_TREASURE_TWO
    		(coefficient_t)1u,	// LOOT_TREASURE_FIVE
    		(coefficient_t)1u,	// LOOT_TREASURE_OTHER
    		(coefficient_t)4u,	// TREASURE_TWO
    		(coefficient_t)4u,	// TREASURE_THREE
    		(coefficient_t)6u,	// TREASURE_FOUR
    		(coefficient_t)19u,	// TREASURE_FIVE
    		(coefficient_t)6u,	// TREASURE_OTHER

    		(coefficient_t)1u,	// YOUNG_WITCH
    		(coefficient_t)1u,	// FERRYMAN
    		(coefficient_t)1u,	// KNIGHTS
    		(coefficient_t)1u,	// DRUID
    		(coefficient_t)1u	// RIVERBOAT
    	));

        ASSERT_THAT(actual.landscapes_, FieldsAre(
    		(coefficient_t)79u,	// EVENT
    		(coefficient_t)15u,	// TRAIT
    		(coefficient_t)59u,	// OTHER_SUPPLY

    		(coefficient_t)1u,	// OBELISK
    		(coefficient_t)1u	// WAY_OF_THE_MOUSE
    	));
    }

    TEST(generator_bounds, ferryman_knights_way_of_the_mouse) {
    	const auto expansion_edition_filter = ExpansionEditionFilter{
    		{ Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK },
    		{ Expansion::DARK_AGES, EditionModifier::NONE },
    		{ Expansion::MENAGERIE, EditionModifier::NONE },
    	};

    	const auto actual = from_expansions_editions(expansion_edition_filter);
        ASSERT_THAT(actual.kingdom_, FieldsAre(
    		(coefficient_t)0u,	// NONE_TWO
    		(coefficient_t)0u,	// NONE_THREE
    		(coefficient_t)1u,	// NONE_FOUR
    		(coefficient_t)0u,	// NONE_FIVE
    		(coefficient_t)0u,	// NONE_OTHER
    		(coefficient_t)5u,	// ACTION_TWO
    		(coefficient_t)12u,	// ACTION_THREE
    		(coefficient_t)12u,	// ACTION_FOUR
    		(coefficient_t)18u,	// ACTION_FIVE
    		(coefficient_t)6u,	// ACTION_OTHER
    		(coefficient_t)1u,	// ACTION_ATTACK_TWO
    		(coefficient_t)1u,	// ACTION_ATTACK_THREE
    		(coefficient_t)1u,	// ACTION_ATTACK_FOUR
    		(coefficient_t)4u,	// ACTION_ATTACK_FIVE
    		(coefficient_t)0u,	// ACTION_ATTACK_OTHER
    		(coefficient_t)0u,	// ACTION_ATTACK_DOOM_FOUR
    		(coefficient_t)0u,	// ACTION_ATTACK_DOOM_FIVE
    		(coefficient_t)0u,	// ACTION_ATTACK_DURATION_THREE
    		(coefficient_t)0u,	// ACTION_ATTACK_DURATION_FOUR
    		(coefficient_t)1u,	// ACTION_ATTACK_DURATION_FIVE
    		(coefficient_t)0u,	// ACTION_ATTACK_DURATION_OTHER
    		(coefficient_t)0u,	// ACTION_ATTACK_DURATION_LOOT_FIVE
    		(coefficient_t)1u,	// ACTION_ATTACK_LOOTER_FOUR
    		(coefficient_t)1u,	// ACTION_ATTACK_LOOTER_FIVE
    		(coefficient_t)0u,	// ACTION_ATTACK_OMEN_FIVE
    		(coefficient_t)0u,	// ACTION_DOOM_THREE
    		(coefficient_t)0u,	// ACTION_DOOM_FIVE
    		(coefficient_t)0u,	// ACTION_DURATION_TWO
    		(coefficient_t)0u,	// ACTION_DURATION_THREE
    		(coefficient_t)1u,	// ACTION_DURATION_FOUR
    		(coefficient_t)2u,	// ACTION_DURATION_FIVE
    		(coefficient_t)0u,	// ACTION_DURATION_OTHER
    		(coefficient_t)0u,	// ACTION_DURATION_LIAISON_THREE
    		(coefficient_t)0u,	// ACTION_DURATION_LOOT_TWO
    		(coefficient_t)0u,	// ACTION_FATE_TWO
    		(coefficient_t)0u,	// ACTION_FATE_THREE
    		(coefficient_t)0u,	// ACTION_FATE_FOUR
    		(coefficient_t)0u,	// ACTION_FATE_FIVE
    		(coefficient_t)0u,	// ACTION_LIAISON_TWO
    		(coefficient_t)0u,	// ACTION_LIAISON_THREE
    		(coefficient_t)0u,	// ACTION_LIAISON_FOUR
    		(coefficient_t)0u,	// ACTION_LIAISON_FIVE
    		(coefficient_t)0u,	// ACTION_LOOT_FIVE
    		(coefficient_t)1u,	// ACTION_LOOTER_FOUR
    		(coefficient_t)0u,	// ACTION_OMEN_FOUR
    		(coefficient_t)0u,	// ACTION_OMEN_FIVE
    		(coefficient_t)0u,	// ACTION_OMEN_OTHER
    		(coefficient_t)0u,	// ACTION_TREASURE_FIVE
    		(coefficient_t)0u,	// ATTACK_DOOM_FIVE
    		(coefficient_t)0u,	// ATTACK_DURATION_OTHER
    		(coefficient_t)0u,	// ATTACK_FATE_TREASURE_FIVE
    		(coefficient_t)0u,	// ATTACK_TREASURE_FIVE
    		(coefficient_t)0u,	// DURATION_TWO
    		(coefficient_t)0u,	// DURATION_THREE
    		(coefficient_t)0u,	// DURATION_FIVE
    		(coefficient_t)0u,	// DURATION_LIAISON_TREASURE_FIVE
    		(coefficient_t)0u,	// DURATION_TREASURE_TWO
    		(coefficient_t)0u,	// DURATION_TREASURE_THREE
    		(coefficient_t)0u,	// DURATION_TREASURE_FOUR
    		(coefficient_t)0u,	// DURATION_TREASURE_FIVE
    		(coefficient_t)0u,	// LIAISON_TREASURE_TWO
    		(coefficient_t)0u,	// LOOT_TREASURE_TWO
    		(coefficient_t)0u,	// LOOT_TREASURE_FIVE
    		(coefficient_t)0u,	// LOOT_TREASURE_OTHER
    		(coefficient_t)1u,	// TREASURE_TWO
    		(coefficient_t)1u,	// TREASURE_THREE
    		(coefficient_t)0u,	// TREASURE_FOUR
    		(coefficient_t)1u,	// TREASURE_FIVE
    		(coefficient_t)0u,	// TREASURE_OTHER

    		(coefficient_t)0u,	// YOUNG_WITCH
    		(coefficient_t)1u,	// FERRYMAN
    		(coefficient_t)1u,	// KNIGHTS
    		(coefficient_t)0u,	// DRUID
    		(coefficient_t)0u	// RIVERBOAT
    	));

        ASSERT_THAT(actual.landscapes_, FieldsAre(
    		(coefficient_t)20u,	// EVENT
    		(coefficient_t)0u,	// TRAIT
    		(coefficient_t)19u,	// OTHER_SUPPLY

    		(coefficient_t)0u,	// OBELISK
    		(coefficient_t)1u	// WAY_OF_THE_MOUSE
    	));
    }


}