#pragma once

#include <concepts>
#include <algorithm>
#include <cstdint>
#include <ranges>

namespace utils::math {
    template<typename U, typename T>
    consteval U factorial(T n){
        auto result = (U)1;
        for(auto j = (U)2u; j <= n; ++j){
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
        for(auto j = (U)0u; j < std::min((U)k, (U)(n-k)); ++j){
            result *= (U)(n - j);
            result /= (j+1);
        }
        return result;
    }

    template<typename U, typename T>
    constexpr U binomial_product(const T& n_collection, const T& k_collection){
        auto impl = [&]<size_t ... I>(std::index_sequence<I...>) constexpr -> U {
            return (... * (binomial<U>(std::get<I>(n_collection), std::get<I>(k_collection))));
        };
        return impl(std::make_index_sequence<std::tuple_size<T>::value>{});
    }

}