#include <chrono>
#include <print>

#include "generator_bounds.hpp"
#include "default_expansion_editions.hpp"
#include "constrained_product_generator.hpp"


using config::coefficient_t;
using config::GeneratorBounds;
using config::Expansion;
using config::kAllExpansionEditions;
using config::kingdom::ExpansionEditionFilter;
using config::kingdom::EditionModifier;

using V = boost::multiprecision::uint128_t;

template<>
struct std::formatter<V> : std::formatter<std::string> {
    constexpr auto parse(std::format_parse_context& ctx) const {
        return ctx.begin();
    }

    auto format(const V& obj, std::format_context& ctx) const {
        std::ostringstream oss{};
        oss << obj;
        return std::format_to(ctx.out(), "{}", oss.str());
    }
};


auto main() -> int {
//    const auto& expansion_edition_filter = kAllExpansionEditions;
//
//    const auto actual = from_expansions_editions(expansion_edition_filter);
//
//
//    const auto t1 = std::chrono::steady_clock::now();
//    uint64_t i = 0;
//    for (const auto& x : utils::generators::constrained_product<utils::generators::Constraint::EQ>(actual.kingdom_, 10u)){
//        (void)x;
//        ++i;
//    }
//    std::cout << i << '\n';
//    const auto t2 = std::chrono::steady_clock::now();
//    std::println("Elapsed time: {}", std::chrono::duration_cast<std::chrono::seconds>(t2-t1));



//    const std::array<uint8_t, 4> a{2, 11, 4, 6};
    const std::array<uint16_t, 16> a2{98, 16, 4, 1, 3,1,1,246,37,4,3,3,1,4,1,1};
    const std::array<uint8_t, 74> a{1,4,6,2,3,34,53,84,87,28,2,6,12,26,3,1,2,2,1,8,1,1,1,1,1,1,1,3,12,9,12,3,1,1,2,1,2,1,1,2,1,2,1,1,3,1,1,1,1,1,1,2,1,1,3,1,1,1,3,1,1,1,1,1,4,4,6,19,6,1,1,1,1,1};
//    {
//        const auto t1 = std::chrono::steady_clock::now();
//        const auto result = utils::generators::count_combinations2<V>(a, 10);
//        const auto t2 = std::chrono::steady_clock::now();
//        std::println("Iteration space determined:\nResult: {}\nElapsed time: {}\n", result, std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1));
//    }
    {
        const auto t1 = std::chrono::steady_clock::now();
        const auto result = utils::generators::sum_binom_products1<V>(a2, 10);
        const auto t2 = std::chrono::steady_clock::now();
        std::println("Sum of binomial products of old tuple (dynamic programming)\nResult: {}\nElapsed time: {}\n", result, std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1));
    }
    {
        const auto t1 = std::chrono::steady_clock::now();
        const auto result = utils::generators::sum_binom_products1<V>(a, 10);
        const auto t2 = std::chrono::steady_clock::now();
        std::println("Sum of binomial products of new tuple (dynamic programming)\nResult: {}\nElapsed time: {}\n", result, std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1));
    }

}