#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <boost/multiprecision/integer.hpp>
#include <boost/functional/hash.hpp>

#include <omp.h>
#include <tbb/parallel_for.h>
#include <tbb/global_control.h>
#include <tbb/blocked_range.h>

#include <vector>
#include <ranges>
#include <cstdint>
#include <chrono>

#include <card_data/kingdom/card_type_major_table.hpp>
#include <card_data/extra_setup/state.hpp>
#include <card_data/combination_modifiers.hpp>

#include <utils/math.hpp>
#include <utils/constrained_product_generator.hpp>
#include <utils/result_type.hpp>
#include <array>



struct Nonzero{
    card_data::kingdom::MembershipMask mask;
    card_data::kingdom::CardType card_type;
    uint8_t amount;
};

static constexpr std::size_t kNumNonZeros = 53uz;
using nonzeros_t = std::array<Nonzero, kNumNonZeros>;

//TODO: GENERATE NONZEROS DIRECTLY INSTEAD OF DERIVING FROM TABLE
constexpr auto nonzeros() -> const nonzeros_t& {
    static constexpr nonzeros_t instance = [] static {
        nonzeros_t nonzeros{};

        std::size_t n = 0;
        for (const auto& [j, col] : card_data::kingdom::card_type_major_table().columns() | std::views::enumerate){
            for (const auto& [i, amount] : col | std::views::enumerate){
                if(amount > 0) {
                    nonzeros[n++] = {card_data::kingdom::card_type_major_table().row_labels().at(i), static_cast<card_data::kingdom::CardType>(j), amount};
                }
            }
        }

        return nonzeros;
    }();
    return instance;
}

void print_nonzeros() {
    for (const auto& [i, nonzero] : nonzeros() | std::views::enumerate) {
        const auto& [mask, card_type, amount] = nonzero;
        fmt::print("Nonzero {:>2}: mask: {}, card_type: {:>10}, amount: {:>2}\n", i, mask, card_type, amount);
    }
    fmt::print("");
    std::fflush(stdout);
}

auto binomial_store64() -> const utils::math::BinomialStore64& {
    static const utils::math::BinomialStore64 instance(nonzeros() | std::views::transform([](const auto& non_zero){ return non_zero.amount; }));
    return instance;
}

using search_table_t = std::array<std::array<uint64_t, 11uz>, kNumNonZeros>;

constexpr auto search_table() -> const search_table_t& {
    static constexpr search_table_t instance = [] static {
        search_table_t search_table{};
        search_table.back().back() = 1ul;
        for (const auto i : std::views::iota(0uz, kNumNonZeros - 1uz) | std::views::reverse) {
            const auto max_amount = std::min(nonzeros()[i+1].amount, static_cast<uint8_t>(10u));
            for (const auto j : std::views::iota(0uz, 11uz)) {
                auto v = 0ul;
                for (auto k = 0uz; k <= max_amount && j + k < 11uz; ++k) {
                    v += search_table[i + 1][j + k];
                }
                search_table[i][j] = v;
            }
        }
        return search_table;
    }();
    return instance;
}

struct KingdomTuple {
    using data_t = std::array<uint8_t, kNumNonZeros>;

    data_t data = {};
    card_data::CombinationModifiers combination_modifiers {};
    uint8_t number_action_or_treasure = 0u;
    uint64_t binom_product = 1ul;
};

