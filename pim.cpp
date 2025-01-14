#include <chrono>
#include <print>
#include <algorithm>
#include <unordered_map>

#include "generator_bounds.hpp"
#include "amount_getters.hpp"
#include "tuple.hpp"
#include "default_expansion_editions.hpp"
#include "constrained_product_generator.hpp"
#include "card_amount_tables.hpp"


using config::coefficient_t;
using config::GeneratorBounds;
using config::Expansion;
using config::kAllExpansionEditions;
using config::kingdom::ExpansionEditionFilter;
using config::kingdom::EditionModifier;

using V = boost::multiprecision::uint256_t;

template<>
struct std::formatter<V> : std::formatter<std::string> {
    std::optional<std::size_t> width{};

    constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it == ctx.end() || *it == '}')
            return it;

        std::size_t w = 0uz;
        while(*it != '}'){
            w = 10*w + (std::size_t)(*it - '0');
            ++it;
        }
        width = w;
        return it;
    }

    auto format(const V& obj, std::format_context& ctx) const {
        std::ostringstream oss{};
        oss << obj;
        if (width.has_value()){
            return std::format_to(ctx.out(), "{:>{}}", oss.str(), width.value());
        } else {
            return std::format_to(ctx.out(), "{}", oss.str());
        }

    }
};

static constexpr auto kBounds = config::from_expansions_editions(kAllExpansionEditions);
static constexpr auto kMaxAddableSum = std::ranges::fold_left(kBounds.kingdom_regular | std::views::drop(1), 0u, std::plus{});
static const V loot_number = []() -> V{
    V result{1};
    result *= V{2048u}; // 2^11
    result *= V{4782969u}; // 3^14
    result *= V{78125}; // 5^7
    result *= V{2401}; // 7^4
    result *= V{121u}; // 11^2
    result *= V{169u}; // 13^2
    result *= V{17u}; // 17
    result *= V{19u}; // 19
    result *= V{23u}; // 23
    result *= V{29u}; // 29
    return result;
}();

enum SpecialBit : uint8_t {
    YoungWitch,         // unused Venn keuze (+ add to supply)
    OmenAA,             // 14 + unused Venn keuze (add to supply) + young witch
    WayOfTheMouse,      // unused Venn keuze
    Ferryman,           // unused Venn keuze + young witch
    Riverboat,          // unused Venn keuze + ferryman
    Obelisk             // used Venn keuze
};

//enum class SpecialState : uint8_t {
//    Unused,
//    Supply,
//    Side
//};

static constexpr std::size_t kNumSpecialBits = 6uz;
static constexpr std::size_t kVennSize = (1uz << kNumSpecialBits);

static constexpr std::size_t kNumModifiers = 6uz;
static constexpr std::size_t kMaxModifierMask = (1u << kNumModifiers);

using modifiers_array_t = std::array<coefficient_t, kNumModifiers + 1u>;
using venn_modifiers_array_t = std::array<modifiers_array_t, kVennSize>;
using venn_array_t = std::array<coefficient_t, kVennSize>;
using modifier_mask_subtotals_t = std::array<V, (1u << kNumModifiers)>;

struct VennRegionMask {
    uint8_t mask;
    uint32_t subtotal;
};

struct VennRegionPicks {
    std::vector<VennRegionMask> modifier_masks;
    std::vector<std::vector<V>> coefficients;
};

struct VennRegion {
    uint8_t venn_membership_mask;
    VennRegionPicks picks;
};

void print_venn_region(const VennRegion& vr){
    std::println("Venn region with membership mask {:0>6b}", vr.venn_membership_mask);
    std::print("modifier masks: ");
    for (const auto& pick : vr.picks.modifier_masks) {
        std::print("{:9}", " ");
        std::print("{:0>6b} ", pick.mask);
    }
    std::print("\n     subtotals: ");
    for (const auto& pick : vr.picks.modifier_masks) {
        std::print("{:15} ", pick.subtotal);
    }
    std::print("\n---------------");
    for(auto i=0uz; i < vr.picks.coefficients.size(); ++i ){
        std::print("\n{:14}: ", i);
        for(const auto& v : vr.picks.coefficients[i]) {
            std::print("{:15} ", v);
        }
    }
    std::println();
    std::println();
}

