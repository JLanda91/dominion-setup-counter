#pragma once

#include <tuple>
#include <iostream>
#include <format>

namespace utils::tuple {
    template<size_t I, typename T>
    constexpr auto reverse_partial_sum(const T& t){
        static_assert(I <= std::tuple_size_v<T>);
        if constexpr(I == 0uz){
            return 0u;
        } else {
            return std::get<std::tuple_size_v<T> - I>(t) + reverse_partial_sum<I - 1>(t);
        }
    }

    template<size_t I, typename ... Args>
    constexpr auto reverse_partial_sum(const std::tuple<Args...>& t){
        static_assert(I <= sizeof...(Args));
        if constexpr(I == 0uz){
            return 0u;
        } else {
            return std::get<sizeof...(Args) - I>(t) + reverse_partial_sum<I - 1>(t);
        }
    }

    template<std::unsigned_integral U, std::size_t N>
    struct repeated_tuple {
        using type = decltype(std::tuple_cat(typename repeated_tuple<U, N/2>::type{}, typename repeated_tuple<U, N/2 + (N&1)>::type{}));
        using element_type = U;
    };

    template<std::unsigned_integral U>
    struct repeated_tuple<U, 1uz> {
        using type = std::tuple<U>;
        using element_type = U;
    };

    template<typename T, typename U>
    concept RepeatedTuple = std::tuple_size_v<T> > 0 && std::is_same_v<T, typename repeated_tuple<U, std::tuple_size_v<T>>::type >;

    template<typename T>
    concept GenericRepeatedTuple = RepeatedTuple<T, typename std::tuple_element<0, T>::type>;
}
