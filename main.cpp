#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <boost/multiprecision/integer.hpp>
#include <boost/functional/hash.hpp>

#include <tbb/parallel_pipeline.h>

#include <vector>
#include <ranges>
#include <cstdint>
#include <chrono>
#include <unordered_map>
#include <generator>

#include <card_data/kingdom/card_type_major_table.hpp>
#include <card_data/extra_setup/state.hpp>
#include <card_data/combination_modifiers.hpp>

#include <utils/math.hpp>
#include <utils/filtered_index_sequence.hpp>
#include <utils/constrained_product_generator.hpp>
#include <utils/result_type.hpp>
#include <stack>
#include <array>
#include <build/debug-gcc-14/vcpkg_installed/x64-linux/include/gtest/internal/gtest-type-util.h>

static constexpr auto kNumIterations = 1'000'000ul;
static constexpr auto kNumLiveTokens = 11u;

struct Nonzero{
    card_data::kingdom::MembershipMask mask;
    card_data::kingdom::CardType card_type;
    uint8_t amount;
};

static constexpr std::size_t kNumNonZeros = 53uz;
using nonzeros_t = std::array<Nonzero, kNumNonZeros>;
using nonzero_card_type_offsets_t = std::array<std::size_t, card_data::kingdom::kNumCardTypes + 1uz>;

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

using tuple_t = std::array<uint8_t, kNumNonZeros>;

constexpr auto from_n(uint64_t n) -> tuple_t {
    tuple_t result{};
    uint8_t s = 0u;
    for (auto i = 0uz; i < kNumNonZeros - 1uz; ++i) {
        const auto max_amount = nonzeros()[i].amount;
        if (s == 10u) {
            return result;
        }
        for (uint8_t j = 0u; j <= max_amount && s + j <= 10u; ++j) {
            if (search_table()[i][j] > n) {
                result[i] = j;
                s += j;
                break;
            }
            n -= search_table()[i][j];
        }
    }
    result.back() = 10u - s;
    return result;
}

template<uint8_t V>
using integral_byte_constant = std::integral_constant<uint8_t, V>;

template<card_data::kingdom::CardType KCT>
using span_size = integral_byte_constant<std::ranges::count(nonzeros(), KCT, &Nonzero::card_type)>;

template<card_data::kingdom::CardType KCT>
using span_offset = integral_byte_constant<std::ranges::distance(std::cbegin(nonzeros()), std::ranges::find(nonzeros(), KCT, &Nonzero::card_type))>;

template<card_data::kingdom::CardType KCT>
constexpr auto nonzero_span() -> const std::span<const Nonzero>& {
    static constexpr auto result = std::span(nonzeros()).subspan(span_offset<KCT>::value, span_size<KCT>::value);
    return result;
}

const auto binomial_store64() -> const utils::math::BinomialStore64& {
    static const utils::math::BinomialStore64 instance(nonzeros() | std::views::transform([](const auto& non_zero){ return non_zero.amount; }));
    return instance;
}

template<card_data::kingdom::CardType KCT>
static const auto num_subtuples_per_subtotal() -> const std::array<std::size_t, 11uz>& {
    static const std::array<std::size_t, 11uz> instance = [] static {
        auto impl = [](this auto&& self, uint8_t s, std::size_t i) {
            if (i == nonzero_span<KCT>().size()) {
                return s == 0uz ? 1uz : 0uz;
            }
            std::size_t result = 0u;
            for (auto k = 0uz; k <= std::min(s, nonzero_span<KCT>()[i].amount); ++k) {
                result += self(s - k, i + 1);
            }
            return result;
        };

        std::array<std::size_t, 11uz> result;
        for (uint8_t s = 0u; s < 11uz; ++s) {
            result[s] = impl(s, 0uz);
        }
        return result;
    }();
    return instance;
}

template<card_data::kingdom::CardType KCT>
static const auto num_subtuples() -> std::size_t {
    static const std::size_t instance = std::ranges::fold_left(num_subtuples_per_subtotal<KCT>(), 0uz, std::plus<std::size_t>{});
    return instance;
}

// consteval uint64_t foo_val(std::size_t i, uint8_t s) {
//     if (i == std::size(nonzeros())) {
//         return s == 10ul ? 1ul : 0ul;
//     }
//     const auto max_amount = nonzeros()[i].amount;
//
// }
//
// template<std::size_t I, std::size_t S>
// struct Foo : std::integral_constant<uint64_t, foo_val(I, S)> {};

template<card_data::kingdom::CardType KCT>
struct Subtuple {
    uint64_t binomial_product;
    uint8_t num_action_or_treasure;
    std::array<uint8_t, span_size<KCT>::value> data;
};

template<card_data::kingdom::CardType KCT>
struct fmt::formatter<Subtuple<KCT>> : public fmt::formatter<std::string> {
    auto format(const Subtuple<KCT>& st, fmt::format_context& ctx) const {
        return fmt::format_to(ctx.out(), "({}, binom_product={}, num_at={})", st.data, st.binomial_product, st.num_action_or_treasure);
    }
};

template<card_data::kingdom::CardType KCT>
class SubtupleVector {
    using subtuple_data_t = decltype(Subtuple<KCT>::data);
    using data_t = std::vector<Subtuple<KCT>>;
    using offsets_t = std::vector<std::size_t>;

    data_t data_{};
    offsets_t offsets_{};

