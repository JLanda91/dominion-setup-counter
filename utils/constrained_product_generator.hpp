#pragma once

#include <generator>

#include "tuple.hpp"

namespace utils::generators {

    enum class Constraint : uint8_t {
        EQ,
        LE,
    };

    template<Constraint C, utils::tuple::GenericRepeatedTuple Tuple, size_t I, typename U = std::tuple_element<0, Tuple>::type>
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

    template<Constraint C, utils::tuple::GenericRepeatedTuple Tuple, typename U = std::tuple_element<0, Tuple>::type>
    std::generator<Tuple> constrained_product(const Tuple& boundary, U s) {
        Tuple arr{};
        for (const auto& elem : constrained_product_impl<C, Tuple, 0uz, U>(boundary, s, arr, 0,
                                                                           utils::tuple::reverse_partial_sum<
                                                                                   std::tuple_size_v<Tuple> - 1>(
                                                                                   boundary))) {
            co_yield elem;
        }
    }
}