using compressed_venn_data_t = std::vector<VennRegion>;

auto create_venn_region_picks(const modifiers_array_t& modifiers, unsigned int venn_region_size) -> VennRegionPicks {
    VennRegionPicks result{};
    const auto nnz_modifier_indices = std::views::iota(1u, kNumModifiers + 1u) | std::views::filter([&modifiers](const auto i){return modifiers[i]>0;}) | std::ranges::to<std::vector>();
    result.modifier_masks.reserve((1u << nnz_modifier_indices.size()));
    result.modifier_masks.emplace_back(0, modifiers[0]);
    for(const auto& nnz : nnz_modifier_indices){
        const auto old_size = result.modifier_masks.size();
        for(auto i = 0uz; i < old_size; ++i){
            result.modifier_masks.emplace_back(result.modifier_masks[i].mask | (1u << (nnz - 1)), result.modifier_masks[i].subtotal + modifiers[nnz]);
        }
    }

    for(auto n = 0u; n <= std::min(10u, venn_region_size); ++n){
        result.coefficients.emplace_back();
        auto& back = result.coefficients.back();
        for(auto i = 0uz; i < result.modifier_masks.size(); ++i){
            const auto& [mi, subtotal] = result.modifier_masks[i];
            V coefficient = utils::math::binomial<V>(subtotal, n);
            for(auto j = 0uz; j < i; ++j){
                const auto& [mj, _] = result.modifier_masks[j];
                if ((mi & mj) == mj){
                    coefficient -= back[j];
                }
            }
            back.emplace_back(coefficient);
        }
    }

    return result;
}

auto create_venn_diagram(const config::kingdom::query_amounts_t& input) -> venn_modifiers_array_t {
    auto impl = [&input]<std::size_t ... I>(std::index_sequence<I...>) -> venn_modifiers_array_t {
        venn_modifiers_array_t result{};
        (..., (result[config::kingdom::special_venn_diagram_masks()[I].to_ulong()][std::to_underlying(config::kingdom::modifiers()[I])] += input[I]));
        return result;
    };
    return impl(std::make_index_sequence<std::tuple_size_v<config::kingdom::query_amounts_t>>());
}

auto create_compressed_venn(const venn_modifiers_array_t& input) -> compressed_venn_data_t {
    compressed_venn_data_t result{};
    for (const auto& [venn_membership_mask, v] : std::views::enumerate(input)){
        const auto venn_region_size = std::ranges::fold_left(v, 0u, std::plus{});
        if(venn_region_size > 0){
            result.emplace_back(venn_membership_mask, create_venn_region_picks(v, venn_region_size));
        }
    }
    return result;
}

static const venn_modifiers_array_t kVennDiagram = create_venn_diagram(kBounds.kingdom_regular);
static const compressed_venn_data_t kCompressedVenn = create_compressed_venn(kVennDiagram);
static const std::vector<coefficient_t> kCompressedVennRegionSizes = [](){
    std::vector<coefficient_t> result(kCompressedVenn.size(), 0u);
    std::size_t i = 0uz;
    for (const auto& modifier_array : kVennDiagram){
        const auto size = std::ranges::fold_left(modifier_array, 0u, std::plus{});
        if(size >0){
            result.at(i) = size;
            ++i;
        }
    }
    return result;
}();
static const std::vector<coefficient_t> kMaxAddable = [](){
    std::vector<coefficient_t> result(kCompressedVenn.size() + 1uz, 0u);
    std::partial_sum(kCompressedVennRegionSizes.crbegin(), kCompressedVennRegionSizes.crend(), result.rbegin() + 1);
    return result;
}();

