#pragma once

#include <concepts>
#include <algorithm>
#include <cstdint>
#include <ranges>

namespace math_utils {
    template<typename U, typename T>
    consteval U factorial(T n){
        auto result = (U)1;
        for(U j = 2; j <= n; ++j){
            result *= j;
        }
        return result;
    }

    template<typename U, typename T>
    constexpr U binomial(T n, T k){
        if(k > n){
            return (U)0;
        }
        auto result = (U)1;
        for(U j = 0; j < std::min(k, n-k); ++j){
            result *= (U)(n - j);
            result /= (j+1);
        }
        return result;
    }

    template<typename U, tuple_utils::GenericRepeatedTuple T>
    constexpr U binomial_product(const T& n_collection, const T& k_collection){
        auto impl = [&]<size_t ... I>(std::index_sequence<I...>) constexpr -> U {
            return (... * (binomial<U>(std::get<I>(n_collection), std::get<I>(k_collection))));
        };
        return impl(std::make_index_sequence<std::tuple_size<T>::value>{});
    }
}