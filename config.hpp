#pragma once

#include <boost/multiprecision/integer.hpp>

#include "tuple_utils.hpp"
#include "math_utils.hpp"

using result_t = boost::multiprecision::uint256_t;
using result_pair_t = std::pair<result_t, result_t>;
using coefficient_t = uint32_t;

template<size_t N>
using coefficient_tuple_t = typename tuple_utils::repeated_tuple<coefficient_t, N>::type;

template<typename T>
concept CoefficientTuple = tuple_utils::RepeatedTuple<T, coefficient_t>;

/*
 * KINGDOM CARD TYPES
 * =====================================================================================================================
 */
inline constexpr coefficient_t kActionLow = 98;
inline constexpr coefficient_t kOtherLow = 16;
inline constexpr coefficient_t kActionLiaisonLow = 4;
inline constexpr coefficient_t kOtherLiaisonLow = 1;
inline constexpr coefficient_t kActionFateLow = 3;
inline constexpr coefficient_t kActionDoomLow = 1;
inline constexpr coefficient_t kDruid = 1;
inline constexpr coefficient_t kActionHigh = 246;
inline constexpr coefficient_t kOtherHigh = 37;
inline constexpr coefficient_t kActionLiaisonHigh = 4;
inline constexpr coefficient_t kActionLooterHigh = 3;
inline constexpr coefficient_t kActionFateHigh = 3;
inline constexpr coefficient_t kOtherFateHigh = 1;
inline constexpr coefficient_t kActionDoomHigh = 4;
inline constexpr coefficient_t kOtherDoomHigh = 1;
inline constexpr coefficient_t kKnight = 1;

// should be equal to the number of constants in the above list
inline constexpr coefficient_t kKingdomCardTypes = 16;

using kingdom_card_type_amounts_t = coefficient_tuple_t<kKingdomCardTypes>;
inline constexpr kingdom_card_type_amounts_t kKingdomCardAmounts{
        kActionLow,
        kOtherLow,
        kActionLiaisonLow,
        kOtherLiaisonLow,
        kActionFateLow,
        kActionDoomLow,
        kDruid,
        kActionHigh,
        kOtherHigh,
        kActionLiaisonHigh,
        kActionLooterHigh,
        kActionFateHigh,
        kOtherFateHigh,
        kActionDoomHigh,
        kOtherDoomHigh,
        kKnight
};

inline constexpr result_t kActionLowMax = kActionLow +
                                          kActionLiaisonLow +
                                          kActionFateLow +
                                          kActionDoomLow +
                                          kDruid;

/*
 * NON-CARD TYPES
 * =====================================================================================================================
 */

// landscapes
inline constexpr coefficient_t kAllies      = 23;
inline constexpr coefficient_t kArtifacts   = 5;
inline constexpr coefficient_t kBoons       = 12;
inline constexpr coefficient_t kEvents      = 54;
inline constexpr coefficient_t kHexes       = 12;
inline constexpr coefficient_t kLandmarks   = 21;
inline constexpr coefficient_t kProjects    = 20;
inline constexpr coefficient_t kStates      = 5;
inline constexpr coefficient_t kWays        = 20;

// landscapes with special behavior
inline constexpr coefficient_t kObelisk         = 1;
inline constexpr coefficient_t kWayOfTheMouse   = 1;

// should be equal to the number of special behavior landscapes above
inline constexpr coefficient_t kNumSpecialLandscapes    = 2;

inline constexpr coefficient_t kNumStarterLandscapes = kEvents + kLandmarks + kProjects + kWays;
static_assert(kNumStarterLandscapes == 115u);

using non_card_type_amounts_t = coefficient_tuple_t<kNumSpecialLandscapes + 1>;

inline constexpr non_card_type_amounts_t kNonCardAmounts{
        kNumStarterLandscapes - kNumSpecialLandscapes,
        kObelisk,
        kWayOfTheMouse,
};

inline constexpr coefficient_t kNonCardsFactor = (1 << kNumSpecialLandscapes) + (1u + kNumSpecialLandscapes)*(kNumStarterLandscapes - kNumSpecialLandscapes) + math_utils::binomial<coefficient_t>(kNumStarterLandscapes - kNumSpecialLandscapes, 2u);

/*
 * Other
 * =====================================================================================================================
 */
inline constexpr coefficient_t kKnights         = 10;
inline constexpr coefficient_t kRuinsTypes      = 5;
inline constexpr coefficient_t kRuinsPerType    = 10;

inline constexpr coefficient_t kDruidBoonPicks  = 3;

/*
 * Combinations and factorials
 * =====================================================================================================================
 */
inline constexpr auto kBoonShufflesNoDruid = math_utils::factorial<result_t>(kBoons);
inline constexpr auto kBoonShufflesDruid = math_utils::factorial<result_t>(kBoons - kDruidBoonPicks);
inline constexpr auto kHexShuffles = math_utils::factorial<result_t>(kHexes);
inline constexpr auto kKnightShuffles = math_utils::factorial<result_t>(kKnights);
inline constexpr auto kDruidBoonCombinations = math_utils::binomial<result_t>(kBoons, kDruidBoonPicks);
inline constexpr auto kLiaisonAllyCombinations = (result_t)kAllies;
inline constexpr coefficient_t kPlayers = 2u;
inline constexpr coefficient_t kNumRuinsPerExtraPlayer = 10u;

auto ruin_combinations(coefficient_t num_ruin_types, coefficient_t remainder) -> result_t {
    if (remainder == 0)
        return 1ul;

    if (num_ruin_types == 0)
        return 0ul;

    auto result = (result_t)0u;
    for(auto i = (coefficient_t)0u; i <= std::min(remainder, kRuinsPerType); ++i){
        result += math_utils::binomial<result_t>(remainder, i) * ruin_combinations(num_ruin_types - 1U, remainder - i);
    }
    return result;
}

inline auto kRuinsCombinations = ruin_combinations(kRuinsTypes, (kPlayers - 1)*kNumRuinsPerExtraPlayer);

inline constexpr coefficient_t kFactorSpecialLandscapeActive = 1 + kNumSpecialLandscapes;
inline constexpr coefficient_t kFactorSpecialLandscapeInactive = 2 + kNumSpecialLandscapes + math_utils::binomial<coefficient_t>(kNumSpecialLandscapes, 2u);