/*constexpr venn_array_t complement(const config::kingdom::query_amounts_t& input){
    venn_array_t result{};
    std::ranges::transform(kVennDiagramBound, input, result.begin(), std::minus{});
    return result;
}

template<SpecialBit S>
auto index_helper(const venn_array_t& vd){
    return std::views::iota(0uz, kVennSize) |
        std::views::filter([&vd](const auto i){return (((i >> S) & 1) > 0u) && (vd[i] > 0u);});
}

template<SpecialBit S>
auto index_sum(const venn_array_t& v) {
    return std::ranges::fold_left(index_helper<S>() | std::views::transform([&](const auto i){return v[i];}), 0u, std::plus{});
}*/

/*auto special_states_from_tuple(const config::kingdom::special_amounts_t& kingdom_special,
                               const config::landscapes::supply::special_amounts_t& landscape_supply_special,
                               bool has_omen) -> special_states_t {
    special_states_t result{};
    result[YoungWitch] = (SpecialState)std::get<std::to_underlying(config::kingdom::SpecialIndex::YOUNG_WITCH)>(kingdom_special);
    result[OmenAA] = has_omen ? SpecialState::Supply : SpecialState::Unused;
    result[WayOfTheMouse] = (SpecialState)std::get<std::to_underlying(config::landscapes::supply::SpecialIndex::WAY_OF_THE_MOUSE)>(landscape_supply_special);
    result[Ferryman] = (SpecialState)std::get<std::to_underlying(config::kingdom::SpecialIndex::FERRYMAN)>(kingdom_special);
    result[Riverboat] = (SpecialState)std::get<std::to_underlying(config::kingdom::SpecialIndex::RIVERBOAT)>(kingdom_special);
    result[Obelisk] = (SpecialState)std::get<std::to_underlying(config::landscapes::supply::SpecialIndex::OBELISK)>(landscape_supply_special);
    return result;
}

template<SpecialBit S, SpecialBit ... T>
auto calc7(special_states_t& special_states, venn_array_t& supply_vd, venn_array_t& unused_vd, bool has_ruins) -> V {
    V result{};

    if constexpr (S == YoungWitch) {
        // if young witch is in play and it can actually choose an unused card
        if ((special_states[S] != SpecialState::Unused) && index_sum<S>(unused_vd) > 0) {
            // over all Venn regions with YoungWitch bit ON, if can pick from that, then remove from Venn, add to supply and go to next step
            for (const auto i : index_helper<S>(unused_vd)){
                const auto num_choices = unused_vd[i];
                --unused_vd[i];
                ++supply_vd[i];
                result += num_choices * calc7<T...>(special_states, supply_vd, unused_vd, has_ruins);
                ++unused_vd[i];
                --supply_vd[i];
            }
        } else {
            result += calc7<T...>(special_states, supply_vd, unused_vd, has_ruins);
        }
    }
    else if constexpr (S == OmenAA) {
        const auto no_aa_result = calc7<T...>(special_states, supply_vd, unused_vd, has_ruins);

        // if Omen in kingdom
        if (special_states[S] != SpecialState::Unused) {
            const auto has_picks = index_sum<S>(unused_vd) > 0; // check how many choices AA has
            const auto is_young_witch_unused = special_states[YoungWitch] == SpecialState::Unused;

            if (has_picks || is_young_witch_unused) { // if has any
                if (has_picks) {
                    result += 14u * no_aa_result; // 14 possibilities to not pick AA
                    for (const auto i : index_helper<S>(unused_vd)) { // pick AA: sum all the possibilities for AA choices
                        const auto num_choices = unused_vd[i];
                        --unused_vd[i];
                        ++supply_vd[i];
                        result += num_choices * calc7<T...>(special_states, supply_vd, unused_vd, has_ruins);
                        ++unused_vd[i];
                        --supply_vd[i];
                    }
                }
                if (is_young_witch_unused) { // if young witch not already in kingdom, there's one possibility to have AA pick it and add to kingdom
                    special_states[YoungWitch] = SpecialState::Supply;
                    result += calc7<YoungWitch, T...>(special_states, supply_vd, unused_vd, has_ruins);
                    special_states[YoungWitch] = SpecialState::Unused;
                }
            } else { // if we cannot pick anything for AA, just multiply with 15
                result += 15u * no_aa_result;
            }
        } else { // if omen not in kingdom, empty choice (1), go to next step
            result += no_aa_result;
        }
    }
    else if constexpr (S == WayOfTheMouse) {
        // if way of the mouse is in play and it can actually choose an unused ard
        if ((special_states[S] != SpecialState::Unused) && index_sum<S>(unused_vd) > 0) {
            // over all Venn regions with Way of the mouse bit ON, if can pick from that, then remove from Venn, set aside (not add to supply)
            for (const auto i : index_helper<S>(unused_vd)){
                const auto num_choices = unused_vd[i];
                --unused_vd[i];
                result += num_choices * calc7<T...>(special_states, supply_vd, unused_vd, has_ruins);
                ++unused_vd[i];
            }
        } else { // choose nothing and continue
            result += calc7<T...>(special_states, supply_vd, unused_vd, has_ruins);
        }
    }
    else if constexpr (S == Ferryman) {
        const auto has_picks = index_sum<S>(unused_vd) > 0;
        const auto is_young_witch_unused = special_states[YoungWitch] == SpecialState::Unused;

        // if Ferryman is in play and it can actually choose an unused card
        if ((special_states[S] != SpecialState::Unused) && (has_picks || is_young_witch_unused)) {
            // over all Venn regions with Ferryman bit ON, if can pick from that, then remove from Venn, set aside (not add to supply)
            if (has_picks) {
                for (const auto i : index_helper<S>(unused_vd)) {
                    const auto num_choices = unused_vd[i];
                    --unused_vd[i];
                    result += num_choices * calc7<T...>(special_states, supply_vd, unused_vd, has_ruins);
                    ++unused_vd[i];
                }
            }
            if (is_young_witch_unused){ // if young witch not already in kingdom, there's one possibility to pick it and set aside
                special_states[YoungWitch] = SpecialState::Side;
                result += calc7<YoungWitch, T...>(special_states, supply_vd, unused_vd, has_ruins);
                special_states[YoungWitch] = SpecialState::Unused;
            }
        } else { // choose nothing and continue
            result += calc7<T...>(special_states, supply_vd, unused_vd, has_ruins);
        }
    }
    else if constexpr (S == Riverboat) {
        const auto has_picks = index_sum<S>(unused_vd) > 0;
        const auto is_ferryman_unused = special_states[Ferryman] == SpecialState::Unused;

        // if Riverboat is in kingdom and it can actually choose an unused card
        if ((special_states[S] != SpecialState::Unused) && (has_picks || is_ferryman_unused)) {
            if (has_picks) {
                // over all Venn regions with Riverboat bit ON, if can pick from that, then remove from Venn, set aside (not add to supply)
                for (const auto i : index_helper<S>(unused_vd)) {
                    const auto num_choices = unused_vd[i];
                    --unused_vd[i];
                    result += num_choices * calc7<T...>(special_states, supply_vd, unused_vd, has_ruins);
                    ++unused_vd[i];
                }
            }
            if (is_ferryman_unused){ // if ferryman not already in kingdom, there's one possibility to pick it and set aside
                special_states[Ferryman] = SpecialState::Side;
                result += calc7<Ferryman, T...>(special_states, supply_vd, unused_vd, has_ruins);
                special_states[Ferryman] = SpecialState::Unused;;
            }
        } else { // choose nothing and continue
            result += calc7<T...>(special_states, supply_vd, unused_vd, has_ruins);
        }
    }
    else if constexpr (S == Obelisk) {
        if (special_states[S] == SpecialState::Unused){ // obelisk not in play: 1 (empty choice) and recursion ends
            return V{1u};
        }
        // if obelisk in kingdom, choose any feasible card or ruins, or riverboat/ferryman/youngwitch if in supply
        const auto num_choices = index_sum<S>(supply_vd) +
                                 (has_ruins ? 1u : 0u) +
                                 (special_states[Riverboat] == SpecialState::Supply ? 1 : 0) +
                                 (special_states[Ferryman] == SpecialState::Supply ? 1 : 0) +
                                 (special_states[YoungWitch] == SpecialState::Supply ? 1 : 0);
        return std::max(V{1u}, V{num_choices});
    }
    else{
        exit(1);
    }

    return result;
}

struct X {
    const config::kingdom::special_amounts_t& kingdom_special;
    const config::landscapes::supply::query_amounts_t& landscapes_supply_regular;
    const config::landscapes::supply::special_amounts_t& landscapes_supply_special;
    // venn diagram

    //auto operator==(const X& x) const noexcept -> bool = default;
};

template<typename ReturnType>
auto calc6(const X& x) -> ReturnType {
    const ReturnType constant_factors = 1u;
            // if has knights, multiply by 10!
            // if has druid, multiply by 3!
            // if has fate, multiply by 9! if druid else 12!
            // if has doom, multiply by 12!
            // if has looter, multiply by ruins number
            // if has liaison, multiply by number of allies (choose 1)

*/

