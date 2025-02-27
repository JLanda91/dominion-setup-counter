#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <boost/multiprecision/integer.hpp>
#include <boost/functional/hash.hpp>

#include <vector>
#include <ranges>
#include <cstdint>
#include <chrono>
#include <unordered_map>

#include <card_data/kingdom/card_type_major_table.hpp>
#include <card_data/extra_setup/state.hpp>
#include <card_data/combination_modifiers.hpp>

#include <utils/math.hpp>
#include <utils/filtered_index_sequence.hpp>
#include <utils/result_type.hpp>

using namespace std::chrono_literals;

class OffsetsCalculator {
    using cache_key_t = std::pair<uint8_t, uint8_t >;
    using offsets_t = std::vector<size_t>;
public:
    explicit OffsetsCalculator(std::span<const uint8_t> data) : data_{data}, max_subtotal_(std::min(std::ranges::fold_left(data, static_cast<uint8_t>(0u), std::plus<uint8_t>{}), static_cast<uint8_t>(10u))) {}

    offsets_t operator()(){
        offsets_t result(max_subtotal_ + 2u, 0u);
        for(auto s : std::views::iota(0u, max_subtotal_ + 1u)){
            result.at(s + 1u) = result.at(s) + impl(0u, s);
        }
        return result;
    }

private:
    std::span<const uint8_t> data_;
    uint8_t max_subtotal_;
    std::unordered_map<cache_key_t, uint64_t, boost::hash<cache_key_t>> cache_{};

    auto impl(uint8_t i, uint8_t s) -> uint64_t {
        if(i == data_.size()){
            return s == 0 ? 1ul : 0ul;
        }
        const cache_key_t key{i, s};
        if(const auto it = cache_.find(key); it != cache_.end()){
            return it->second;
        }
        uint64_t result = 0u;
        for(uint8_t k = 0; k <= std::min(s, data_[i]); ++k){
            result += impl(i + 1, s - k);
        }
        cache_[key] = result;
        return result;
    }
};

struct SubArray {
    using data_t = std::array<uint8_t, card_data::kingdom::kNumVennRegions>;
    uint64_t binom_product;
    data_t data;
    uint8_t num_action_treasure;
};

using sub_array_combination_t = std::array<const SubArray*, card_data::kingdom::kNumCardTypes>;

class SubArrayVector {
    using data_t = std::vector<SubArray>;
    using offsets_t = std::vector<size_t>;
public:
    explicit SubArrayVector(std::span<const uint8_t> arr) {
        auto impl = [&arr]<std::size_t I>(this auto&& self, data_t& data, SubArray::data_t subArray, uint64_t binomProduct, uint8_t total, uint8_t numActionTreasure) -> void {
            if constexpr (I == card_data::kingdom::kNumVennRegions) {
                data.emplace_back(binomProduct, subArray, numActionTreasure);
            } else {
                const auto max_amount = arr[I];
                for (uint8_t k = 0; k <= std::min(total, max_amount); ++k) {
                    subArray[I] = k;
                    if constexpr (card_data::kingdom::card_type_major_table().row_labels().at(I).is_action_or_treasure_region) {
                        self.template operator()<I+1>(data, subArray, binomProduct * utils::math::binomial<uint64_t>(max_amount, k), total - k, numActionTreasure + k);
                    } else {
                        self.template operator()<I+1>(data, subArray, binomProduct * utils::math::binomial<uint64_t>(max_amount, k), total - k, numActionTreasure);
                    }
                }
            }
        };
        offsets_ = OffsetsCalculator(arr)();
        sub_arrays_.reserve(offsets_.back());
        SubArray::data_t sub_array{};
        uint8_t num_action_treasure{};
        sub_array.fill(0u);
        impl.template operator()<0>(sub_arrays_, sub_array, 1ul, std::size(offsets_) - 2u, num_action_treasure);
        const auto subtotal_proj = [](const SubArray& sa) { return std::ranges::fold_left(sa.data, 0u, std::plus{}); };
        std::ranges::sort(sub_arrays_, std::less{}, subtotal_proj);
    }

    auto offsets() const -> const offsets_t& {
        return offsets_;
    }

    auto size() const -> std::size_t {
        return std::size(sub_arrays_);
    }

    auto max_subtotal() const -> uint8_t {
        return std::size(offsets_) - 2u;
    }

