#pragma once

namespace card_data::extra_setup {
    struct EffectiveCardTypeMask {
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

        __host__ bool operator==(const EffectiveCardTypeMask& rhs) const = default;
    };


}