/*
1. young witch kiest een unused 2/3
2. approaching army: unused attack: check eerst of je omen hebt, dan (14 + num_unused_attack)
3. wotm kiest een unused action 2/3
4. ferryman kiest een unused 3/4 kaart
5. riverboat kiest een unused non-duration action 5
6. obelisk kiest een used action of ruins
*/

/*

    //create aux array [has_yw, has_omen, has_wotm, has_fm, has_rb, has_obelisk]


    //loop factors
    const auto kingdom_regular_omen_factor = kingdom_regular_has_omen ? (14u + num_unused_actions + (has_young_witch ? num_yw_choices : 0u)) : 1u;

}

template<typename ReturnType, std::size_t NumKingdomSpecials>
auto calc5() -> ReturnType {
    ReturnType result{};
    for (const auto& kingdom_special : utils::generators::constrained_product<utils::generators::Constraint::EQ>(kBounds.kingdom_special, NumKingdomSpecials)) {
        for (const auto num_landscape_supply_regular : std::views::iota(0u, 3u)) {
            for (const auto& landscape_supply_regular : utils::generators::constrained_product<utils::generators::Constraint::EQ>(kBounds.landscapes_supply_regular, num_landscape_supply_regular)) {
                for (const auto& landscape_supply_special : utils::generators::constrained_product<utils::generators::Constraint::LE>(kBounds.landscapes_supply_special, 2U - num_landscape_supply_regular)) {
                    const X x{
                        .kingdom_special = kingdom_special,
                        .landscapes_supply_regular = landscape_supply_regular,
                        .landscapes_supply_special = landscape_supply_special,
                    };

                    result += calc6<ReturnType>(x);
                }
            }
        }
    }
    return result;
}*/

