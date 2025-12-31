#include <fmt/format.h>

#include <string>
#include <string_view>
#include <bitset>
#include <utility>
#include <array>

#include "../data_definitions/kingdom.hpp"
#include "../data_definitions/equivalence_class.cuh"
#include "kingdom/venn_diagram_region_mask.cuh"

#include "extra_setup/card.cuh"
#include "extra_setup/effective_card_type_mask.cuh"
#include "extra_setup/state.hpp"

template<>
struct fmt::formatter<card_data::kingdom::EffectiveCardType> : fmt::formatter<std::string_view> {
private:
    static constexpr std::array<std::string_view, card_data::kingdom::kNumEffectiveCardTypes> kCardTypeNames = { "YoungWitch", "Knights", "Druid", "Ferryman", "Riverboat", "Looter", "Fate", "Doom", "Liaison", "Omen", "Loot", "None" };

public:
    auto format(const card_data::kingdom::EffectiveCardType& obj, fmt::format_context& ctx) const {
        return formatter<std::string_view>::format(kCardTypeNames.at( static_cast<std::underlying_type_t<card_data::kingdom::EffectiveCardType>>(obj)), ctx);
    }
};

template<>
struct fmt::formatter<card_data::kingdom::VennDiagramRegionMask> : fmt::formatter<std::string> {
    auto format(const card_data::kingdom::VennDiagramRegionMask& obj, fmt::format_context& ctx) const {
        return fmt::formatter<std::string>::format(std::bitset<card_data::kingdom::kNumVennDiagramRegions>{card_data::kingdom::VennDiagramRegionMask::ToUnsigned(obj)}.to_string(), ctx);
    }
};

template<>
struct fmt::formatter<card_data::kingdom::EquivalenceClass> : fmt::formatter<std::string> {
    auto format(const card_data::kingdom::EquivalenceClass& obj, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(),
                              "EquivalenceClass({}, {}, {}, {}",
                              obj.effective_card_type, obj.venn_diagram_region_mask, obj.size, obj.num_action_or_treasure);
    }
};

template<>
struct fmt::formatter<card_data::extra_setup::Card> : fmt::formatter<std::string> {
    static constexpr std::array<std::string_view, 6> kCardTypeStrings = {
        "YoungWitch",
        "ApproachingArmy",
        "WayOfTheMouse",
        "Ferryman",
        "Riverboat",
        "Obelisk"
    };

    auto format(const card_data::extra_setup::Card& obj, fmt::format_context& ctx) const {
        return formatter<std::string>::format(kCardTypeStrings[static_cast<uint8_t>(obj)], ctx);
    }
};
