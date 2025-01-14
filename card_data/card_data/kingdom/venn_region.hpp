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

    inline constexpr auto kKingdomVennRegionsSize = 20uz;
    using kingdom_venn_region_array_t = std::array<VennRegion, kKingdomVennRegionsSize>;

    inline constexpr kingdom_venn_region_array_t kKingdomVennRegions {{
    	{ MembershipMask::FromUnsigned(0u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8} },
    	{ MembershipMask::FromUnsigned(1u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2} },
    	{ MembershipMask::FromUnsigned(2u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1} },
    	{ MembershipMask::FromUnsigned(8u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6} },
    	{ MembershipMask::FromUnsigned(9u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5} },
    	{ MembershipMask::FromUnsigned(64u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 26} },
    	{ MembershipMask::FromUnsigned(65u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 5} },
    	{ MembershipMask::FromUnsigned(66u), CardTypeTally{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2} },
    	{ MembershipMask::FromUnsigned(72u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9} },
    	{ MembershipMask::FromUnsigned(73u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5} },
    	{ MembershipMask::FromUnsigned(96u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 43} },
    	{ MembershipMask::FromUnsigned(98u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 12} },
    	{ MembershipMask::FromUnsigned(101u), CardTypeTally{0, 0, 1, 0, 0, 0, 2, 0, 1, 0, 1, 37} },
    	{ MembershipMask::FromUnsigned(103u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2} },
    	{ MembershipMask::FromUnsigned(104u), CardTypeTally{0, 0, 0, 0, 0, 1, 2, 0, 1, 3, 0, 93} },
    	{ MembershipMask::FromUnsigned(106u), CardTypeTally{1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 13} },
    	{ MembershipMask::FromUnsigned(109u), CardTypeTally{0, 0, 0, 0, 1, 0, 1, 1, 3, 0, 0, 65} },
    	{ MembershipMask::FromUnsigned(111u), CardTypeTally{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8} },
    	{ MembershipMask::FromUnsigned(112u), CardTypeTally{0, 0, 0, 1, 0, 0, 1, 1, 2, 1, 1, 88} },
    	{ MembershipMask::FromUnsigned(114u), CardTypeTally{0, 1, 0, 0, 0, 1, 0, 2, 0, 1, 0, 26} },
    }};

    static_assert(std::ranges::fold_left(kKingdomVennRegions, 0u, [](auto acc, const auto& elem){
    	return acc + card_type_total(elem.card_type_tally);
    }) == 498u, "Card count mismatch");
}