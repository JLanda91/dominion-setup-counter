#pragma once

#include "card_data/kingdom/card_type.hpp"

namespace card_data {
    struct CombinationModifiers {
        bool has_looter: 1;
        bool has_fate: 1;
        bool has_doom: 1;
        bool has_liaison: 1;
        bool has_omen: 1;
        bool has_approaching_army: 1;
        bool has_loot: 1;
        bool has_young_witch: 1;
        bool has_knights: 1;
        bool has_druid: 1;
        bool has_ferryman: 1;
        bool has_riverboat: 1;
        bool has_obelisk: 1;
        bool has_way_of_the_mouse: 1;

        template<kingdom::CardType C>
        constexpr auto get_from_kingdom_column() const noexcept {
            if constexpr (C == kingdom::CardType::YoungWitch) {
                return has_young_witch;
            } else if constexpr (C == kingdom::CardType::Knights) {
                return has_knights;
            } else if constexpr (C == kingdom::CardType::Druid) {
                return has_druid;
            } else if constexpr (C == kingdom::CardType::Ferryman) {
                return has_ferryman;
            } else if constexpr (C == kingdom::CardType::Riverboat) {
                return has_riverboat;
            } else if constexpr (C == kingdom::CardType::Looter) {
                return has_looter;
            } else if constexpr (C == kingdom::CardType::Fate) {
                return has_fate;
            } else if constexpr (C == kingdom::CardType::Doom) {
                return has_doom;
            } else if constexpr (C == kingdom::CardType::Liaison) {
                return has_liaison;
            } else if constexpr (C == kingdom::CardType::Omen) {
                return has_omen;
            } else if constexpr (C == kingdom::CardType::Loot) {
                return has_loot;
            } else {
                return false;
            }
        };

        template<kingdom::CardType C, bool AddApproachingArmy>
        constexpr auto with_set_column() const noexcept {
            static_assert((C == kingdom::CardType::Omen) || !AddApproachingArmy);

            auto result = *this;
            if constexpr (C == kingdom::CardType::YoungWitch) {
                result.has_young_witch = true;
            } else if constexpr (C == kingdom::CardType::Knights) {
                result.has_knights = true;
            } else if constexpr (C == kingdom::CardType::Druid) {
                result.has_druid = true;
            } else if constexpr (C == kingdom::CardType::Ferryman) {
                result.has_ferryman = true;
            } else if constexpr (C == kingdom::CardType::Riverboat) {
                result.has_riverboat = true;
            } else if constexpr (C == kingdom::CardType::Looter) {
                result.has_looter = true;
            } else if constexpr (C == kingdom::CardType::Fate) {
                result.has_fate = true;
            } else if constexpr (C == kingdom::CardType::Doom) {
                result.has_doom = true;
            } else if constexpr (C == kingdom::CardType::Liaison) {
                result.has_liaison = true;
            } else if constexpr (C == kingdom::CardType::Omen) {
                result.has_omen = true;
                if constexpr (AddApproachingArmy) {
                    result.has_approaching_army = true;
                }
            } else if constexpr (C == kingdom::CardType::Loot) {
                result.has_loot = true;
            }
            return result;
        };

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

    };
}
