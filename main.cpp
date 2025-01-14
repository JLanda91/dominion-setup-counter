#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <sstream>
#include <vector>

#include <card_data/kingdom/venn_region.hpp>

template<>
struct fmt::formatter<card_data::kingdom::MembershipMask> : formatter<uint8_t> {
    auto format(const card_data::kingdom::MembershipMask& obj, fmt::format_context& ctx) const {
        return formatter<uint8_t>::format(card_data::kingdom::MembershipMask::ToUnsigned(obj), ctx);
    }
};

template<>
struct fmt::formatter<card_data::kingdom::CardTypeTally> : formatter<std::string> {
    auto format(const card_data::kingdom::CardTypeTally& obj, fmt::format_context& ctx) const {
        auto member_stream = [&obj]<std::size_t ... I>(std::ostringstream& out, std::index_sequence<I...>){
            (..., (out << card_data::kingdom::card_type_name<I>() << '=' << fmt::format("{:{}}{}", card_data::kingdom::card_type_amount<I>(obj), (I == card_data::kingdom::kNumCardTypes - 1 ? 2 : 1),(I == card_data::kingdom::kNumCardTypes - 1 ? "" : ", "))));
        };
        std::ostringstream oss{};
        oss << "CardTypeTally(";
        member_stream(oss, std::make_index_sequence<card_data::kingdom::kNumCardTypes>{});
        oss << ')';
        return formatter<std::string>::format(oss.str(), ctx);
    }
};

template<>
struct fmt::formatter<card_data::kingdom::VennRegion> : formatter<std::string> {
    auto format(const card_data::kingdom::VennRegion& obj, fmt::format_context& ctx) const {
        std::ostringstream oss{};
        oss << "VennRegion(membership_mask=" << fmt::format("{:0{}b}", obj.membership_mask, card_data::kingdom::kNumMembershipRegions) << ", " << fmt::format("{}", obj.card_type_tally) << ')';
        return formatter<std::string>::format(oss.str(), ctx);
    }
};

using CardTypeTallyVector = std::vector<card_data::kingdom::CardTypeTally>;

auto sub_tallies(const card_data::kingdom::CardTypeTally& ctt) -> CardTypeTallyVector{
    auto impl = [&ctt]<std::size_t I>(this auto&& self, CardTypeTallyVector& result, card_data::kingdom::CardTypeTally& card_type_tally, uint8_t total) -> void {
        if constexpr(I == card_data::kingdom::kNumCardTypes){
            result.push_back(card_type_tally);
        } else {
            for(uint8_t k = 0; k <= std::min(total, card_data::kingdom::card_type_amount<I>(ctt)); ++k){
                card_data::kingdom::card_type_amount<I>(card_type_tally) = k;
                self.template operator()<I + 1>(result, card_type_tally, static_cast<uint8_t>(total - k));
            }
        }
    };
    std::vector<card_data::kingdom::CardTypeTally> result{};
    card_data::kingdom::CardTypeTally card_type_tally{};
    const auto total = std::min(card_data::kingdom::card_type_total(ctt), static_cast<uint8_t>(10u));
    impl.template operator()<0>(result, card_type_tally, total);
    std::ranges::sort(result, std::less{}, card_data::kingdom::card_type_total);
    return result;
}

int main() {
//    fmt::println("{}", fmt::join(card_data::kingdom::kKingdomVennRegions, "\n"));
//    fmt::println("");
//    fmt::println("{}", std::ranges::fold_left(card_data::kingdom::kKingdomVennRegions, 0u, [](auto acc, const auto& venn_region){
//        return acc + std::size(sub_tallies(venn_region.card_type_tally));
//    }));

    for (const auto& venn_region : card_data::kingdom::kKingdomVennRegions){
        fmt::println("{}\tsubtallies: {:>3}", venn_region, std::size(sub_tallies(venn_region.card_type_tally)));
    }
}