constexpr auto from_n(uint64_t n) -> KingdomTuple {
    KingdomTuple result{};
    result.data.fill(0);
    uint8_t s = 0u;
    for (auto i = 0uz; i < kNumNonZeros - 1uz; ++i) {
        const auto max_amount = nonzeros()[i].amount;
        const auto kingdom_card_type = nonzeros()[i].card_type;
        if (s == 10u) {
            break;
        }
        for (uint8_t j = 0u; j <= max_amount && s + j <= 10u; ++j) {
            if (search_table()[i][j] > n) {
                result.data[i] = j;
                if (j > 0) {
                    result.combination_modifiers.set_kingdom_column(kingdom_card_type);
                }
                s += j;
                break;
            }
            n -= search_table()[i][j];
        }
    }
    result.data.back() = 10u - s;

    for (auto i = 0uz; i < kNumNonZeros; ++i) {
        result.binom_product *= utils::math::binomial<uint64_t>(nonzeros()[i].amount, result.data[i]);
        if (nonzeros()[i].mask.is_action_or_treasure_region) {
            result.number_action_or_treasure += result.data[i];
        }
    }
    return result;
}

static constexpr std::size_t kNumModifierCombinations = (1u << 7u);
using compute_result_type_t = uint64_t;
using compute_result_t = std::array<compute_result_type_t, kNumModifierCombinations>;

constexpr auto new_combination_modifiers(const card_data::CombinationModifiers& combination_modifiers, card_data::kingdom::CardType kingdom_card_type) -> card_data::CombinationModifiers {
    card_data::CombinationModifiers result = combination_modifiers;
    result.set_kingdom_column(kingdom_card_type);
    return result;
}

constexpr auto new_dispatch(uint8_t dispatch, std::size_t extra_setup_index, card_data::kingdom::CardType c) -> uint8_t {
    const auto result = dispatch ^ (1u << extra_setup_index);
    if (c == card_data::kingdom::CardType::YoungWitch) {
        return result | (1u << std::to_underlying(card_data::extra_setup::CardType::YoungWitch));
    }
    if (c == card_data::kingdom::CardType::Ferryman) {
        return result | (1u << std::to_underlying(card_data::extra_setup::CardType::Ferryman));
    }
    if (c == card_data::kingdom::CardType::Riverboat) {
        return result | (1u << std::to_underlying(card_data::extra_setup::CardType::Riverboat));
    }
    return result;
}

void impl(uint8_t dispatch, card_data::extra_setup::State picks, const KingdomTuple::data_t& tuple_data, const card_data::CombinationModifiers& combination_modifiers, compute_result_type_t f, compute_result_t& result) {
    if (dispatch == 0u) {
        const auto pile_mask = combination_modifiers.to_pile_mask();
#ifdef OVERFLOW_CHECKED
        if ( f > (std::numeric_limits<compute_result_type_t>::max() - result[pile_mask])) {
            std::cout << "Overflow in addition" << std::endl;
            exit(1);
        }
#endif
        result[pile_mask] += f;

    } else {
        const std::size_t extra_setup_index = std::countr_zero(dispatch);
        const auto extra_setup_card_type = static_cast<card_data::extra_setup::CardType>(extra_setup_index);

        // TODO: make function in extra setup ns
        const bool is_picking_unused = (extra_setup_card_type != card_data::extra_setup::CardType::Obelisk);

        if (is_picking_unused) {
            bool has_choice = false;

            for ( const auto& [i, tuple_elem] : tuple_data | std::views::enumerate) {
                if (card_data::kingdom::MembershipMask::ToUnsigned(nonzeros()[i].mask) & (1u << extra_setup_index)) {
                    if (const auto available_amount = nonzeros()[i].amount - tuple_elem - picks.num_unused_added_from(i); available_amount > 0) {
                        has_choice = true;
                        const auto kingdom_card_type = nonzeros()[i].card_type;
                        const auto new_cm = new_combination_modifiers(combination_modifiers, kingdom_card_type);
                        const auto new_disp = new_dispatch(dispatch, extra_setup_index, kingdom_card_type);

#ifdef OVERFLOW_CHECKED
                        if ( f > (std::numeric_limits<compute_result_type_t>::max() / available_amount)) {
                            std::cout << "Overflow in multiplication" << std::endl;
                            exit(1);
                        }
#endif

                        const auto new_f = f * available_amount;
                        const auto new_picks = picks.with_added_picker(extra_setup_card_type, i);

                        impl(new_disp, new_picks, tuple_data, new_cm, new_f, result);
                    }
                }
            }

            if (!has_choice) {
                const auto new_disp = dispatch ^ (1u << extra_setup_index);
                impl(new_disp, picks, tuple_data, combination_modifiers, f, result);
            }

        } else {

            auto num_obelisk_choices = 0u;

            for ( const auto& [i, tuple_elem] : tuple_data | std::views::enumerate) {
                if (card_data::kingdom::MembershipMask::ToUnsigned(nonzeros()[i].mask) & (1u << extra_setup_index)) {
                    num_obelisk_choices += tuple_elem + picks.num_used_added_at(i);
                }
            }

            if (num_obelisk_choices == 0) { // if no obelisk choices, then we don't let obelisk pick
                num_obelisk_choices = 1;
            }

#ifdef OVERFLOW_CHECKED
            if ( f > (std::numeric_limits<compute_result_type_t>::max() / num_obelisk_choices)) {
                std::cout << "Overflow in multiplication" << std::endl;
                exit(1);
            }
#endif

            const auto new_f = f * num_obelisk_choices;
            const auto new_dispatch = dispatch ^ (1u << extra_setup_index);

            impl(new_dispatch, picks, tuple_data, combination_modifiers, new_f, result);
        }
    }
}

