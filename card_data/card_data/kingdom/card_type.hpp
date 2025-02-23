#pragma once

#include <cstdint>
#include <array>
#include <string_view>
#include <string>

#include "utils/result_type.hpp"

namespace card_data::kingdom {
    enum class CardType : uint8_t {
        YoungWitch = 0,
        Knights = 1,
        Druid = 2,
        Ferryman = 3,
        Riverboat = 4,
        Looter = 5,
        Fate = 6,
        Doom = 7,
        Liaison = 8,
        Omen = 9,
        Loot = 10,
        None = 11,
    };

    inline constexpr std::size_t kNumCardTypes = 12uz;

    constexpr CardType next_card_type (CardType ct) noexcept {
        return static_cast<CardType>(std::to_underlying(ct) + 1);
    };

    template<CardType C>
    constexpr auto column_factor() noexcept {
        if constexpr (C == CardType::YoungWitch) {
            return 1ul; // does not add a pile
        } else if constexpr (C == CardType::Knights) {
            return 3628800; // 10! ways to shuffle knights
        } else if constexpr (C == CardType::Druid) {
            return 79833600u; // 12!/3! ways to shuffle boons and draw top 3
        } else if constexpr (C == CardType::Ferryman) {
            return 1u; // does not add a pile
        } else if constexpr (C == CardType::Riverboat) {
            return 1u; // does not add a pile
        } else if constexpr (C == CardType::Looter) {
            return 9765625u; // 5^10 ways to create a ruins pile for 2 players
        } else if constexpr (C == CardType::Fate) {
            return 1ul; // Fate factor can only be assigned as soon as we know no druid is in play
        } else if constexpr (C == CardType::Doom) {
            return 479001600u; // 12! ways to shuffle hexes
        } else if constexpr (C == CardType::Liaison) {
            return 23u; // pick one of 23 allies
        } else if constexpr (C == CardType::Omen) {
            return 1u; // Prophecies are handled differently
        } else if constexpr (C == CardType::Loot) {
            return 0x1A27EC6E1F2D0CBBEBBAA800_cppui128;
        } else {
            return 1u; // None adds nothing
        }
    };

    template<CardType ... C>
    struct card_type_sequence {
        static constexpr std::size_t size() noexcept {
            return sizeof...(C);
        }
    };

    using make_card_type_sequence = card_type_sequence<CardType::YoungWitch, CardType::Knights, CardType::Druid, CardType::Ferryman, CardType::Riverboat, CardType::Looter, CardType::Fate, CardType::Doom, CardType::Liaison, CardType::Omen, CardType::Loot, CardType::None>;
}

template<>
struct fmt::formatter<card_data::kingdom::CardType> : formatter<std::string_view> {
private:
    static constexpr std::array<std::string_view, card_data::kingdom::kNumCardTypes> kCardTypeNames = { "YoungWitch", "Knights", "Druid", "Ferryman", "Riverboat", "Looter", "Fate", "Doom", "Liaison", "Omen", "Loot", "None" };

public:
    constexpr auto format(const card_data::kingdom::CardType& obj, fmt::format_context& ctx) const {
        return formatter<std::string_view>::format(kCardTypeNames.at(std::to_underlying(obj)), ctx);
    }
};