/*struct State {
    uint32_t current_sum;
    venn_array_t venn_diagram;
    bool has_doom :1;
    bool has_fate :1;
    bool has_liaison :1;
    bool has_loot :1;
    bool has_looter :1;
    bool has_omen :1;

    auto operator==(const State& s) const noexcept -> bool = default;
};

struct StateHash {
    auto operator()(const State& s) const noexcept {
        return s.current_sum +
                ((std::size_t)s.has_doom << 32u) +
                ((std::size_t)s.has_fate << 33u) +
                ((std::size_t)s.has_liaison << 34u) +
                ((std::size_t)s.has_loot << 35u) +
                ((std::size_t)s.has_looter << 36u)  +
                ((std::size_t)s.has_omen << 37u) ;
    }
};

auto calc5(const State& s) -> V {
    V result{1u};
    if(s.has_doom){
        result *= utils::math::factorial<V>(12u);
    }
    if(s.has_liaison){
        result *= V{23u};
    }
    if(s.has_loot){
        result *= 1u; //TODO RIDICULOUS LOOT NUMBER
    }
    if(s.has_looter){
        result *= V{9765625u};
    }
    return result;
}

template<std::size_t NumKingdomSpecials, std::size_t KingdomRegularIndex>
auto calc4(const State& state) -> V {
//    std::println("calc4<NuKingdomSpecials = {}, Index = {}", NumKingdomSpecials, KingdomRegularIndex);

    static constexpr auto NumKingdomRegulars = 10uz - NumKingdomSpecials;

    if constexpr (KingdomRegularIndex == std::tuple_size_v<config::kingdom::query_amounts_t> - 1uz){
        return utils::math::binomial<V>(kBounds.kingdom_regular[KingdomRegularIndex], (uint32_t)NumKingdomRegulars - state.current_sum)
//                * calc5(state)
                ;
    } else {
        static std::unordered_map<State, V, StateHash> cache{};
        auto findIt = cache.find(state);
        if (findIt != std::ranges::cend(cache)) {
            return findIt->second;
        }
        constexpr coefficient_t lo = kMaxAddableSum  <= NumKingdomRegulars ? NumKingdomRegulars - kMaxAddableSum : 0u;
        const coefficient_t hi = std::min(kBounds.kingdom_regular[KingdomRegularIndex], (coefficient_t)(NumKingdomRegulars - state.current_sum));
        V subtotal{};
        for (coefficient_t k = lo; k <= hi; ++k){
            V coefficient{1};
            const bool is_non_zero = (k > 0);
            State new_state = state;
            new_state.current_sum += k;
            new_state.venn_diagram[config::kingdom::special_venn_diagram_masks()[KingdomRegularIndex].to_ulong()] += k;
            if constexpr (config::kingdom::multiplier_masks()[KingdomRegularIndex].test(config::kingdom::Multiplier::DOOM)){
                new_state.has_doom = state.has_doom || is_non_zero;
                if (new_state.has_doom && !state.has_doom){
                    coefficient *= utils::math::factorial<V>(12u);
                }
            }
            if constexpr (config::kingdom::multiplier_masks()[KingdomRegularIndex].test(config::kingdom::Multiplier::FATE)){
                new_state.has_fate = state.has_fate || is_non_zero;
            }
            if constexpr (config::kingdom::multiplier_masks()[KingdomRegularIndex].test(config::kingdom::Multiplier::LIAISON)){
                new_state.has_liaison = state.has_liaison || is_non_zero;
                if (new_state.has_liaison && !state.has_liaison){
                    coefficient *= V{23};
                }
            }
            if constexpr (config::kingdom::multiplier_masks()[KingdomRegularIndex].test(config::kingdom::Multiplier::LOOT)){
                new_state.has_loot = state.has_loot || is_non_zero;
                if (new_state.has_loot && !state.has_loot){
                    coefficient *= 1; //TODO ridiculously large number
                }
            }
            if constexpr (config::kingdom::multiplier_masks()[KingdomRegularIndex].test(config::kingdom::Multiplier::LOOTER)){
                new_state.has_looter = state.has_looter || is_non_zero;
                if (new_state.has_looter && !state.has_looter){
                    coefficient *= loot_number;
                }
            }
            if constexpr (config::kingdom::multiplier_masks()[KingdomRegularIndex].test(config::kingdom::Multiplier::OMEN)){
                new_state.has_omen = state.has_omen || is_non_zero;
            }

            subtotal += utils::math::binomial<V>(kBounds.kingdom_regular[KingdomRegularIndex], k) * calc4<NumKingdomSpecials, KingdomRegularIndex + 1>(new_state)
                        * coefficient
                        ;
        }
        cache[state] = subtotal;
        return subtotal;
    }
}*/

