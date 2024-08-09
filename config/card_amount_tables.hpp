#pragma once

#include <cstdint>
#include <array>
#include <utility>

#include "sized_enum.hpp"
#include "enum_mask.hpp"

namespace config {
    enum class Expansion : uint8_t {
        DOMINION,
        INTRIGUE,
        SEASIDE,
        ALCHEMY,
        PROSPERITY,
        CORNUCOPIA,
        HINTERLANDS,
        DARK_AGES,
        GUILDS,
        CORNUCOPIA_GUILDS,
        ADVENTURES,
        EMPIRES,
        NOCTURNE,
        RENAISSANCE,
        MENAGERIE,
        ALLIES,
        PROMO,
    };
    
    using SizedExpansion = utils::enums::SizedEnum<Expansion, 17uz>;
    
    namespace kingdom {
        enum class EditionModifier : uint8_t {
            NONE,
            UPDATE_PACK,
            REMOVED,
        };
    
        using SizedEditionModifier = utils::enums::SizedEnum<EditionModifier, 3uz>;
    }
    
    namespace kingdom {
        enum class PrimaryType : uint8_t {
            ACTION,
            TREASURE,
            OTHER,
        };
    
        using SizedPrimaryType = utils::enums::SizedEnum<PrimaryType, 3uz>;
    }
    
    namespace kingdom {
        enum class SecondaryType : uint8_t {
            REGULAR,
            LOOTER,
            FATE,
            DOOM,
            LIAISON,
        };
    
        using SizedSecondaryType = utils::enums::SizedEnum<SecondaryType, 5uz>;
    }
    
    namespace kingdom {
        enum class Cost : uint8_t {
            TWO,
            THREE,
            FOUR,
            FIVE,
            SIX,
            POTION,
            SEVEN,
            EIGHT,
            ONE,
            DEBT,
        };
    
        using SizedCost = utils::enums::SizedEnum<Cost, 10uz>;
    }
    
    namespace landscapes {
        enum class Type : uint8_t {
            ALLY,
            ARTIFACT,
            BOON,
            EVENT,
            HEX,
            LANDMARK,
            PROJECT,
            STATE,
            WAY,
        };
    
        using SizedType = utils::enums::SizedEnum<Type, 9uz>;
    }
    

    namespace kingdom {
        struct row_t {
            std::pair<Expansion, EditionModifier> expansion_edition_;
            PrimaryType primary_type_;
            SecondaryType secondary_type_;
            Cost cost_;
            int8_t amount_;
    
            constexpr bool operator==(const row_t& other) const noexcept = default;
        };
    
        using table_t = std::array<row_t, 184uz>;
    