    auto sub_arrays_with_subtotal(uint8_t s) const  {
        if (s >= std::size(offsets_) - 1u){
            return std::ranges::subrange(sub_arrays_.cend(), sub_arrays_.cend(), 0u);
        }
        return std::ranges::subrange(sub_arrays_.cbegin() + offsets_.at(s), sub_arrays_.cbegin() + offsets_.at(s + 1), offsets_.at(s + 1) - offsets_.at(s));
    }

private:
    data_t sub_arrays_{};
    offsets_t offsets_{};
};

static auto sub_array_vectors() -> const std::vector<SubArrayVector>& {
    static const auto singleton = []{
        const auto t1 = std::chrono::steady_clock::now();
        const auto result = card_data::kingdom::card_type_major_table().columns() | std::views::transform([](const auto& column) { return SubArrayVector(column); }) | std::ranges::to<std::vector>();
        const auto t2 = std::chrono::steady_clock::now();
        fmt::println("Generated all subcolumns in: {} s\n", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() / static_cast<double>(1s / 1ms));
        return result;
    }();
    return singleton;
}

struct SubArrayCombinationState {
    uint8_t action_treasure_subtotal : 4;
    card_data::CombinationModifiers modifiers;
};

template<card_data::kingdom::CardType C, bool AddApproachingArmy>
constexpr auto new_sub_array_combination_state_from(const SubArrayCombinationState& state, const SubArray& nonzero_sa) -> SubArrayCombinationState {
    static_assert((C == card_data::kingdom::CardType::Omen) || !AddApproachingArmy);

    SubArrayCombinationState new_state = state;
    new_state.action_treasure_subtotal += nonzero_sa.num_action_treasure;
    new_state.modifiers = state.modifiers.with_set_column<C, AddApproachingArmy>();
    return new_state;
}

template<typename Index>
struct UnusedTableElements {
    template<typename F>
    static result_t fold(const sub_array_combination_t& subArrayCombination, const card_data::extra_setup::State& picks_state, F&& op){
        return col_impl(subArrayCombination, picks_state, std::forward<F>(op), card_data::kingdom::make_card_type_sequence{});
    }
private:
    template<typename F, std::size_t I, card_data::kingdom::CardType C>
    static result_t element_impl(const sub_array_combination_t& subArrayCombination, const card_data::extra_setup::State& picks_state, F&& op){
        constexpr std::size_t J = std::to_underlying(C);
        const auto total = card_data::kingdom::card_type_major_table().column(J)[I];
        const auto subtotal = subArrayCombination[J]->data[I];
        if (const auto num_picks = total - subtotal - picks_state.num_already_picked_at(I, C); num_picks > 0) {
            return op.template operator()<I, C>(static_cast<uint8_t>(num_picks));
        }
        return 0u;
    }

    template<typename F, card_data::kingdom::CardType C, std::size_t ... I>
    static result_t row_impl(const sub_array_combination_t& subArrayCombination, const card_data::extra_setup::State& picks_state, F&& op, std::index_sequence<I...>){
        return (... + (element_impl<F, I, C>(subArrayCombination, picks_state, std::forward<F>(op))));
    }

    template<typename F, card_data::kingdom::CardType ... C>
    static result_t col_impl(const sub_array_combination_t& subArrayCombination, const card_data::extra_setup::State& picks_state, F&& op, card_data::kingdom::card_type_sequence<C...>){
        return (... + (row_impl<F, C>(subArrayCombination, picks_state, std::forward<F>(op), Index{})));
    }
};