void bar(const KingdomTuple::data_t& tuple_data, const card_data::CombinationModifiers& combination_modifiers, compute_result_type_t f, compute_result_type_t* result) {
    uint8_t dispatch = 0u;
    if (combination_modifiers.has_young_witch) {
        dispatch |= (1u << std::to_underlying(card_data::extra_setup::CardType::YoungWitch));
    }
    if (combination_modifiers.has_way_of_the_mouse) {
        dispatch |= (1u << std::to_underlying(card_data::extra_setup::CardType::WayOfTheMouse));
    }
    if (combination_modifiers.has_ferryman) {
        dispatch |= (1u << std::to_underlying(card_data::extra_setup::CardType::Ferryman));
    }
    if (combination_modifiers.has_riverboat) {
        dispatch |= (1u << std::to_underlying(card_data::extra_setup::CardType::Riverboat));
    }
    if (combination_modifiers.has_obelisk) {
        dispatch |= (1u << std::to_underlying(card_data::extra_setup::CardType::Obelisk));
    }

    compute_result_t incr{};

    if((dispatch & 0b011011)  != 0u){
        #pragma omp critical
        {
            fmt::println("Non-trivial dispatch {:6b} at thread {}", dispatch, omp_get_thread_num());
        }
    }
    impl(dispatch, {}, tuple_data, combination_modifiers, f, incr);

    if (combination_modifiers.has_omen) {
        dispatch |= (1u << std::to_underlying(card_data::extra_setup::CardType::ApproachingArmy));
        std::ranges::for_each(incr, [](auto& elem){elem *= 14;});
        impl(dispatch, {}, tuple_data, combination_modifiers, f, incr);
    }

    for (auto i = 0uz; i < kNumModifierCombinations; ++i) {
        result[i] += incr[i];
    }
}

using non_loot_factors_t = std::array<unsigned, 5>;
using loot_factors_t = std::array<unsigned, 3>;
using modifier_combination_index_factors_t = std::array<result_t, kNumModifierCombinations>;

constexpr auto non_loot_factors(const unsigned n) -> const non_loot_factors_t& {
    static constexpr std::array<non_loot_factors_t, 11uz> store = [] {
        std::array<non_loot_factors_t, 11uz> result{};
        result[0] = {11027u, 149u, 1u, 755u, 5u};
        result[1] = {10984u, 148u, 1u, 903u, 6u};
        for (auto i = 2u; i < 11u; ++i) {
            const auto i2 = i*(i-1);
            result[i] = {8912u + 1876u*i + 91u*i2, 134u + 14u*i, 1u, 680u + 209u*i + 14*i2, 5u + i};
        }
        return result;
    }();
    return store[n];
}

