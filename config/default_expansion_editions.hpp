#pragma once

#include "card_amount_tables.hpp"
#include "enum_mask.hpp"

namespace config {
    inline constexpr kingdom::ExpansionEditionFilter kDefaultExpansionEditions{
        { Expansion::DOMINION, kingdom::EditionModifier::NONE },
        { Expansion::DOMINION, kingdom::EditionModifier::UPDATE_PACK },
        { Expansion::INTRIGUE, kingdom::EditionModifier::NONE },
        { Expansion::INTRIGUE, kingdom::EditionModifier::UPDATE_PACK },
        { Expansion::SEASIDE, kingdom::EditionModifier::NONE },
        { Expansion::SEASIDE, kingdom::EditionModifier::UPDATE_PACK },
        { Expansion::ALCHEMY, kingdom::EditionModifier::NONE },
        { Expansion::PROSPERITY, kingdom::EditionModifier::NONE },
        { Expansion::PROSPERITY, kingdom::EditionModifier::UPDATE_PACK },
        { Expansion::CORNUCOPIA, kingdom::EditionModifier::NONE },
        { Expansion::HINTERLANDS, kingdom::EditionModifier::NONE },
        { Expansion::HINTERLANDS, kingdom::EditionModifier::UPDATE_PACK },
        { Expansion::DARK_AGES, kingdom::EditionModifier::NONE },
        { Expansion::GUILDS, kingdom::EditionModifier::NONE },
        { Expansion::CORNUCOPIA_GUILDS, kingdom::EditionModifier::UPDATE_PACK },
        { Expansion::ADVENTURES, kingdom::EditionModifier::NONE },
        { Expansion::EMPIRES, kingdom::EditionModifier::NONE },
        { Expansion::NOCTURNE, kingdom::EditionModifier::NONE },
        { Expansion::RENAISSANCE, kingdom::EditionModifier::NONE },
        { Expansion::MENAGERIE, kingdom::EditionModifier::NONE },
        { Expansion::ALLIES, kingdom::EditionModifier::NONE },
        { Expansion::PROMO, kingdom::EditionModifier::NONE },
    };

    inline constexpr kingdom::ExpansionEditionFilter kAllExpansionEditions{};
}