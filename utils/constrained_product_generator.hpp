#pragma once

#include <generator>
#include <unordered_map>
#include <ranges>

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
                for (auto i = (U)0; i <= std::min((U)std::get<I>(boundary), (U)(s - current_sum)); ++i) {
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
            const U upper_bound = std::min((U)std::get<I>(boundary), (U)(s - current_sum));
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
        std::array<uint16_t, N+1> reverse_partial_sum{};
        for(const auto& i : std::views::iota(0u, N) | std::views::reverse){
            reverse_partial_sum[i] = reverse_partial_sum[i+1] + a[i];
        }
        auto impl = [&a, &reverse_partial_sum, s](this auto self, std::size_t i, uint8_t current_sum){
            using index_t = std::pair<std::size_t, uint8_t>;
            static std::unordered_map<index_t, V, index_hash> cache{};
            if(i == N - 1){
                return utils::math::binomial<V>(a[i], (E)(s - current_sum));
            }
            const auto index = std::make_pair(i, current_sum);
            if(auto it = cache.find(index); it != cache.end()){
                return it->second;
            }
            const E lo = reverse_partial_sum[i+1] + current_sum <= s ? s - reverse_partial_sum[i+1] - current_sum : 0u;
            const E hi = std::min(a[i], (E)(s - current_sum));
            V subtotal{};
            for (E k = lo; k <= hi; ++k){
                subtotal += utils::math::binomial<V>(a[i], k) * self(i+1, current_sum + k);
            }
            cache[index] = subtotal;
            return subtotal;
        };
        return impl(0u, 0u);
    }

    template<size_t ... I, size_t ... J>
    uint8_t get_membership(size_t k, std::index_sequence<I...>, std::index_sequence<J...>){
        auto r = (uint8_t)(... || (I == k));
        r |= (uint8_t)(... || (J == k)) << 1u;
        return r;
    }

    struct DPState {
        std::size_t i;
        uint8_t cs;
        std::array<uint16_t, 3> ie_membership;

        constexpr auto operator==(const DPState&) const noexcept -> bool = default;
    };

    uint64_t inclusion_exlusion(const std::array<uint16_t,3>& ie){
        return std::max((uint64_t)((ie[0] + ie[2]) * (ie[1] + ie[2]) - ie[2]), 1ul);
    }

    struct DPStateHash{
        constexpr std::size_t operator()(const DPState& x) const noexcept {
            size_t r = (x.i << 8u) + x.cs;
            r |= ((size_t)x.ie_membership[0] << 16u);
            r |= ((size_t)x.ie_membership[1] << 32u);
            r |= ((size_t)x.ie_membership[2] << 48u);
            return r;
        }
    };

    template<typename V, typename E, size_t N>
    V example_dp(const std::array<E, N>& a, uint8_t s){
        std::array<uint8_t, N> ie_memberships{};
        for(auto i = 0uz; i < N; ++i)
            ie_memberships[i] = get_membership(i, std::index_sequence<0,1>{}, std::index_sequence<1,2>{});
        std::array<uint16_t, N+1> reverse_partial_sum{};
        for(const auto& i : std::views::iota(0u, N) | std::views::reverse){
            reverse_partial_sum[i] = reverse_partial_sum[i+1] + a[i];
        }

        auto impl = [&a, &ie_memberships, &reverse_partial_sum, s](this auto self, const DPState& dps){
            static std::unordered_map<DPState, V, DPStateHash> cache{};
            if(dps.i == N){
                return V{1};
            }
            if(auto it = cache.find(dps); it != cache.end()){
                return it->second;
            }
            const E lo = reverse_partial_sum[dps.i+1] + dps.cs <= s ? s - reverse_partial_sum[dps.i+1] - dps.cs : 0u;
            const E hi = std::min(a[dps.i], (E)(s - dps.cs));
            V subtotal{};
            for (E k = lo; k <= hi; ++k){
                auto new_dps = dps;
                new_dps.i = dps.i + 1u;
                if(const auto ie_index = ie_memberships[dps.i]; ie_index > 0)
                    new_dps.ie_membership[ie_index - 1u] += k;
                new_dps.cs += k;
                V f = utils::math::binomial<V>(a[dps.i], k);
                if (dps.i==3 && k > 0){
                    f *= 6;
                }
                if (dps.i==2){
                    f *= inclusion_exlusion(new_dps.ie_membership);
                }
                subtotal += f * self(new_dps);
            }
            cache[dps] = subtotal;
//            std::println("state = (i={}, cs={}, ie[0]={}, ie[1]={}, ie[2]={}), value = {}",dps.i, dps.cs, dps.ie_membership[0], dps.ie_membership[1], dps.ie_membership[2], subtotal);
            return subtotal;
        };
        return impl(DPState{});
    }

}