std::ostream& operator<<(std::ostream& os, const modifier_mask_subtotals_t& data){
    os << "[";
    for(const auto& x : data){
        std::cout << x << ", ";
    }
    return os << ']';
}

template<std::size_t NumKingdomSpecials>
auto calc4(std::size_t compressed_venn_index, coefficient_t current_sum, std::vector<coefficient_t>& used_vd, const modifier_mask_subtotals_t& modifier_mask_subtotals) -> V {
//    std::cout << "i = " << compressed_venn_index << ". Mask subtotals: " << modifier_mask_subtotals << '\n';
    if(compressed_venn_index == kCompressedVenn.size()){
        assert(current_sum == 10 - NumKingdomSpecials);
        return (V)std::ranges::fold_left(modifier_mask_subtotals, V{}, std::plus{});
    }
    const auto& current_venn_region = kCompressedVenn[compressed_venn_index];
    const auto remaining_sum = 10 - NumKingdomSpecials - current_sum;
    const coefficient_t lo = compressed_venn_index < kCompressedVenn.size() - 1 ? 0u : remaining_sum; //TODO improve
    const coefficient_t hi = std::min(kCompressedVennRegionSizes[compressed_venn_index], (coefficient_t)remaining_sum);
    V result{};
    for (coefficient_t k = lo; k <= hi; ++k){
        modifier_mask_subtotals_t new_modifier_mask_subtotals{};
        for(auto i = 0u; i < (1u << kNumModifiers); ++i ){
            if(const auto& subtotal = modifier_mask_subtotals[i]; subtotal > 0){
                for(auto j = 0u; j < current_venn_region.picks.modifier_masks.size(); ++j){
                    const auto m = i | current_venn_region.picks.modifier_masks[j].mask;
                    const auto c = current_venn_region.picks.coefficients[k][j];
                    new_modifier_mask_subtotals[m] += subtotal * c;
                }
            }
        }
        used_vd[compressed_venn_index] = k;
        result += calc4<NumKingdomSpecials>(compressed_venn_index + 1u, current_sum + k, used_vd, new_modifier_mask_subtotals);
    }
    used_vd[compressed_venn_index] = 0u;
    return result;
}