auto extra_setup_pick_impl(const sub_array_combination_t& subArrayCombination, const card_data::extra_setup::dispatch_vector_t& dispatch, const card_data::CombinationModifiers& modifiers, const card_data::extra_setup::State& picks_state)-> result_t {
    if (std::empty(dispatch)){
        return 1u;
    }
    const auto current_card = dispatch.front();
    if (current_card == card_data::extra_setup::CardType::YoungWitch) { // young witch
        return UnusedTableElements<card_data::kingdom::row_index_sequence_young_witch_t>::fold(subArrayCombination, picks_state, [&]<size_t I, card_data::kingdom::CardType C>(auto num_picks){
            const auto new_pick_state = picks_state.with_added_picker<card_data::extra_setup::CardType::YoungWitch>(I, C); // update which table cell young witch picks from
            const auto factor = !modifiers.get_from_kingdom_column<C>() ? card_data::kingdom::column_factor<C>() : 1u; // assign pile factor if not already in kingdom
            const auto new_disp = new_dispatch<C, false>(dispatch); // update dispatch without approaching army
            const auto new_modifiers = modifiers.with_set_column<C, false>(); // update modifiers: set column bool to true
            if constexpr (C == card_data::kingdom::CardType::Omen) { // in case young witch picks omen
                if(!modifiers.get_from_kingdom_column<C>()){ // omen not already in kingdom
                    const auto new_disp_with_aa = new_dispatch<C, true>(dispatch); // update dispatch with approaching army
                    const auto new_modifiers_with_aa = modifiers.with_set_column<C, true>(); // update modifiers: set column bool to true and add approaching army
                    return num_picks * factor * (14u * extra_setup_pick_impl(subArrayCombination, new_disp, new_modifiers, new_pick_state) + // 14 ways to not pick approaching army
                                                       extra_setup_pick_impl(subArrayCombination, new_disp_with_aa, new_modifiers_with_aa, new_pick_state)); // 1 way to pick approaching army

                }
            }
            return num_picks * factor * extra_setup_pick_impl(subArrayCombination, new_disp, new_modifiers, new_pick_state);
        });

    } else if (current_card == card_data::extra_setup::CardType::ApproachingArmy) { // approaching army
        return UnusedTableElements<card_data::kingdom::row_index_sequence_approaching_army_t>::fold(subArrayCombination, picks_state, [&]<std::size_t I, card_data::kingdom::CardType C>(auto num_picks){
            const auto new_pick_state = picks_state.with_added_picker<card_data::extra_setup::CardType::ApproachingArmy>(I, C); // update which table cell approaching army picks from
            const auto factor = !modifiers.get_from_kingdom_column<C>() ? card_data::kingdom::column_factor<C>() : 1u; // assign pile factor if not already in kingdom
            const auto new_disp = new_dispatch<C, false>(dispatch); // update dispatch without approaching army
            const auto new_modifiers = modifiers.with_set_column<C, false>(); // update modifiers: set column bool to true
            // compile time if for J == 9 removed because we only handle approaching army here if we already picked omen
            return num_picks * factor * extra_setup_pick_impl(subArrayCombination, new_disp, new_modifiers, new_pick_state);
        });
    } else if (current_card == card_data::extra_setup::CardType::WayOfTheMouse) { // way of the mouse
        return UnusedTableElements<card_data::kingdom::row_index_sequence_way_of_the_mouse_t>::fold(subArrayCombination, picks_state, [&]<std::size_t I, card_data::kingdom::CardType C>(auto num_picks){
            const auto new_pick_state = picks_state.with_added_picker<card_data::extra_setup::CardType::WayOfTheMouse>(I, C); // update which table cell way of the mouse picks from
            const auto factor = !modifiers.get_from_kingdom_column<C>() ? card_data::kingdom::column_factor<C>() : 1u; // assign pile factor if not already in kingdom
            const auto new_disp = new_dispatch<C, false>(dispatch); // update dispatch without approaching army
            const auto new_modifiers = modifiers.with_set_column<C, false>(); // update modifiers: set column bool to true
            if constexpr (C == card_data::kingdom::CardType::Omen) { // in case young witch picks omen
                if(!modifiers.get_from_kingdom_column<C>()){ // omen not already in kingdom
                    const auto new_disp_with_aa = new_dispatch<C, true>(dispatch); // update dispatch with approaching army
                    const auto new_modifiers_with_aa = modifiers.with_set_column<C, true>(); // update modifiers: set column bool to true and add approaching army
                    return num_picks * factor * (14u * extra_setup_pick_impl(subArrayCombination, new_disp, new_modifiers, new_pick_state) + // 14 ways to not pick approaching army
                                                 extra_setup_pick_impl(subArrayCombination, new_disp_with_aa, new_modifiers_with_aa, new_pick_state)); // 1 way to pick approaching army

                }
            }
            return num_picks * factor * extra_setup_pick_impl(subArrayCombination, new_disp, new_modifiers, new_pick_state);
        });
    } else if (current_card == card_data::extra_setup::CardType::Ferryman) { // ferryman
        return UnusedTableElements<card_data::kingdom::row_index_sequence_ferryman_t>::fold(subArrayCombination, picks_state, [&]<std::size_t I, card_data::kingdom::CardType C>(auto num_picks){
            const auto new_pick_state = picks_state.with_added_picker<card_data::extra_setup::CardType::Ferryman>(I, C); // update which table cell ferryman picks from
            const auto factor = !modifiers.get_from_kingdom_column<C>() ? card_data::kingdom::column_factor<C>() : 1u; // assign pile factor if not already in kingdom
            const auto new_disp = new_dispatch<C, false>(dispatch); // update dispatch without approaching army
            const auto new_modifiers = modifiers.with_set_column<C, false>(); // update modifiers: set column bool to true
            if constexpr (C == card_data::kingdom::CardType::Omen) { // in case young witch picks omen
                if(!modifiers.get_from_kingdom_column<C>()){ // omen not already in kingdom
                    const auto new_disp_with_aa = new_dispatch<C, true>(dispatch); // update dispatch with approaching army
                    const auto new_modifiers_with_aa = modifiers.with_set_column<C, true>(); // update modifiers: set column bool to true and add approaching army
                    return num_picks * factor * (14u * extra_setup_pick_impl(subArrayCombination, new_disp, new_modifiers, new_pick_state) + // 14 ways to not pick approaching army
                                                 extra_setup_pick_impl(subArrayCombination, new_disp_with_aa, new_modifiers_with_aa, new_pick_state)); // 1 way to pick approaching army

                }
            }
            return num_picks * factor * extra_setup_pick_impl(subArrayCombination, new_disp, new_modifiers, new_pick_state);
        });
    } else if (current_card == card_data::extra_setup::CardType::Riverboat) { // riverboat
        return UnusedTableElements<card_data::kingdom::row_index_sequence_riverboat_t>::fold(subArrayCombination, picks_state, [&]<std::size_t I, card_data::kingdom::CardType C>(auto num_picks){
            const auto new_pick_state = picks_state.with_added_picker<card_data::extra_setup::CardType::Riverboat>(I, C); // update which table cell riverboat picks from
            const auto factor = !modifiers.get_from_kingdom_column<C>() ? card_data::kingdom::column_factor<C>() : 1u; // assign pile factor if not already in kingdom
            const auto new_disp = new_dispatch<C, false>(dispatch); // update dispatch without approaching army
            const auto new_modifiers = modifiers.with_set_column<C, false>(); // update modifiers: set column bool to true
            if constexpr (C == card_data::kingdom::CardType::Omen) { // in case young witch picks omen
                if(!modifiers.get_from_kingdom_column<C>()){ // omen not already in kingdom
                    const auto new_disp_with_aa = new_dispatch<C, true>(dispatch); // update dispatch with approaching army
                    const auto new_modifiers_with_aa = modifiers.with_set_column<C, true>(); // update modifiers: set column bool to true and add approaching army
                    return num_picks * factor * (14u * extra_setup_pick_impl(subArrayCombination, new_disp, new_modifiers, new_pick_state) + // 14 ways to not pick approaching army
                                                 extra_setup_pick_impl(subArrayCombination, new_disp_with_aa, new_modifiers_with_aa, new_pick_state)); // 1 way to pick approaching army

                }
            }
            return num_picks * factor * extra_setup_pick_impl(subArrayCombination, new_disp, new_modifiers, new_pick_state);
        });
    } else { // obelisk
        // we've added everything here, so we're done, except for:
        // 1. Obelisk can pick any of:
        //      - the 10 kingdom cards
        //      - any of the cards added by other special setup cards
        //      - or the ruins pile if looter is in kingdom
        // 2. if druid has not been added upstream but other fate cards have, we add the fate pile here (12!)

        const auto num_obelisk_picks = 10uz + picks_state.num_added_to_supply() + (uint8_t)(modifiers.has_looter);
        const auto fate_no_druid_factor = (uint8_t)(modifiers.has_fate && !modifiers.has_druid) ? 479001600ul : 1u;
        return num_obelisk_picks * fate_no_druid_factor;
    }
}

