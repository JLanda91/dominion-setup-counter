#pragma once

#include "extra_setup/card.cuh"
#include "extra_setup/effective_card_type_mask.cuh"
#include "../data_definitions/equivalence_class.cuh"
#include "kingdom/venn_diagram_region_mask.cuh"

namespace card_data {
    __host__ __device__
    constexpr bool is_venn_region_for_extra_setup_card(kingdom::VennDiagramRegionMask vdrm, extra_setup::Card esc)
    {
        if (esc == extra_setup::Card::YoungWitch) return vdrm.is_young_witch_region;
        if (esc == extra_setup::Card::ApproachingArmy) return vdrm.is_approaching_army_region;
        if (esc == extra_setup::Card::WayOfTheMouse) return vdrm.is_way_of_the_mouse_region;
        if (esc == extra_setup::Card::Ferryman) return vdrm.is_ferryman_region;
        if (esc == extra_setup::Card::Riverboat) return vdrm.is_riverboat_region;
        if (esc == extra_setup::Card::Obelisk) return vdrm.is_obelisk_region;
        return false;
    }

    template<size_t Size> __host__ __device__
    extra_setup::EffectiveCardTypeMask ectm_from_ecd(
        const kingdom::EquivalenceClass* eca,
        const kingdom::EquivalenceClassDistribution<Size>& ecd)
    {
        extra_setup::EffectiveCardTypeMask result {};
        for (size_t i = 0; i < Size; i++) {
            const bool has_type = ecd[i] > 0;
            switch (eca[i].effective_card_type) {
                case kingdom::EffectiveCardType::YoungWitch:
                    result.has_young_witch |= has_type; break;
                case kingdom::EffectiveCardType::Knights:
                    result.has_knights |= has_type; break;
                case kingdom::EffectiveCardType::Druid:
                    result.has_druid |= has_type; break;
                case kingdom::EffectiveCardType::Ferryman:
                    result.has_ferryman |= has_type; break;
                case kingdom::EffectiveCardType::Riverboat:
                    result.has_riverboat |= has_type; break;
                case kingdom::EffectiveCardType::Looter:
                    result.has_looter |= has_type; break;
                case kingdom::EffectiveCardType::Fate:
                    result.has_fate |= has_type; break;
                case kingdom::EffectiveCardType::Doom:
                    result.has_doom |= has_type; break;
                case kingdom::EffectiveCardType::Liaison:
                    result.has_liaison |= has_type; break;
                case kingdom::EffectiveCardType::Omen:
                    result.has_omen |= has_type; break;
                case kingdom::EffectiveCardType::Loot:
                    result.has_loot |= has_type; break;
                default:
                    break;
            }
        }
        return result;
    }
}