constexpr auto loot_factors(const unsigned n) -> const loot_factors_t& {
    static constexpr std::array<loot_factors_t, 11uz> store = [] {
        std::array<loot_factors_t, 11uz> result{};
        result[1] = {11887u, 154u, 1u};
        for (auto i = 2u; i < 11u; ++i) {
            const auto i2 = i*(i-1);
            result[i] = {9592u  + 2085u*i + 105u*i2, 139u + 15u*i, 1u};
        }
        return result;
    }();
    if (n == 0) {
        throw std::runtime_error("Loot factors without action or treasure shouldn't happen");
    }
    return store[n];
}

auto modifier_combination_index_factors() -> const modifier_combination_index_factors_t& {
    static const modifier_combination_index_factors_t instance = [] {
        modifier_combination_index_factors_t result{};
        result.fill(1u);
        for (auto i = 0u; i < kNumModifierCombinations; ++i) {
            if (i & 0x1u) {
                result[i] *= card_data::kingdom::column_factor<card_data::kingdom::CardType::Knights>();
            }
            if (i & 0x2u) {
                result[i] *= card_data::kingdom::column_factor<card_data::kingdom::CardType::Druid>();
            }
            if (i & 0x4u) {
                result[i] *= card_data::kingdom::column_factor<card_data::kingdom::CardType::Looter>();
            }
            if (i & 0x8u) {
                result[i] *= card_data::kingdom::column_factor<card_data::kingdom::CardType::Fate>();
            }
            if (i & 0x10u) {
                result[i] *= card_data::kingdom::column_factor<card_data::kingdom::CardType::Doom>();
            }
            if (i & 0x20u) {
                result[i] *= card_data::kingdom::column_factor<card_data::kingdom::CardType::Liaison>();
            }
            if (i & 0x40u) {
                result[i] *= card_data::kingdom::column_factor<card_data::kingdom::CardType::Loot>();
            }
        }
        return result;
    }();
    return instance;
}

void foo(const KingdomTuple& kingdom_tuple, compute_result_type_t* output, uint64_t* binom_product){
    const unsigned n = kingdom_tuple.number_action_or_treasure;

    for(auto i = 0ul; i < kNumModifierCombinations; ++i){
        output[i] = 0ul;
    }

    if (!kingdom_tuple.combination_modifiers.has_loot) {
        const auto& [nl0, nl1, nl2, nl3, l0, l1, l2] = kingdom_tuple.combination_modifiers.non_loot_states();
        auto& [f_nl0, f_nl12, f_nl3, f_l0, f_l12] = non_loot_factors(n);

        bar(kingdom_tuple.data, nl0, f_nl0, output);
        bar(kingdom_tuple.data, nl1, f_nl12, output);
        bar(kingdom_tuple.data, nl2, f_nl12, output);
        bar(kingdom_tuple.data, nl3, f_nl3, output);
        bar(kingdom_tuple.data, l0, f_l0, output);
        bar(kingdom_tuple.data, l1, f_l12, output);
        bar(kingdom_tuple.data, l2, f_l12, output);

    } else {
        const auto& [l0, l1, l2, l3] = kingdom_tuple.combination_modifiers.loot_states();
        const auto& [f_l0, f_l12, f_l3] = loot_factors(n);
        bar(kingdom_tuple.data, l0, f_l0, output);
        bar(kingdom_tuple.data, l1, f_l12, output);
        bar(kingdom_tuple.data, l2, f_l12, output);
        bar(kingdom_tuple.data, l3, f_l3, output);
    }

    *binom_product = kingdom_tuple.binom_product;
}

static constexpr auto kBatchSize = 1'000'000ul;
static constexpr auto kNumBatchChunks = 100u;
//static constexpr auto kBatchChunkSize = kBatchSize / kNumBatchChunks;