auto num_ways_to_pick_extra_setup_cards(const sub_array_combination_t& subArrayCombination, const card_data::CombinationModifiers& state) -> result_t {
    card_data::extra_setup::dispatch_vector_t dispatch{};
    dispatch.reserve(6);
    if (state.has_young_witch) {
        dispatch.push_back(card_data::extra_setup::CardType::YoungWitch);
    }
    if (state.has_approaching_army) {
        dispatch.push_back(card_data::extra_setup::CardType::ApproachingArmy);
    }
    if (state.has_way_of_the_mouse) {
        dispatch.push_back(card_data::extra_setup::CardType::WayOfTheMouse);
    }
    if (state.has_ferryman) {
        dispatch.push_back(card_data::extra_setup::CardType::Ferryman);
    }
    if (state.has_riverboat) {
        dispatch.push_back(card_data::extra_setup::CardType::Riverboat);
    }
    if (state.has_obelisk) {
        dispatch.push_back(card_data::extra_setup::CardType::Obelisk);
    }

    return extra_setup_pick_impl(subArrayCombination, dispatch, state, {});
}

static unsigned num_leaf_calls = 0u;
static constexpr unsigned kNumLeafCalls = 10000000u;
static std::chrono::steady_clock::duration total{};

auto landscape_and_picker_factors(const sub_array_combination_t& subArrayCombination, const SubArrayCombinationState& state) -> result_t {
    const auto& n_at = state.action_treasure_subtotal;

    if (!state.modifiers.has_loot){
        const auto states_after_landscapes = state.modifiers.non_loot_states();
        const auto picker_nl0 = num_ways_to_pick_extra_setup_cards(subArrayCombination, states_after_landscapes[0]);
        const auto picker_nl1 = num_ways_to_pick_extra_setup_cards(subArrayCombination, states_after_landscapes[1]);
        const auto picker_nl2 = num_ways_to_pick_extra_setup_cards(subArrayCombination, states_after_landscapes[2]);
        const auto picker_nl3 = num_ways_to_pick_extra_setup_cards(subArrayCombination, states_after_landscapes[3]);
        const auto picker_l0 = num_ways_to_pick_extra_setup_cards(subArrayCombination, states_after_landscapes[4]);
        const auto picker_l1 = num_ways_to_pick_extra_setup_cards(subArrayCombination, states_after_landscapes[5]);
        const auto picker_l2 = num_ways_to_pick_extra_setup_cards(subArrayCombination, states_after_landscapes[6]);

        if (n_at == 0){
            return (11027u * picker_nl0 +
                    149u * (picker_nl1 + picker_nl2) +
                    1u * picker_nl3 +
                    755u * picker_l0 +
                    5u * (picker_l1 + picker_l2));
        } else if (n_at == 1){
            return (10984u * picker_nl0 +
                    148u * (picker_nl1 + picker_nl2) +
                    1u * picker_nl3 +
                    903u * picker_l0 +
                    6u * (picker_l1 + picker_l2));
        } else {
            const auto n_at_two = n_at*(n_at-1u);
            return ((8912u + 1876u * n_at + 91u * n_at_two) * picker_nl0 +
                    (134u + 14u * n_at) * (picker_nl1 + picker_nl2) +
                    1u * picker_nl3 +
                    (680u + 209u * n_at + 14u * n_at_two) * picker_l0 +
                    (5u + 1u * n_at) * (picker_l1 + picker_l2));
        }
    } else {
        const auto states_after_landscapes = state.modifiers.loot_states();
        const auto picker_l0 = num_ways_to_pick_extra_setup_cards(subArrayCombination, states_after_landscapes[0]);
        const auto picker_l1 = num_ways_to_pick_extra_setup_cards(subArrayCombination, states_after_landscapes[1]);
        const auto picker_l2 = num_ways_to_pick_extra_setup_cards(subArrayCombination, states_after_landscapes[2]);
        const auto picker_l3 = num_ways_to_pick_extra_setup_cards(subArrayCombination, states_after_landscapes[3]);

        if (n_at == 1) {
            return (11887u * picker_l0 +
                    154u * (picker_l1 + picker_l2) +
                    1u * picker_l3);
        } else {
            const auto n_at_two = n_at*(n_at-1u);
            return ((9592u + 2085 * n_at + 105u * n_at_two) * picker_l0 +
                    (139u + 15u * n_at) * (picker_l1 + picker_l2) +
                    1u * picker_l3);
        }
    }
}

