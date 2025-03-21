#pragma once

#include "card_data/kingdom/card_type.hpp"

namespace card_data {
    struct CombinationModifiers {
        bool has_young_witch: 1;
        bool has_knights: 1;
        bool has_druid: 1;
        bool has_ferryman: 1;
        bool has_riverboat: 1;
        bool has_looter: 1;
        bool has_fate: 1;
        bool has_doom: 1;
        bool has_liaison: 1;
        bool has_omen: 1;
        bool has_loot: 1;
        bool has_obelisk: 1;
        bool has_way_of_the_mouse: 1;

        constexpr auto operator==(const CombinationModifiers& other) const noexcept -> bool = default;

        constexpr auto get_from_kingdom_column(kingdom::CardType kingdom_card_type) const noexcept -> bool {
            if (kingdom_card_type == kingdom::CardType::YoungWitch) {
                return has_young_witch;
            }
            if (kingdom_card_type == kingdom::CardType::Knights) {
                return has_knights;
            }
            if (kingdom_card_type == kingdom::CardType::Druid) {
                return has_druid;
            }
            if (kingdom_card_type == kingdom::CardType::Ferryman) {
                return has_ferryman;
            }
            if (kingdom_card_type == kingdom::CardType::Riverboat) {
                return has_riverboat;
            }
            if (kingdom_card_type == kingdom::CardType::Looter) {
                return has_looter;
            }
            if (kingdom_card_type == kingdom::CardType::Fate) {
                return has_fate;
            }
            if (kingdom_card_type == kingdom::CardType::Doom) {
                return has_doom;
            }
            if (kingdom_card_type == kingdom::CardType::Liaison) {
                return has_liaison;
            }
            if (kingdom_card_type == kingdom::CardType::Omen) {
                return has_omen;
            }
            if (kingdom_card_type == kingdom::CardType::Loot) {
                return has_loot;
            }
            return false;
        }

        constexpr void set_kingdom_column(kingdom::CardType kingdom_card_type) noexcept {
            if (kingdom_card_type == kingdom::CardType::YoungWitch) {
                has_young_witch = true;
            }
            if (kingdom_card_type == kingdom::CardType::Knights) {
                has_knights = true;
            }
            if (kingdom_card_type == kingdom::CardType::Druid) {
                has_druid = true;
            }
            if (kingdom_card_type == kingdom::CardType::Ferryman) {
                has_ferryman = true;
            }
            if (kingdom_card_type == kingdom::CardType::Riverboat) {
                has_riverboat = true;
            }
            if (kingdom_card_type == kingdom::CardType::Looter) {
                has_looter = true;
            }
            if (kingdom_card_type == kingdom::CardType::Fate) {
                has_fate = true;
            }
            if (kingdom_card_type == kingdom::CardType::Doom) {
                has_doom = true;
            }
            if (kingdom_card_type == kingdom::CardType::Liaison) {
                has_liaison = true;
            }
            if (kingdom_card_type == kingdom::CardType::Omen) {
                has_omen = true;
            }
            if (kingdom_card_type == kingdom::CardType::Loot) {
                has_loot = true;
            }
        }

        constexpr auto loot_states() const noexcept {
            std::array<CombinationModifiers, 4> result{};
            result.fill(*this);

            result[1].has_way_of_the_mouse = true;
            result[2].has_obelisk = true;
            result[3].has_obelisk = true;
            result[3].has_way_of_the_mouse = true;

            return result;
        };

        constexpr auto non_loot_states() const noexcept {
            std::array<CombinationModifiers, 7> result{};
            result.fill(*this);

            result[1].has_way_of_the_mouse = true;
            result[2].has_obelisk = true;
            result[3].has_obelisk = true;
            result[3].has_way_of_the_mouse = true;
            result[4].has_loot = true;
            result[5].has_loot = true;
            result[5].has_way_of_the_mouse = true;
            result[6].has_loot = true;
            result[6].has_obelisk = true;

            return result;
        };

        constexpr std::size_t to_pile_mask() const noexcept {
            return
                (has_knights << 0u) |
                (has_druid << 1u) |
                (has_looter << 2u) |
                (has_fate << 3u) |
                (has_doom << 4u) |
                (has_liaison << 5u) |
                (has_loot << 6u);
        }

    };
}
