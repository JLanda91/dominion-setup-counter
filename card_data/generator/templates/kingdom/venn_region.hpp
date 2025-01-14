#pragma once

#include <array>
#include <algorithm>

#include "card_type_tally.hpp"
#include "membership_mask.hpp"

namespace card_data::kingdom {
    struct VennRegion {
        MembershipMask membership_mask;
        CardTypeTally card_type_tally;
    };

    /*[[[cog
    from generator.kingdom.venn_region import instance
    instance.generate_singleton()
    ]]]*/
    //[[[end]]]
}