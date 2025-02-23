// GENERATED CODE - DO NOT MODIFY

#pragma once

#include <cstdint>
#include <bitset>
#include <fmt/format.h>

namespace card_data::kingdom {
    struct MembershipMask {
        bool is_young_witch_region : 1;
        bool is_approaching_army_region : 1;
        bool is_way_of_the_mouse_region : 1;
        bool is_ferryman_region : 1;
        bool is_riverboat_region : 1;
        bool is_obelisk_region : 1;
        bool is_action_or_treasure_region : 1;

        static constexpr MembershipMask FromUnsigned(uint8_t bit_pack) noexcept {
        	return MembershipMask{
        		.is_young_witch_region = (bit_pack & (1u << 0u)) > 0,
        		.is_approaching_army_region = (bit_pack & (1u << 1u)) > 0,
        		.is_way_of_the_mouse_region = (bit_pack & (1u << 2u)) > 0,
        		.is_ferryman_region = (bit_pack & (1u << 3u)) > 0,
        		.is_riverboat_region = (bit_pack & (1u << 4u)) > 0,
        		.is_obelisk_region = (bit_pack & (1u << 5u)) > 0,
        		.is_action_or_treasure_region = (bit_pack & (1u << 6u)) > 0
        	};
        }

        static constexpr uint8_t ToUnsigned(const MembershipMask& m) noexcept {
        	return
        		(m.is_young_witch_region << 0uz) +
        		(m.is_approaching_army_region << 1uz) +
        		(m.is_way_of_the_mouse_region << 2uz) +
        		(m.is_ferryman_region << 3uz) +
        		(m.is_riverboat_region << 4uz) +
        		(m.is_obelisk_region << 5uz) +
        		(m.is_action_or_treasure_region << 6uz) ;
        }
    };

    inline constexpr std::size_t kNumMembershipRegions = 7uz;

}

template<>
struct fmt::formatter<card_data::kingdom::MembershipMask> : formatter<std::string> {
    auto format(const card_data::kingdom::MembershipMask& obj, fmt::format_context& ctx) const {
        return formatter<std::string>::format(std::bitset<card_data::kingdom::kNumMembershipRegions>{card_data::kingdom::MembershipMask::ToUnsigned(obj)}.to_string(), ctx);
    }
};
