#pragma once

#include <generator>
#include <unordered_map>

#include "tuple.hpp"
#include "math.hpp"

namespace utils::generators {

    enum class Constraint : uint8_t {
        EQ,
        LE,
    };

    template<Constraint C, typename Tuple, size_t I, typename U = std::tuple_element<0, Tuple>::type>
    std::generator<Tuple> constrained_product_impl(const Tuple& boundary, U s, Tuple& current_elements, U current_sum, U max_added_sum) {
        if constexpr (I == std::tuple_size_v<Tuple> - 1) {
            if constexpr (C == Constraint::EQ) {
                std::get<I>(current_elements) = s - current_sum;
                co_yield current_elements;
            } else if constexpr (C == Constraint::LE) {
                for (auto i = (U)0; i <= std::min(std::get<I>(boundary), (U)(s - current_sum)); ++i) {
                    std::get<I>(current_elements) = i;
                    co_yield current_elements;
                }
            } else {
                static_assert(false, "Constraint not implemented");
            }
        } else {
            const U lower_bound = [&](){
                if constexpr(C == Constraint::EQ){
                   return max_added_sum + current_sum <= s ? s - max_added_sum - current_sum : 0u;
                } else if constexpr (C == Constraint::LE){
                    return 0u;
                } else {
                   static_assert(false, "Constraint not implemented");
                }
            }();
            const U upper_bound = std::min(std::get<I>(boundary), (U)(s - current_sum));
            for (auto i = lower_bound; i <= upper_bound; ++i) {
                std::get<I>(current_elements) = i;
                for (const auto& elem : constrained_product_impl<C, Tuple, I + 1, U>(boundary, s, current_elements,
                                                                                     current_sum + i,
                                                                                     max_added_sum -
                                                                                     std::get<I + 1>(boundary))) {
                    co_yield elem;
                }
            }
        }
    }

    template<Constraint C, typename Tuple, typename U = std::tuple_element<0, Tuple>::type>
    std::generator<Tuple> constrained_product(const Tuple& boundary, U s) {
        Tuple arr{};
        for (const auto& elem : constrained_product_impl<C, Tuple, 0uz, U>(boundary, s, arr, 0,
                                                                           utils::tuple::reverse_partial_sum<
                                                                                   std::tuple_size_v<Tuple> - 1>(
                                                                                   boundary))) {
            co_yield elem;
        }
    }

    template<typename V, size_t N>
    V count_combinations1(const std::array<uint8_t, N>& a, uint8_t s){
        auto impl = [&a, s](this auto self, std::size_t i, uint8_t current_sum, uint32_t max_added_sum){
//            using index_t = std::pair<std::size_t, uint8_t>;
//            static std::unordered_map<index_t, V> cache{};
            if(i == N - 1){
                return V{1};
            }
            const uint8_t lo = max_added_sum + current_sum <= s ? s - max_added_sum - current_sum : 0u;
            const uint8_t hi = std::min(a[i], (uint8_t)(s - current_sum));
            V subtotal{};
            for (uint8_t k = lo; k <= hi; ++k){
                subtotal += self(i+1, current_sum + k, max_added_sum - a[i+1]);
            }
            return subtotal;
        };
        const auto a_sum_ex_first = std::ranges::fold_left(a | std::views::drop(1), 0u, std::plus{});
        return impl(0u, 0u, a_sum_ex_first);
    }

    struct index_hash {
        constexpr std::size_t operator()(const std::pair<std::size_t, uint8_t>& p) const noexcept {
            return p.second + (p.first << 8u);
        }
        constexpr std::size_t operator()(const std::pair<std::size_t, uint64_t>& p) const noexcept {
            return p.second ^ p.first;
        }
    };

    template<typename V, typename E, size_t N>
    V count_combinations2(const std::array<E, N>& a, uint8_t s){
        auto impl = [&a, s](this auto self, std::size_t i, uint8_t current_sum, uint32_t max_added_sum){
            using index_t = std::pair<std::size_t, uint8_t>;
            static std::unordered_map<index_t, V, index_hash> cache{};
            if(i == N - 1){
                return V{1};
            }
            const auto index = std::make_pair(i, current_sum);
            if(auto it = cache.find(index); it != cache.end()){
                return it->second;
            }
            const E lo = max_added_sum + current_sum <= s ? s - max_added_sum - current_sum : 0u;
            const E hi = std::min(a[i], (E)(s - current_sum));
            V subtotal{};
            for (E k = lo; k <= hi; ++k){
                subtotal += self(i+1, current_sum + k, max_added_sum - a[i+1]);
            }
            cache[index] = subtotal;
            return subtotal;
        };
        const auto a_sum_ex_first = std::ranges::fold_left(a | std::views::drop(1), 0u, std::plus{});
        return impl(0u, 0u, a_sum_ex_first);
    }

    template<typename V, typename E, size_t N>
    V sum_binom_products1(const std::array<E, N>& a, uint8_t s){
        auto impl = [&a, s](this auto self, std::size_t i, uint8_t current_sum, uint32_t max_added_sum){
            using index_t = std::pair<std::size_t, uint8_t>;
            static std::unordered_map<index_t, V, index_hash> cache{};
            if(i == N - 1){
                return utils::math::binomial<V>(a[i], (E)(s - current_sum));
            }
            const auto index = std::make_pair(i, current_sum);
            if(auto it = cache.find(index); it != cache.end()){
                return it->second;
            }
            const E lo = max_added_sum + current_sum <= s ? s - max_added_sum - current_sum : 0u;
            const E hi = std::min(a[i], (E)(s - current_sum));
            V subtotal{};
            for (E k = lo; k <= hi; ++k){
                subtotal += utils::math::binomial<V>(a[i], k) * self(i+1, current_sum + k, max_added_sum - a[i+1]);
            }
            cache[index] = subtotal;
            return subtotal;
        };
        const auto a_sum_ex_first = std::ranges::fold_left(a | std::views::drop(1), 0u, std::plus{});
        return impl(0u, 0u, a_sum_ex_first);
    }

}