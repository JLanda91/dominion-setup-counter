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
        GUILDS,
        CORNUCOPIA_GUILDS,
        HINTERLANDS,
        DARK_AGES,
        ADVENTURES,
        EMPIRES,
        NOCTURNE,
        RENAISSANCE,
        MENAGERIE,
        ALLIES,
        PLUNDER,
        RISING_SUN,
        PROMOS,
    };

    using SizedExpansion = utils::enums::SizedEnum<Expansion, 19uz>;

    namespace kingdom {
        enum class EditionModifier : uint8_t {
            NONE,
            UPDATE_PACK,
            REMOVED,
        };

        using SizedEditionModifier = utils::enums::SizedEnum<EditionModifier, 3uz>;
    }

    namespace kingdom {
        enum class CostGroup : uint8_t {
            TWO,
            THREE,
            FOUR,
            FIVE,
            OTHER,
        };

        using SizedCostGroup = utils::enums::SizedEnum<CostGroup, 5uz>;
    }

    namespace kingdom {
        enum class TrackedType : uint8_t {
            ACTION,
            ATTACK,
            DURATION,
            TREASURE,
            LOOTER,
            FATE,
            DOOM,
            LIAISON,
            LOOT,
            OMEN,
        };

        using SizedTrackedType = utils::enums::SizedEnum<TrackedType, 10uz>;
    }


    namespace kingdom {
        using TrackedTypeMask = utils::table::EnumMask<SizedTrackedType>;

        struct row_t {
            std::pair<Expansion, EditionModifier> expansion_edition_;
            TrackedTypeMask tracked_types_;
            CostGroup cost_group_;
            int8_t amount_;

            constexpr bool operator==(const row_t& other) const noexcept = default;
        };

        using table_t = std::array<row_t, 260uz>;

        static constexpr auto table() noexcept -> const table_t& {
            static constexpr table_t singleton {{
                { { Expansion::DOMINION, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 3 },
                { { Expansion::DOMINION, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 2 },
                { { Expansion::DOMINION, EditionModifier::NONE }, { }, CostGroup::FOUR, 1 },
                { { Expansion::DOMINION, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 4 },
                { { Expansion::DOMINION, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FOUR, 2 },
                { { Expansion::DOMINION, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 6 },
                { { Expansion::DOMINION, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::DOMINION, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::THREE, 3 },
                { { Expansion::DOMINION, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::FOUR, 1 },
                { { Expansion::DOMINION, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::FIVE, 1 },
                { { Expansion::DOMINION, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::DOMINION, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::OTHER, 1 },
                { { Expansion::DOMINION, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::THREE, -2 },
                { { Expansion::DOMINION, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::FOUR, -1 },
                { { Expansion::DOMINION, EditionModifier::REMOVED }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FOUR, -2 },
                { { Expansion::DOMINION, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::OTHER, -1 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 2 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 4 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::THREE, 1 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 5 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, { }, CostGroup::FIVE, 1 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 2 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 2 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::OTHER, 1 },
                { { Expansion::INTRIGUE, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::OTHER, 1 },
                { { Expansion::INTRIGUE, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::TWO, 1 },
                { { Expansion::INTRIGUE, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::FOUR, 3 },
                { { Expansion::INTRIGUE, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::FIVE, 2 },
                { { Expansion::INTRIGUE, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::INTRIGUE, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::TWO, -1 },
                { { Expansion::INTRIGUE, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::THREE, -1 },
                { { Expansion::INTRIGUE, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::FOUR, -2 },
                { { Expansion::INTRIGUE, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::FIVE, -1 },
                { { Expansion::INTRIGUE, EditionModifier::REMOVED }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, -1 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 1 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::TWO, 2 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 3 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::THREE, 1 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 3 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FOUR, 1 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::FOUR, 1 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 2 },
                { { Expansion::SEASIDE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::FIVE, 4 },
                { { Expansion::SEASIDE, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::THREE, 1 },
                { { Expansion::SEASIDE, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::THREE, 1 },
                { { Expansion::SEASIDE, EditionModifier::UPDATE_PACK }, { TrackedType::DURATION, TrackedType::TREASURE, }, CostGroup::THREE, 1 },
                { { Expansion::SEASIDE, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, CostGroup::FOUR, 1 },
                { { Expansion::SEASIDE, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::FOUR, 2 },
                { { Expansion::SEASIDE, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, CostGroup::FIVE, 2 },
                { { Expansion::SEASIDE, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::FIVE, 1 },
                { { Expansion::SEASIDE, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::TWO, -2 },
                { { Expansion::SEASIDE, EditionModifier::REMOVED }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::THREE, -1 },
                { { Expansion::SEASIDE, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::FOUR, -1 },
                { { Expansion::SEASIDE, EditionModifier::REMOVED }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FOUR, -2 },
                { { Expansion::SEASIDE, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::FIVE, -1 },
                { { Expansion::SEASIDE, EditionModifier::REMOVED }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, -1 },
                { { Expansion::ALCHEMY, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 1 },
                { { Expansion::ALCHEMY, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 1 },
                { { Expansion::ALCHEMY, EditionModifier::NONE }, { }, CostGroup::OTHER, 1 },
                { { Expansion::ALCHEMY, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::OTHER, 6 },
                { { Expansion::ALCHEMY, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::OTHER, 2 },
                { { Expansion::ALCHEMY, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::OTHER, 1 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 1 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 3 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::FOUR, 1 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 3 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::OTHER, 5 },
                { { Expansion::PROSPERITY, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::OTHER, 2 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, { TrackedType::TREASURE, }, CostGroup::THREE, 1 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FOUR, 1 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, { TrackedType::TREASURE, }, CostGroup::FOUR, 2 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::FIVE, 1 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::PROSPERITY, EditionModifier::UPDATE_PACK }, { TrackedType::TREASURE, }, CostGroup::FIVE, 3 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::THREE, -1 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, { TrackedType::TREASURE, }, CostGroup::THREE, -1 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, { TrackedType::TREASURE, }, CostGroup::FOUR, -1 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::FIVE, -1 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, -1 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, { TrackedType::TREASURE, }, CostGroup::FIVE, -3 },
                { { Expansion::PROSPERITY, EditionModifier::REMOVED }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::OTHER, -1 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 1 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 1 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 1 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 1 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::FIVE, 1 },
                { { Expansion::CORNUCOPIA, EditionModifier::NONE }, { }, CostGroup::OTHER, 1 },
                { { Expansion::CORNUCOPIA, EditionModifier::REMOVED }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::THREE, -1 },
                { { Expansion::CORNUCOPIA, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::FOUR, -3 },
                { { Expansion::CORNUCOPIA, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::FIVE, -1 },
                { { Expansion::GUILDS, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 2 },
                { { Expansion::GUILDS, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 3 },
                { { Expansion::GUILDS, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 4 },
                { { Expansion::GUILDS, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::GUILDS, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::THREE, -1 },
                { { Expansion::GUILDS, EditionModifier::REMOVED }, { TrackedType::TREASURE, }, CostGroup::THREE, -1 },
                { { Expansion::GUILDS, EditionModifier::REMOVED }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FOUR, -1 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::TWO, 1 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::THREE, 2 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::FOUR, 1 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::FIVE, 2 },
                { { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::TWO, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, { }, CostGroup::THREE, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 3 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 3 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 5 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, { }, CostGroup::OTHER, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::OTHER, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::THREE, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::FOUR, 3 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, }, CostGroup::FIVE, 2 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 2 },
                { { Expansion::HINTERLANDS, EditionModifier::UPDATE_PACK }, { TrackedType::ATTACK, TrackedType::TREASURE, }, CostGroup::FIVE, 1 },
                { { Expansion::HINTERLANDS, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::TWO, -1 },
                { { Expansion::HINTERLANDS, EditionModifier::REMOVED }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::THREE, -1 },
                { { Expansion::HINTERLANDS, EditionModifier::REMOVED }, { }, CostGroup::FOUR, -1 },
                { { Expansion::HINTERLANDS, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::FOUR, -1 },
                { { Expansion::HINTERLANDS, EditionModifier::REMOVED }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FOUR, -1 },
                { { Expansion::HINTERLANDS, EditionModifier::REMOVED }, { TrackedType::ACTION, }, CostGroup::FIVE, -2 },
                { { Expansion::HINTERLANDS, EditionModifier::REMOVED }, { TrackedType::TREASURE, }, CostGroup::FIVE, -2 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 3 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 5 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::THREE, 1 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { }, CostGroup::FOUR, 1 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 7 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::LOOTER, }, CostGroup::FOUR, 1 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::LOOTER, }, CostGroup::FOUR, 1 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 8 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 2 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::LOOTER, }, CostGroup::FIVE, 1 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::FIVE, 1 },
                { { Expansion::DARK_AGES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::OTHER, 3 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 4 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::TWO, 1 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 1 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::THREE, 4 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 7 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 6 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, CostGroup::FIVE, 3 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, { TrackedType::ATTACK, TrackedType::TREASURE, }, CostGroup::FIVE, 1 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::FIVE, 1 },
                { { Expansion::ADVENTURES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::OTHER, 1 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 3 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { }, CostGroup::THREE, 1 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 3 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::THREE, 1 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, CostGroup::THREE, 1 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 3 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 3 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::FIVE, 1 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::TREASURE, }, CostGroup::FIVE, 1 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::FIVE, 2 },
                { { Expansion::EMPIRES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::OTHER, 4 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { }, CostGroup::TWO, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::FATE, }, CostGroup::TWO, 2 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::DURATION, }, CostGroup::TWO, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { }, CostGroup::THREE, 2 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DOOM, }, CostGroup::THREE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::THREE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::FATE, }, CostGroup::THREE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::DURATION, }, CostGroup::THREE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { }, CostGroup::FOUR, 3 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 3 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DOOM, }, CostGroup::FOUR, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::FATE, }, CostGroup::FOUR, 2 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 2 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DOOM, }, CostGroup::FIVE, 2 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DOOM, }, CostGroup::FIVE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::FATE, }, CostGroup::FIVE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ATTACK, TrackedType::DOOM, }, CostGroup::FIVE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ATTACK, TrackedType::FATE, TrackedType::TREASURE, }, CostGroup::FIVE, 1 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::DURATION, }, CostGroup::FIVE, 3 },
                { { Expansion::NOCTURNE, EditionModifier::NONE }, { TrackedType::ATTACK, TrackedType::DURATION, }, CostGroup::OTHER, 1 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 2 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::TWO, 1 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 3 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::THREE, 1 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 7 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::FOUR, 1 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 6 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 2 },
                { { Expansion::RENAISSANCE, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::FIVE, 2 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 1 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::TWO, 1 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::TWO, 1 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 5 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::THREE, 1 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 4 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FOUR, 1 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::FOUR, 1 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 8 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, CostGroup::FIVE, 1 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::FIVE, 2 },
                { { Expansion::MENAGERIE, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::OTHER, 3 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::LIAISON, }, CostGroup::TWO, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::LIAISON, TrackedType::TREASURE, }, CostGroup::TWO, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 6 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, TrackedType::LIAISON, }, CostGroup::THREE, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::LIAISON, }, CostGroup::THREE, 2 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 4 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::FOUR, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::LIAISON, }, CostGroup::FOUR, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 6 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 2 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, CostGroup::FIVE, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::LIAISON, }, CostGroup::FIVE, 2 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::DURATION, TrackedType::LIAISON, TrackedType::TREASURE, }, CostGroup::FIVE, 1 },
                { { Expansion::ALLIES, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::OTHER, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::TWO, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, TrackedType::LOOT, }, CostGroup::TWO, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::DURATION, TrackedType::TREASURE, }, CostGroup::TWO, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::LOOT, TrackedType::TREASURE, }, CostGroup::TWO, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, CostGroup::THREE, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::THREE, 3 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 5 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::FOUR, 3 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::DURATION, TrackedType::TREASURE, }, CostGroup::FOUR, 3 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::FOUR, 2 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 3 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FIVE, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, CostGroup::FIVE, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, TrackedType::LOOT, }, CostGroup::FIVE, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::FIVE, 4 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::LOOT, }, CostGroup::FIVE, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::DURATION, TrackedType::TREASURE, }, CostGroup::FIVE, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::LOOT, TrackedType::TREASURE, }, CostGroup::FIVE, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::FIVE, 3 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::LOOT, TrackedType::TREASURE, }, CostGroup::OTHER, 1 },
                { { Expansion::PLUNDER, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::OTHER, 1 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::TWO, 1 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::TWO, 1 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 3 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 2 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, }, CostGroup::FOUR, 1 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::OMEN, }, CostGroup::FOUR, 3 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 6 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::OMEN, }, CostGroup::FIVE, 1 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::OMEN, }, CostGroup::FIVE, 1 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::OTHER, 2 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, CostGroup::OTHER, 1 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::OMEN, }, CostGroup::OTHER, 1 },
                { { Expansion::RISING_SUN, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::OTHER, 1 },
                { { Expansion::PROMOS, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::THREE, 1 },
                { { Expansion::PROMOS, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::THREE, 1 },
                { { Expansion::PROMOS, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FOUR, 4 },
                { { Expansion::PROMOS, EditionModifier::NONE }, { }, CostGroup::FIVE, 1 },
                { { Expansion::PROMOS, EditionModifier::NONE }, { TrackedType::ACTION, }, CostGroup::FIVE, 1 },
                { { Expansion::PROMOS, EditionModifier::NONE }, { TrackedType::TREASURE, }, CostGroup::FIVE, 1 },
                { { Expansion::PROMOS, EditionModifier::NONE }, { TrackedType::ACTION, TrackedType::DURATION, }, CostGroup::OTHER, 2 },
            }};
            return singleton;
        }

        using ExpansionEditionFilter = utils::table::EnumMask<SizedExpansion, SizedEditionModifier>;
        using CostGroupFilter = utils::table::EnumMask<SizedCostGroup>;

        struct TableQuery {
            TrackedTypeMask tracked_types_{};
            CostGroupFilter cost_group_{};
        };

        using queries_t = std::array<TableQuery, 69uz>;

        static constexpr auto amount_queries() noexcept -> const queries_t& {
            static constexpr queries_t singleton {{
                { {}, {CostGroup::TWO} },    // none_two
                { {}, {CostGroup::THREE} },    // none_three
                { {}, {CostGroup::FOUR} },    // none_four
                { {}, {CostGroup::FIVE} },    // none_five
                { {}, {CostGroup::OTHER} },    // none_other
                { {TrackedType::ACTION, }, {CostGroup::TWO} },    // action_two
                { {TrackedType::ACTION, }, {CostGroup::THREE} },    // action_three
                { {TrackedType::ACTION, }, {CostGroup::FOUR} },    // action_four
                { {TrackedType::ACTION, }, {CostGroup::FIVE} },    // action_five
                { {TrackedType::ACTION, }, {CostGroup::OTHER} },    // action_other
                { {TrackedType::ACTION, TrackedType::ATTACK, }, {CostGroup::TWO} },    // action_attack_two
                { {TrackedType::ACTION, TrackedType::ATTACK, }, {CostGroup::THREE} },    // action_attack_three
                { {TrackedType::ACTION, TrackedType::ATTACK, }, {CostGroup::FOUR} },    // action_attack_four
                { {TrackedType::ACTION, TrackedType::ATTACK, }, {CostGroup::FIVE} },    // action_attack_five
                { {TrackedType::ACTION, TrackedType::ATTACK, }, {CostGroup::OTHER} },    // action_attack_other
                { {TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DOOM, }, {CostGroup::FOUR} },    // action_attack_doom_four
                { {TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DOOM, }, {CostGroup::FIVE} },    // action_attack_doom_five
                { {TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, {CostGroup::THREE} },    // action_attack_duration_three
                { {TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, {CostGroup::FOUR} },    // action_attack_duration_four
                { {TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, {CostGroup::FIVE} },    // action_attack_duration_five
                { {TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, }, {CostGroup::OTHER} },    // action_attack_duration_other
                { {TrackedType::ACTION, TrackedType::ATTACK, TrackedType::DURATION, TrackedType::LOOT, }, {CostGroup::FIVE} },    // action_attack_duration_loot_five
                { {TrackedType::ACTION, TrackedType::ATTACK, TrackedType::LOOTER, }, {CostGroup::FOUR} },    // action_attack_looter_four
                { {TrackedType::ACTION, TrackedType::ATTACK, TrackedType::LOOTER, }, {CostGroup::FIVE} },    // action_attack_looter_five
                { {TrackedType::ACTION, TrackedType::ATTACK, TrackedType::OMEN, }, {CostGroup::FIVE} },    // action_attack_omen_five
                { {TrackedType::ACTION, TrackedType::DOOM, }, {CostGroup::THREE} },    // action_doom_three
                { {TrackedType::ACTION, TrackedType::DOOM, }, {CostGroup::FIVE} },    // action_doom_five
                { {TrackedType::ACTION, TrackedType::DURATION, }, {CostGroup::TWO} },    // action_duration_two
                { {TrackedType::ACTION, TrackedType::DURATION, }, {CostGroup::THREE} },    // action_duration_three
                { {TrackedType::ACTION, TrackedType::DURATION, }, {CostGroup::FOUR} },    // action_duration_four
                { {TrackedType::ACTION, TrackedType::DURATION, }, {CostGroup::FIVE} },    // action_duration_five
                { {TrackedType::ACTION, TrackedType::DURATION, }, {CostGroup::OTHER} },    // action_duration_other
                { {TrackedType::ACTION, TrackedType::DURATION, TrackedType::LIAISON, }, {CostGroup::THREE} },    // action_duration_liaison_three
                { {TrackedType::ACTION, TrackedType::DURATION, TrackedType::LOOT, }, {CostGroup::TWO} },    // action_duration_loot_two
                { {TrackedType::ACTION, TrackedType::FATE, }, {CostGroup::TWO} },    // action_fate_two
                { {TrackedType::ACTION, TrackedType::FATE, }, {CostGroup::THREE} },    // action_fate_three
                { {TrackedType::ACTION, TrackedType::FATE, }, {CostGroup::FOUR} },    // action_fate_four
                { {TrackedType::ACTION, TrackedType::FATE, }, {CostGroup::FIVE} },    // action_fate_five
                { {TrackedType::ACTION, TrackedType::LIAISON, }, {CostGroup::TWO} },    // action_liaison_two
                { {TrackedType::ACTION, TrackedType::LIAISON, }, {CostGroup::THREE} },    // action_liaison_three
                { {TrackedType::ACTION, TrackedType::LIAISON, }, {CostGroup::FOUR} },    // action_liaison_four
                { {TrackedType::ACTION, TrackedType::LIAISON, }, {CostGroup::FIVE} },    // action_liaison_five
                { {TrackedType::ACTION, TrackedType::LOOT, }, {CostGroup::FIVE} },    // action_loot_five
                { {TrackedType::ACTION, TrackedType::LOOTER, }, {CostGroup::FOUR} },    // action_looter_four
                { {TrackedType::ACTION, TrackedType::OMEN, }, {CostGroup::FOUR} },    // action_omen_four
                { {TrackedType::ACTION, TrackedType::OMEN, }, {CostGroup::FIVE} },    // action_omen_five
                { {TrackedType::ACTION, TrackedType::OMEN, }, {CostGroup::OTHER} },    // action_omen_other
                { {TrackedType::ACTION, TrackedType::TREASURE, }, {CostGroup::FIVE} },    // action_treasure_five
                { {TrackedType::ATTACK, TrackedType::DOOM, }, {CostGroup::FIVE} },    // attack_doom_five
                { {TrackedType::ATTACK, TrackedType::DURATION, }, {CostGroup::OTHER} },    // attack_duration_other
                { {TrackedType::ATTACK, TrackedType::FATE, TrackedType::TREASURE, }, {CostGroup::FIVE} },    // attack_fate_treasure_five
                { {TrackedType::ATTACK, TrackedType::TREASURE, }, {CostGroup::FIVE} },    // attack_treasure_five
                { {TrackedType::DURATION, }, {CostGroup::TWO} },    // duration_two
                { {TrackedType::DURATION, }, {CostGroup::THREE} },    // duration_three
                { {TrackedType::DURATION, }, {CostGroup::FIVE} },    // duration_five
                { {TrackedType::DURATION, TrackedType::LIAISON, TrackedType::TREASURE, }, {CostGroup::FIVE} },    // duration_liaison_treasure_five
                { {TrackedType::DURATION, TrackedType::TREASURE, }, {CostGroup::TWO} },    // duration_treasure_two
                { {TrackedType::DURATION, TrackedType::TREASURE, }, {CostGroup::THREE} },    // duration_treasure_three
                { {TrackedType::DURATION, TrackedType::TREASURE, }, {CostGroup::FOUR} },    // duration_treasure_four
                { {TrackedType::DURATION, TrackedType::TREASURE, }, {CostGroup::FIVE} },    // duration_treasure_five
                { {TrackedType::LIAISON, TrackedType::TREASURE, }, {CostGroup::TWO} },    // liaison_treasure_two
                { {TrackedType::LOOT, TrackedType::TREASURE, }, {CostGroup::TWO} },    // loot_treasure_two
                { {TrackedType::LOOT, TrackedType::TREASURE, }, {CostGroup::FIVE} },    // loot_treasure_five
                { {TrackedType::LOOT, TrackedType::TREASURE, }, {CostGroup::OTHER} },    // loot_treasure_other
                { {TrackedType::TREASURE, }, {CostGroup::TWO} },    // treasure_two
                { {TrackedType::TREASURE, }, {CostGroup::THREE} },    // treasure_three
                { {TrackedType::TREASURE, }, {CostGroup::FOUR} },    // treasure_four
                { {TrackedType::TREASURE, }, {CostGroup::FIVE} },    // treasure_five
                { {TrackedType::TREASURE, }, {CostGroup::OTHER} },    // treasure_other
            }};
            return singleton;
        }
    }


    namespace kingdom {
        using special_tests_t = std::array<typename ExpansionEditionFilter::enum_t, 5uz>;

        static constexpr auto special_tests() noexcept -> const special_tests_t& {
            static constexpr special_tests_t singleton {{
                { Expansion::CORNUCOPIA, EditionModifier::NONE },    // YOUNG_WITCH
                { Expansion::CORNUCOPIA_GUILDS, EditionModifier::UPDATE_PACK },    // FERRYMAN
                { Expansion::DARK_AGES, EditionModifier::NONE },    // KNIGHTS
                { Expansion::NOCTURNE, EditionModifier::NONE },    // DRUID
                { Expansion::RISING_SUN, EditionModifier::NONE },    // RIVERBOAT
            }};
            return singleton;
        }
    }



    namespace kingdom {
        enum class AmountIndex : std::size_t {
            NONE_TWO,
            NONE_THREE,
            NONE_FOUR,
            NONE_FIVE,
            NONE_OTHER,
            ACTION_TWO,
            ACTION_THREE,
            ACTION_FOUR,
            ACTION_FIVE,
            ACTION_OTHER,
            ACTION_ATTACK_TWO,
            ACTION_ATTACK_THREE,
            ACTION_ATTACK_FOUR,
            ACTION_ATTACK_FIVE,
            ACTION_ATTACK_OTHER,
            ACTION_ATTACK_DOOM_FOUR,
            ACTION_ATTACK_DOOM_FIVE,
            ACTION_ATTACK_DURATION_THREE,
            ACTION_ATTACK_DURATION_FOUR,
            ACTION_ATTACK_DURATION_FIVE,
            ACTION_ATTACK_DURATION_OTHER,
            ACTION_ATTACK_DURATION_LOOT_FIVE,
            ACTION_ATTACK_LOOTER_FOUR,
            ACTION_ATTACK_LOOTER_FIVE,
            ACTION_ATTACK_OMEN_FIVE,
            ACTION_DOOM_THREE,
            ACTION_DOOM_FIVE,
            ACTION_DURATION_TWO,
            ACTION_DURATION_THREE,
            ACTION_DURATION_FOUR,
            ACTION_DURATION_FIVE,
            ACTION_DURATION_OTHER,
            ACTION_DURATION_LIAISON_THREE,
            ACTION_DURATION_LOOT_TWO,
            ACTION_FATE_TWO,
            ACTION_FATE_THREE,
            ACTION_FATE_FOUR,
            ACTION_FATE_FIVE,
            ACTION_LIAISON_TWO,
            ACTION_LIAISON_THREE,
            ACTION_LIAISON_FOUR,
            ACTION_LIAISON_FIVE,
            ACTION_LOOT_FIVE,
            ACTION_LOOTER_FOUR,
            ACTION_OMEN_FOUR,
            ACTION_OMEN_FIVE,
            ACTION_OMEN_OTHER,
            ACTION_TREASURE_FIVE,
            ATTACK_DOOM_FIVE,
            ATTACK_DURATION_OTHER,
            ATTACK_FATE_TREASURE_FIVE,
            ATTACK_TREASURE_FIVE,
            DURATION_TWO,
            DURATION_THREE,
            DURATION_FIVE,
            DURATION_LIAISON_TREASURE_FIVE,
            DURATION_TREASURE_TWO,
            DURATION_TREASURE_THREE,
            DURATION_TREASURE_FOUR,
            DURATION_TREASURE_FIVE,
            LIAISON_TREASURE_TWO,
            LOOT_TREASURE_TWO,
            LOOT_TREASURE_FIVE,
            LOOT_TREASURE_OTHER,
            TREASURE_TWO,
            TREASURE_THREE,
            TREASURE_FOUR,
            TREASURE_FIVE,
            TREASURE_OTHER,
        };
    }


}