auto do_batch(const uint64_t batch_num, const uint64_t batch_size, std::vector<uint64_t>& output_per_modifier_combination, std::vector<uint64_t>& binom_products, std::vector<result_t>& result, std::chrono::steady_clock::duration& total_time) {
    fmt::println("Batch {}:", batch_num);

    const auto t1 = std::chrono::steady_clock::now();
    const auto batch_chunk_size = std::max( 1ul, batch_size / kNumBatchChunks );

    #pragma omp parallel for schedule(dynamic,batch_chunk_size) default(none) shared(output_per_modifier_combination, binom_products, batch_chunk_size, batch_size, batch_num)
    for (auto i = 0ul; i < batch_size; ++i) {
        const auto n = kBatchSize * batch_num + i;
        foo(from_n(n), output_per_modifier_combination.data() + kNumModifierCombinations * i, binom_products.data() + i);
    }

    const auto t2 = std::chrono::steady_clock::now();
    const auto populate_dt = t2 - t1;
    total_time += populate_dt;
    fmt::println("\tPopulated: {}", std::chrono::duration_cast<std::chrono::milliseconds>(populate_dt).count() / 1e3);

    const auto t3 = std::chrono::steady_clock::now();
    #pragma omp parallel for schedule(dynamic,batch_chunk_size) default(none) shared(output_per_modifier_combination, result, batch_size, batch_chunk_size)
    for(auto i = 0ul; i < batch_size; ++i){
        result_t incr{0u};
        for(auto j = 0ul; j < kNumModifierCombinations; ++j){
            incr += output_per_modifier_combination[i * kNumModifierCombinations + j] * modifier_combination_index_factors()[j];
        }
        const auto tid = omp_get_thread_num();
        result[tid] += incr;
    }
    const auto t4 = std::chrono::steady_clock::now();
    const auto reduce_dt = t4 - t3;
    total_time += reduce_dt;
    fmt::println("\tReduced: {}", std::chrono::duration_cast<std::chrono::milliseconds>(reduce_dt).count() / 1e3);
}

auto main(int argc, const char** argv) -> int {
    static constexpr auto kMaxIterations = search_table()[0][0] + search_table()[0][1];


    const auto argspan = std::span(argv, argc);

    auto num_iterations = kMaxIterations;
    if(argspan.size() > 1){
        try {
            num_iterations = std::stoull(argspan[1]);
            if (num_iterations > kMaxIterations){
                throw std::out_of_range("");
            }
        } catch (const std::invalid_argument& e){
            fmt::println("First parameter is not a number.");
            exit(2);
        } catch (const std::out_of_range& e){
            fmt::println("Number of iterations out of range");
            exit(2);
        }
    }

    std::vector<result_t> result{};
    result.resize(omp_get_max_threads());

    const auto num_batches = num_iterations / kBatchSize;
    const auto remainder_batch_size = num_iterations - kBatchSize * num_batches;
    std::vector<compute_result_type_t> output_per_modifier_combination(kNumModifierCombinations * kBatchSize, 0ul);
    std::vector<uint64_t> binom_products(kBatchSize, 0ul);

    std::chrono::steady_clock::duration total_time{};

    for (auto b = 0u; b < num_batches; ++b){
        do_batch(b, kBatchSize, output_per_modifier_combination, binom_products, result, total_time);
    }
    if (remainder_batch_size > 0){
        do_batch(num_batches, remainder_batch_size, output_per_modifier_combination, binom_products, result, total_time);
    }

    const auto answer = std::ranges::fold_left(result, result_t{}, std::plus<result_t>{});

    if(num_iterations == kMaxIterations){
        fmt::println("Solution found in {:%H hours %M minutes %S seconds}", std::chrono::duration_cast<std::chrono::seconds>(total_time));
        fmt::println("Answer: {}", answer);
    } else {
        fmt::println("Processed {} tuples in {} s", num_iterations, std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count() / 1e3);
    }
}