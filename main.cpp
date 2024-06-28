#include <print>
#include <chrono>

#include "config.hpp"
#include "constrained_product_generator.hpp"

auto identity_with_map(auto&& mapper) {
    return [&mapper](const auto& elem) {
        return std::make_pair(elem, mapper(elem));
    };
}

result_t non_card_factor(const non_card_type_amounts_t& non_card_combination) {
    return math_utils::binomial<result_t>(std::get<0>(kNonCardAmounts), std::get<0>(non_card_combination));
}

using non_card_amount_factor_t = std::pair<non_card_type_amounts_t, result_t>;

const auto& non_card_factor_vector() {
    static const auto result = []() {
        std::vector<non_card_amount_factor_t> result{};
        for (const auto& [tup, f] : generators::constrained_product<generators::Constraint::LE>(kNonCardAmounts, 2u) |
                                    std::views::transform(identity_with_map(non_card_factor))) {
            result.emplace_back(std::piecewise_construct, std::forward_as_tuple(tup), std::forward_as_tuple(f));
        }
        return result;
    }();
    return result;
}

result_pair_t kingdom_card_factor(const kingdom_card_type_amounts_t& kingdom_card_combination) {
    const auto& [
            action_low,
            other_low,
            action_liaison_low,
            other_liaison_low,
            action_fate_low,
            action_doom_low,
            druid,
            action_high,
            other_high,
            action_liaison_high,
            action_looter_high,
            action_fate_high,
            other_fate_high,
            action_doom_high,
            other_doom_high,
            knights
    ] = kingdom_card_combination;

    const auto binomial_product = math_utils::binomial_product<result_t>(kKingdomCardAmounts, kingdom_card_combination);

    const auto liaison_total = (result_t) action_liaison_low +
                               other_liaison_low +
                               action_liaison_high;

    const auto fate_total = (result_t) action_fate_low +
                            action_fate_high +
                            other_fate_high;

    const auto doom_total = (result_t) action_doom_low +
                            action_doom_high +
                            other_doom_high;

    const auto result_unordered = binomial_product *
            (druid ? kDruidBoonCombinations : 1u) *
            (liaison_total ? kLiaisonAllyCombinations : 1u);

    const auto result_ordered = result_unordered
        * (fate_total > 0 ? (druid ? kBoonShufflesDruid : kBoonShufflesNoDruid) : 1u)
        * (doom_total > 0 ? kHexShuffles : 1u)
        * (knights > 0 ? kKnightShuffles : 1u)
        * (action_looter_high > 0 ? kRuinsCombinations : 1u);

    return {result_unordered, result_ordered};
}

result_t joint_factor(const kingdom_card_type_amounts_t& kingdom_card_combination
                      , const non_card_type_amounts_t& non_card_combination
                      ) {
    const auto& [
            action_low,
            other_low,
            action_liaison_low,
            other_liaison_low,
            action_fate_low,
            action_doom_low,
            druid,
            action_high,
            other_high,
            action_liaison_high,
            action_looter_high,
            action_fate_high,
            other_fate_high,
            action_doom_high,
            other_doom_high,
            knights
    ] = kingdom_card_combination;

    const auto& [
            regular_landscapes,
            obelisk,
            way_of_the_mouse
    ] = non_card_combination;

    result_t result = 1u;

    const auto action_low_total = (result_t) action_low +
           action_liaison_low +
           action_fate_low +
           action_doom_low +
           druid;

    const auto low_total = (result_t) action_low_total +
        other_low +
        other_liaison_low;

    const auto action_high_total = (result_t) action_high +
                        action_liaison_high +
                        action_fate_high +
                        action_looter_high +
                        action_doom_high +
                        knights;

    const auto obelisk_choices = action_low_total + action_high_total;

    if (way_of_the_mouse) {
        result *= kActionLowMax - action_low_total;
    }

    if (obelisk && obelisk_choices){
        result *= obelisk_choices +                  // No young witch: only obelisk chooses
                  low_total * (obelisk_choices + 1); // With young witch: obelisk can also choose the young witch, and young witch also chooses
    } else {
        result *= 1 +           // No young witch
                  low_total;    // Young witch
    }

    return result;
}

void calculate_total(){
    const auto t1 = std::chrono::steady_clock::now();
    auto unordered_total = (result_t) 0u;
    auto ordered_total = (result_t) 0u;

    for(const auto& kingdom_card_amounts : generators::constrained_product<generators::Constraint::EQ>(kKingdomCardAmounts, 10u)){
        auto incr = (result_t) 0u;
        auto incr2 = std::ranges::fold_left(non_card_factor_vector() | std::views::transform([&kingdom_card_amounts](const auto& pair) -> result_t {
            const auto& [non_card_amounts, non_card_multiplier] = pair;
            return non_card_multiplier * joint_factor(kingdom_card_amounts, non_card_amounts);}
        ), result_t{}, std::plus{});

        const auto [kingdom_card_multiplier_unordered,kingdom_card_multiplier_ordered]  = kingdom_card_factor(kingdom_card_amounts);
        unordered_total += incr2 * kingdom_card_multiplier_unordered;
        ordered_total += incr2 * kingdom_card_multiplier_ordered;
    }

    const auto t2 = std::chrono::steady_clock::now();
    std::println("Total unique kingdoms:");
    std::println("Not considering deck orderings: {}", unordered_total);
    std::println("    Considering deck orderings: {}", ordered_total);
    std::println("Elapsed time: {:.3f} s", std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()/1e3);
}

int main() {
    calculate_total();

}