template<std::size_t NumKingdomSpecials>
auto calc3() -> V {
    std::cout << "calc3<" << NumKingdomSpecials << ">" << std::endl;
    modifier_mask_subtotals_t init{};
    init[0] = V{1};
    std::vector<coefficient_t> used_vd{};
    used_vd.resize(kCompressedVennRegionSizes.size());
    const auto actual = calc4<NumKingdomSpecials>(0u, 0u, used_vd, init);
    return actual;
}

template<std::size_t ... NumKingdomSpecials>
auto calc2(std::index_sequence<NumKingdomSpecials ...>) -> V {
    std::cout << "calc2" << std::endl;
    return (... + (calc3<NumKingdomSpecials>()));
}

auto calc1() -> V {
    std::cout << "calc1" << std::endl;
    static constexpr auto kKingdomSpecialSize = std::tuple_size_v<config::kingdom::special_amounts_t>;
    return calc2(std::make_index_sequence<kKingdomSpecialSize + 1uz>());
}

void print_venn_regions() {
    for(const auto& venn_region : kCompressedVenn){
        print_venn_region(venn_region);
    }
}

auto main() -> int {
    print_venn_regions();
//    const auto t1 = std::chrono::steady_clock::now();
//    const auto result = calc1();
//    const auto t2 = std::chrono::steady_clock::now();
//    std::println("Result:       {}", result);
//    std::println("Should equal: {}", utils::math::binomial<V>(493u, 10u) + utils::math::binomial<V>(493u, 9u) + utils::math::binomial<V>(493u, 8u) + utils::math::binomial<V>(493u, 7u) + utils::math::binomial<V>(493u, 6u) + utils::math::binomial<V>(493u, 5u));
//    std::println("Elapsed time: {}", std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1));
}