    void populator_impl(data_t& this_data, subtuple_data_t& subtuple_data, uint64_t binomial_product, uint8_t n_at, uint8_t s, std::size_t i) {
        static std::vector<typename data_t::iterator> insert_iterators = [this] {
            std::vector<typename data_t::iterator> result{};
            result.reserve(12uz);
            for (const auto& offset : offsets_) {
                result.push_back(std::begin(data_) + static_cast<std::ptrdiff_t>(offset));
            }
            return result;
        }();
        if (i == span_size<KCT>::value) {
            *insert_iterators[10-s]++ = {binomial_product, n_at, subtuple_data};
        } else {
            const auto max_amount = nonzero_span<KCT>()[i].amount;
            for (uint8_t k = 0u; k <= std::min(s, max_amount); ++k) {
                subtuple_data[i] = k;
                const uint8_t new_n_at = nonzero_span<KCT>()[i].mask.is_action_or_treasure_region ? n_at + k : n_at;
                populator_impl(this_data, subtuple_data, binomial_product * binomial_store64()[max_amount, k], new_n_at, s - k, i + 1);
            }
        }
    }

public:
    SubtupleVector() : data_() {
        const auto t1 = std::chrono::steady_clock::now();
        offsets_.reserve(12uz);
        offsets_.push_back(0uz);
        for (const auto& offset_incr : num_subtuples_per_subtotal<KCT>()) {
            if (offset_incr == 0) {
                break;
            }
            offsets_.push_back(offsets_.back() + offset_incr);
        }

        data_.resize(num_subtuples<KCT>());
        subtuple_data_t subtuple{};
        subtuple.fill(0u);
        populator_impl(data_, subtuple, 1u, 0u, 10u, 0uz);
        const auto t2 = std::chrono::steady_clock::now();
        fmt::println("Generated subtuples for kingdom Card Type {} in: {} us",
            KCT,
            std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count());
    }

    const auto size() const noexcept -> std::size_t {
        return std::size(data_);
    }

    constexpr auto offsets() const noexcept -> const offsets_t& {
        return offsets_;
    }

    constexpr auto max_subtotal() const -> uint8_t {
        return std::size(offsets_) - 2uz;
    }

    constexpr auto with_subtotal(const uint8_t k) const {
        if (k + 1u >= offsets_.size()) {
            const auto end = std::ranges::cend(data_);
            return std::ranges::subrange(end, end, 0uz);
        }
        const auto begin = std::ranges::begin(data_) + static_cast<std::ptrdiff_t>(offsets_[k]);
        const auto end = std::ranges::begin(data_) + static_cast<std::ptrdiff_t>(offsets_[k+1]);
        const auto n = std::ranges::distance(begin, end);
        return std::ranges::subrange(begin, end, n);
    }

};

template<card_data::kingdom::CardType KCT>
const auto subtuples() -> const SubtupleVector<KCT>& {
    static const SubtupleVector<KCT> instance{};
    return instance;
}

template<card_data::kingdom::CardType KCT>
void check_all() {
    fmt::println("{}:", KCT);
    for (auto k = 0uz; k <= subtuples<KCT>().max_subtotal(); ++k) {
        fmt::print("\tSubtotal = {}...", k);
        for (const auto& st : subtuples<KCT>().with_subtotal(k)) {
            const auto subtotal = std::ranges::fold_left(st.data, static_cast<uint8_t>(0), std::plus<uint8_t>{});
            auto binom_product = 1ul;
            auto num_action_or_treasure = 0u;
            for (auto i = 0uz; i < std::size(st.data); ++i) {
                binom_product *= binomial_store64()[nonzero_span<KCT>()[i].amount, st.data[i]];
                if (nonzero_span<KCT>()[i].mask.is_action_or_treasure_region) {
                    num_action_or_treasure += st.data[i];
                }
            }

            if (subtotal != k) {
                fmt::print("{}\tSubtotal mismatch: {} != {}\n", st, subtotal, k);
                exit(1);
            }
            if (binom_product != st.binomial_product) {
                fmt::print("{}\tBinom product mismatch: {} != {}\n", st, binom_product, st.binomial_product);
                exit(1);
            }
            if (num_action_or_treasure != st.num_action_or_treasure) {
                fmt::print("{}\tNum A|T mismatch: {} != {}\n", st, num_action_or_treasure, st.num_action_or_treasure);
                exit(1);
            }
        }
        fmt::println("OK!");
    }
}

template<card_data::kingdom::CardType ... KCT>
void unrolled_check_all(card_data::kingdom::card_type_sequence<KCT...>) {
    (check_all<KCT>(), ...);
}

auto main() -> int {
    for (const auto& nonzero : nonzeros()) {
        fmt::println("{}", nonzero.amount);
    }

    fmt::println("");

    for (const auto& [i, row] : search_table() | std::views::enumerate) {
        fmt::println("{:2}: {}", i, row);
    }

    fmt::println("");

    auto dt = std::chrono::steady_clock::duration{};

    for (const auto i : std::views::iota(0uz, kNumIterations)) {
        const auto t1 = std::chrono::steady_clock::now();
        from_n(i);
        const auto t2 = std::chrono::steady_clock::now();
        dt += t2 - t1;
    }

    fmt::println("Generated {} tuples in {} ms", kNumIterations, std::chrono::duration_cast<std::chrono::milliseconds>(dt).count());

//    {
//        const auto t1 = std::chrono::steady_clock::now();
//        const auto result = first_iterations_serial();
//        const auto t2 = std::chrono::steady_clock::now();
//        fmt::print("result = {}\n", result);
//        fmt::print("Elapsed time: {}\n", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
//    }

//    {
//        const auto t1 = std::chrono::steady_clock::now();
//        const auto result = first_iterations_pipelined();
//        const auto t2 = std::chrono::steady_clock::now();
//        fmt::print("result = {}\n", result);
//        fmt::print("Elapsed time: {}\n", std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
//    }
}