//#define COMB_ONLY 0

template<card_data::kingdom::CardType C>
#ifndef COMB_ONLY
auto comb_total(uint8_t s, sub_array_combination_t& subArrayCombination, const SubArrayCombinationState& state)-> result_t {
#else
auto comb_total(uint8_t s) -> result_t {
#endif
    constexpr std::size_t J = std::to_underlying(C);
    if constexpr (C == card_data::kingdom::CardType::None) {
        result_t result = 0u;

        for (const auto& sa : sub_array_vectors().at(J).sub_arrays_with_subtotal(s)) {
            num_leaf_calls++;
            subArrayCombination[J] = &sa;
            const auto t1 = std::chrono::steady_clock::now();
            result += sa.binom_product * card_data::kingdom::column_factor<C>() * landscape_and_picker_factors(subArrayCombination, state);
            const auto t2 = std::chrono::steady_clock::now();
            total += t2 - t1;
            if (num_leaf_calls >= kNumLeafCalls){
                fmt::println("Elapsed time for {} leaf calls: {} s", num_leaf_calls, std::chrono::duration_cast<std::chrono::seconds>(total).count());
                exit(0);
            }
        }



        return result;

//        return std::ranges::fold_left(sub_array_vectors().at(std::to_underlying(C)).sub_arrays_with_subtotal(s) | std::views::transform([](const auto& sa){return sa.binom_product;}), result_t{}, std::plus<result_t>{});
    } else {
        constexpr auto NextC = card_data::kingdom::next_card_type(C);
        const auto& sub_array_vector = sub_array_vectors().at(std::to_underlying(C));
        const auto max_subtotal = sub_array_vector.max_subtotal();
        result_t result = 0u;

#ifndef COMB_ONLY
        subArrayCombination[J] = &(*sub_array_vector.sub_arrays_with_subtotal(0).begin());
        result += comb_total<NextC>(static_cast<uint8_t>(s), subArrayCombination, state);
#else
        result += comb_total<NextC>(static_cast<uint8_t>(s));
#endif

        for (auto k = 1uz; k <= std::min(max_subtotal, s); ++k) {
            const uint8_t new_s = static_cast<uint8_t>(s - k);
            for (const auto& sa : sub_array_vector.sub_arrays_with_subtotal(k)) {
#ifndef COMB_ONLY
                const auto new_state_without_aa = new_sub_array_combination_state_from<C, false>(state, sa);
                subArrayCombination[J] = &sa;
                if constexpr(C == card_data::kingdom::CardType::Omen) { // omen
                    // two possibilities: 14x no Approaching Army, 1x with Approaching Army
                    auto new_state_with_aa = new_sub_array_combination_state_from<C, true>(state, sa);
                    result += sa.binom_product * card_data::kingdom::column_factor<C>() *
                            (14u * comb_total<NextC>(static_cast<uint8_t>(new_s), subArrayCombination, new_state_without_aa) +
                                   comb_total<NextC>(static_cast<uint8_t>(new_s), subArrayCombination, new_state_with_aa));
                } else {
                    result += sa.binom_product * card_data::kingdom::column_factor<C>() * comb_total<NextC>(static_cast<uint8_t>(new_s), subArrayCombination, new_state_without_aa);
                }
#else
                result += sa.binom_product * comb_total<NextC>(static_cast<uint8_t>(new_s));
#endif
            }
        }
        return result;
    }
}

struct Nonzero{
    card_data::kingdom::MembershipMask mask;
    card_data::kingdom::CardType card_type;
    uint8_t amount;
};

using nonzeros_t = std::vector<Nonzero>;

static const nonzeros_t kNonZeros = []{
    nonzeros_t nonzeros{};

    for (const auto& [i, row] : card_data::kingdom::card_type_major_table().rows() | std::views::enumerate){
        for (const auto& [j, amount] : row | std::views::enumerate){
            if(amount > 0) {
                nonzeros.emplace_back(card_data::kingdom::card_type_major_table().row_labels().at(i), card_data::kingdom::CardType(j), amount);
            }
        }
    }

    return nonzeros;
}();

auto main() -> int {
    fmt::println("{}", card_data::kingdom::card_type_major_table());

    for (const auto& [mask, card_type, amount] : kNonZeros){
        fmt::println("{} {:>10} {:>2}", mask, card_type, amount);
    }

//    {
//        const auto t1 = std::chrono::steady_clock::now();
//#ifndef COMB_ONLY
//        sub_array_combination_t sub_array_combination{};
//        SubArrayCombinationState sub_array_combination_modifiers{};
//        const auto result = comb_total<card_data::kingdom::CardType::YoungWitch>(10, sub_array_combination, sub_array_combination_modifiers);
//#else
//        const auto result = comb_total<card_data::kingdom::CardType::YoungWitch>(10);
//#endif
//        const auto t2 = std::chrono::steady_clock::now();
//        fmt::println("Result: {}", result);
//        fmt::println("Elapsed time: {} s", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() / static_cast<double>(1s / 1ms));
//    }
}