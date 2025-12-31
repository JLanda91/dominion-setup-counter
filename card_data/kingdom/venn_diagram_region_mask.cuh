#pragma once

#include <cstdint>

namespace card_data::kingdom {

struct VennDiagramRegionMask {
    bool is_young_witch_region : 1;
    bool is_approaching_army_region : 1;
    bool is_way_of_the_mouse_region : 1;
    bool is_ferryman_region : 1;
    bool is_riverboat_region : 1;
    bool is_obelisk_region : 1;

    static constexpr VennDiagramRegionMask FromUnsigned(std::uint8_t bit_pack) noexcept {
        return VennDiagramRegionMask{
        	.is_young_witch_region = (bit_pack & (1u << 0u)) > 0,
        	.is_approaching_army_region = (bit_pack & (1u << 1u)) > 0,
        	.is_way_of_the_mouse_region = (bit_pack & (1u << 2u)) > 0,
        	.is_ferryman_region = (bit_pack & (1u << 3u)) > 0,
        	.is_riverboat_region = (bit_pack & (1u << 4u)) > 0,
        	.is_obelisk_region = (bit_pack & (1u << 5u)) > 0
        };
    }

    static constexpr std::uint8_t ToUnsigned(const VennDiagramRegionMask& m) noexcept {
        return
        	(m.is_young_witch_region << 0uz) +
        	(m.is_approaching_army_region << 1uz) +
        	(m.is_way_of_the_mouse_region << 2uz) +
        	(m.is_ferryman_region << 3uz) +
        	(m.is_riverboat_region << 4uz) +
        	(m.is_obelisk_region << 5uz);
    }
};

inline constexpr uint8_t kNumVennDiagramRegions = 6;

}