        static constexpr auto& table() noexcept {
            static constexpr table_t singleton {{
                { { Expansion::DOMINION, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 3 },
                { { Expansion::DOMINION, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 4 },
                { { Expansion::DOMINION, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 9 },
                { { Expansion::DOMINION, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 7 },
                { { Expansion::DOMINION, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::DOMINION, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 3 },
                { { Expansion::DOMINION, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 1 },
                { { Expansion::DOMINION, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 2 },
                { { Expansion::DOMINION, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::DOMINION, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, -2 },
                { { Expansion::DOMINION, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, -3 },
                { { Expansion::DOMINION, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, -1 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 3 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 6 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 7 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 6 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::INTRIGUE, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 1 },
                { { Expansion::INTRIGUE, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 3 },
                { { Expansion::INTRIGUE, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 3 },
                { { Expansion::INTRIGUE, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, -1 },
                { { Expansion::INTRIGUE, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, -1 },
                { { Expansion::INTRIGUE, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, -2 },
                { { Expansion::INTRIGUE, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, -2 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 5 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 5 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 8 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 8 },
                { { Expansion::SEASIDE, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 2 },
                { { Expansion::SEASIDE, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 3 },
                { { Expansion::SEASIDE, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 3 },
                { { Expansion::SEASIDE, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, -2 },
                { { Expansion::SEASIDE, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, -1 },
                { { Expansion::SEASIDE, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, -3 },
                { { Expansion::SEASIDE, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, -2 },
                { { Expansion::ALCHEMY, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 1 },
                { { Expansion::ALCHEMY, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 1 },
                { { Expansion::ALCHEMY, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::POTION, 8 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 2 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 3 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 6 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 2 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SEVEN, 3 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::EIGHT, 1 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 1 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 2 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, -1 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, -2 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, -1 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 1 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 2 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 4 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 3 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 2 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 4 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 5 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 8 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 3 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 4 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::ONE, 1 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 3 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 6 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 7 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 10 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 2 },
                { { Expansion::GUILDS, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 2 },
                { { Expansion::GUILDS, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 1 },
                { { Expansion::GUILDS, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 4 },
                { { Expansion::GUILDS, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 5 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 1 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 2 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 1 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 3 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, -3 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, -4 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::REMOVED }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, -1 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 4 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 5 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 7 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 10 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 3 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 5 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 3 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 6 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::DEBT, 4 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 3 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 2 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 2 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 4 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 8 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 8 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 2 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 5 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 6 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 12 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 2 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SEVEN, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::TWO, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 6 },
                { { Expansion::ALLIES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 5 },
                { { Expansion::ALLIES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 9 },
                { { Expansion::ALLIES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::PROMO, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::THREE, 2 },
                { { Expansion::PROMO, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FOUR, 4 },
                { { Expansion::PROMO, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::FIVE, 1 },
                { { Expansion::PROMO, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::PROMO, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::REGULAR, Cost::EIGHT, 1 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::INTRIGUE, EditionModifier::UPDATE_PACK }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::SIX, 2 },
                { { Expansion::INTRIGUE, EditionModifier::REMOVED }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::SEASIDE, EditionModifier::UPDATE_PACK }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::THREE, 1 },
                { { Expansion::SEASIDE, EditionModifier::REMOVED }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::THREE, 1 },
                { { Expansion::ALCHEMY, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::POTION, 1 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::THREE, 1 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FOUR, 2 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FIVE, 3 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::SEVEN, 1 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::THREE, 2 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FOUR, 4 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FIVE, 6 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::SEVEN, 1 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FOUR, 3 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FIVE, 3 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::SEVEN, 1 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FIVE, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::TWO, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FIVE, 2 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::TWO, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FIVE, 3 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FIVE, 1 },
                { { Expansion::GUILDS, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::THREE, 1 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::TWO, 1 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FIVE, 2 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FIVE, 2 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::TWO, 1 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FIVE, 2 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::TWO, 1 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::THREE, 1 },
                { { Expansion::PROMO, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::REGULAR, Cost::FIVE, 1 },
                { { Expansion::DOMINION, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::FOUR, 1 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::FIVE, 1 },
                { { Expansion::INTRIGUE, EditionModifier::UPDATE_PACK }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::FIVE, 1 },
                { { Expansion::INTRIGUE, EditionModifier::REMOVED }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::FIVE, 1 },
                { { Expansion::ALCHEMY, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::POTION, 1 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::THREE, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::FOUR, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::THREE, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::FOUR, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::FOUR, 1 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::THREE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::TWO, 2 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::THREE, 3 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::FOUR, 3 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::FIVE, 3 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::SIX, 1 },
                { { Expansion::PROMO, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::REGULAR, Cost::FIVE, 1 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::LOOTER, Cost::FOUR, 2 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::LOOTER, Cost::FIVE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::FATE, Cost::TWO, 2 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::FATE, Cost::THREE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::FATE, Cost::FOUR, 2 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::FATE, Cost::FIVE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::FATE, Cost::FIVE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::DOOM, Cost::THREE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::DOOM, Cost::FOUR, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::DOOM, Cost::FIVE, 3 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, PrimaryType::OTHER, SecondaryType::DOOM, Cost::FIVE, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::LIAISON, Cost::TWO, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::LIAISON, Cost::THREE, 3 },
                { { Expansion::ALLIES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::LIAISON, Cost::FOUR, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, PrimaryType::ACTION, SecondaryType::LIAISON, Cost::FIVE, 2 },
                { { Expansion::ALLIES, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::LIAISON, Cost::TWO, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, PrimaryType::TREASURE, SecondaryType::LIAISON, Cost::FIVE, 1 },
            }};
            return singleton;
        }
    
        using ExpansionEditionFilter = utils::table::EnumMask<SizedExpansion, SizedEditionModifier>;
        using PrimaryTypeFilter = utils::table::EnumMask<SizedPrimaryType>;
        using SecondaryTypeFilter = utils::table::EnumMask<SizedSecondaryType>;
        using CostFilter = utils::table::EnumMask<SizedCost>;
    
        struct TableQuery {
            PrimaryTypeFilter primary_type_{};
            SecondaryTypeFilter secondary_type_{};
            CostFilter cost_{};
        };
    
        using queries_t = std::array<TableQuery, 29uz>;
    
        static constexpr auto& amount_queries() noexcept {
            static constexpr queries_t singleton {{
                { {PrimaryType::ACTION, }, {SecondaryType::REGULAR, }, {Cost::TWO, } },    // ACTION_REGULAR_TWO
                { {PrimaryType::ACTION, }, {SecondaryType::REGULAR, }, {Cost::THREE, } },    // ACTION_REGULAR_THREE
                { {PrimaryType::ACTION, }, {SecondaryType::REGULAR, }, {Cost::FOUR, } },    // ACTION_REGULAR_FOUR
                { {PrimaryType::ACTION, }, {SecondaryType::REGULAR, }, {Cost::ONE, Cost::FIVE, Cost::SIX, Cost::SEVEN, Cost::EIGHT, Cost::POTION, Cost::DEBT, } },    // ACTION_REGULAR_OTHER
                { {PrimaryType::ACTION, }, {SecondaryType::LOOTER, }, {Cost::FOUR, } },    // ACTION_LOOTER_FOUR
                { {PrimaryType::ACTION, }, {SecondaryType::LOOTER, }, {Cost::ONE, Cost::FIVE, Cost::SIX, Cost::SEVEN, Cost::EIGHT, Cost::POTION, Cost::DEBT, } },    // ACTION_LOOTER_OTHER
                { {PrimaryType::ACTION, }, {SecondaryType::FATE, }, {Cost::TWO, } },    // ACTION_FATE_TWO
                { {PrimaryType::ACTION, }, {SecondaryType::FATE, }, {Cost::THREE, } },    // ACTION_FATE_THREE
                { {PrimaryType::ACTION, }, {SecondaryType::FATE, }, {Cost::FOUR, } },    // ACTION_FATE_FOUR
                { {PrimaryType::ACTION, }, {SecondaryType::FATE, }, {Cost::ONE, Cost::FIVE, Cost::SIX, Cost::SEVEN, Cost::EIGHT, Cost::POTION, Cost::DEBT, } },    // ACTION_FATE_OTHER
                { {PrimaryType::ACTION, }, {SecondaryType::DOOM, }, {Cost::THREE, } },    // ACTION_DOOM_THREE
                { {PrimaryType::ACTION, }, {SecondaryType::DOOM, }, {Cost::FOUR, } },    // ACTION_DOOM_FOUR
                { {PrimaryType::ACTION, }, {SecondaryType::DOOM, }, {Cost::ONE, Cost::FIVE, Cost::SIX, Cost::SEVEN, Cost::EIGHT, Cost::POTION, Cost::DEBT, } },    // ACTION_DOOM_OTHER
                { {PrimaryType::ACTION, }, {SecondaryType::LIAISON, }, {Cost::TWO, } },    // ACTION_LIAISON_TWO
                { {PrimaryType::ACTION, }, {SecondaryType::LIAISON, }, {Cost::THREE, } },    // ACTION_LIAISON_THREE
                { {PrimaryType::ACTION, }, {SecondaryType::LIAISON, }, {Cost::FOUR, } },    // ACTION_LIAISON_FOUR
                { {PrimaryType::ACTION, }, {SecondaryType::LIAISON, }, {Cost::ONE, Cost::FIVE, Cost::SIX, Cost::SEVEN, Cost::EIGHT, Cost::POTION, Cost::DEBT, } },    // ACTION_LIAISON_OTHER
                { {PrimaryType::TREASURE, }, {SecondaryType::REGULAR, }, {Cost::TWO, } },    // TREASURE_REGULAR_TWO
                { {PrimaryType::TREASURE, }, {SecondaryType::REGULAR, }, {Cost::THREE, } },    // TREASURE_REGULAR_THREE
                { {PrimaryType::TREASURE, }, {SecondaryType::REGULAR, }, {Cost::FOUR, } },    // TREASURE_REGULAR_FOUR
                { {PrimaryType::TREASURE, }, {SecondaryType::REGULAR, }, {Cost::ONE, Cost::FIVE, Cost::SIX, Cost::SEVEN, Cost::EIGHT, Cost::POTION, Cost::DEBT, } },    // TREASURE_REGULAR_OTHER
                { {PrimaryType::TREASURE, }, {SecondaryType::FATE, }, {Cost::ONE, Cost::FIVE, Cost::SIX, Cost::SEVEN, Cost::EIGHT, Cost::POTION, Cost::DEBT, } },    // TREASURE_FATE_OTHER
                { {PrimaryType::TREASURE, }, {SecondaryType::LIAISON, }, {Cost::TWO, } },    // TREASURE_LIAISON_TWO
                { {PrimaryType::TREASURE, }, {SecondaryType::LIAISON, }, {Cost::ONE, Cost::FIVE, Cost::SIX, Cost::SEVEN, Cost::EIGHT, Cost::POTION, Cost::DEBT, } },    // TREASURE_LIAISON_OTHER
                { {PrimaryType::OTHER, }, {SecondaryType::REGULAR, }, {Cost::TWO, } },    // OTHER_REGULAR_TWO
                { {PrimaryType::OTHER, }, {SecondaryType::REGULAR, }, {Cost::THREE, } },    // OTHER_REGULAR_THREE
                { {PrimaryType::OTHER, }, {SecondaryType::REGULAR, }, {Cost::FOUR, } },    // OTHER_REGULAR_FOUR
                { {PrimaryType::OTHER, }, {SecondaryType::REGULAR, }, {Cost::ONE, Cost::FIVE, Cost::SIX, Cost::SEVEN, Cost::EIGHT, Cost::POTION, Cost::DEBT, } },    // OTHER_REGULAR_OTHER
                { {PrimaryType::OTHER, }, {SecondaryType::DOOM, }, {Cost::ONE, Cost::FIVE, Cost::SIX, Cost::SEVEN, Cost::EIGHT, Cost::POTION, Cost::DEBT, } },    // OTHER_DOOM_OTHER
            }};
            return singleton;
        }
    }
    

    namespace landscapes {
        struct row_t {
            Expansion expansion_;
            Type type_;
            int8_t amount_;
    
            constexpr bool operator==(const row_t& other) const noexcept = default;
        };
    
        using table_t = std::array<row_t, 12uz>;
    
        static constexpr auto& table() noexcept {
            static constexpr table_t singleton {{
                { Expansion::ADVENTURES, Type::EVENT, 20 },
                { Expansion::EMPIRES, Type::EVENT, 13 },
                { Expansion::EMPIRES, Type::LANDMARK, 20 },
                { Expansion::NOCTURNE, Type::BOON, 12 },
                { Expansion::NOCTURNE, Type::HEX, 12 },
                { Expansion::NOCTURNE, Type::STATE, 5 },
                { Expansion::RENAISSANCE, Type::ARTIFACT, 5 },
                { Expansion::RENAISSANCE, Type::PROJECT, 20 },
                { Expansion::MENAGERIE, Type::EVENT, 20 },
                { Expansion::MENAGERIE, Type::WAY, 19 },
                { Expansion::ALLIES, Type::ALLY, 23 },
                { Expansion::PROMO, Type::EVENT, 1 },
            }};
            return singleton;
        }
    
        using ExpansionFilter = utils::table::EnumMask<SizedExpansion>;
        using TypeFilter = utils::table::EnumMask<SizedType>;
    
        struct TableQuery {
            TypeFilter type_{};
        };
    
        using queries_t = std::array<TableQuery, 1uz>;
    
        static constexpr auto& amount_queries() noexcept {
            static constexpr queries_t singleton {{
                { {Type::EVENT, Type::LANDMARK, Type::PROJECT, Type::WAY, } },  // SETUP
            }};
            return singleton;
        }
    }
    

    namespace kingdom {
        using special_tests_t = std::array<typename ExpansionEditionFilter::enum_t, 3uz>;
    
        static constexpr auto& special_tests() noexcept {
            static constexpr special_tests_t singleton {{
                { Expansion::DARK_AGES, EditionModifier::NONE },    // KNIGHTS
                { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK },    // FERRYMAN
                { Expansion::NOCTURNE, EditionModifier::NONE },    // DRUID
            }};
            return singleton;
        }
    }
    

    namespace landscapes {
        using special_tests_t = std::array<typename ExpansionFilter::enum_t, 2uz>;
    
        static constexpr auto& special_tests() noexcept {
            static constexpr special_tests_t singleton {{
                Expansion::EMPIRES,   // OBELISK
                Expansion::MENAGERIE,   // WAY_OF_THE_MOUSE
            }};
            return singleton;
        }
    }
    

    namespace kingdom {
        enum class AmountIndex : uint8_t {
            ACTION_REGULAR_TWO,
            ACTION_REGULAR_THREE,
            ACTION_REGULAR_FOUR,
            ACTION_REGULAR_OTHER,
            ACTION_LOOTER_FOUR,
            ACTION_LOOTER_OTHER,
            ACTION_FATE_TWO,
            ACTION_FATE_THREE,
            ACTION_FATE_FOUR,
            ACTION_FATE_OTHER,
            ACTION_DOOM_THREE,
            ACTION_DOOM_FOUR,
            ACTION_DOOM_OTHER,
            ACTION_LIAISON_TWO,
            ACTION_LIAISON_THREE,
            ACTION_LIAISON_FOUR,
            ACTION_LIAISON_OTHER,
            TREASURE_REGULAR_TWO,
            TREASURE_REGULAR_THREE,
            TREASURE_REGULAR_FOUR,
            TREASURE_REGULAR_OTHER,
            TREASURE_FATE_OTHER,
            TREASURE_LIAISON_TWO,
            TREASURE_LIAISON_OTHER,
            OTHER_REGULAR_TWO,
            OTHER_REGULAR_THREE,
            OTHER_REGULAR_FOUR,
            OTHER_REGULAR_OTHER,
            OTHER_DOOM_OTHER,
            KNIGHTS,
            FERRYMAN,
            DRUID,
        };
    }
    
    namespace landscapes {
        enum class AmountIndex : uint8_t {
            SETUP,
            OBELISK,
            WAY_OF_THE_MOUSE,
        };
    }
    

}