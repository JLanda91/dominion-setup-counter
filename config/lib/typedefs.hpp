#pragma once

#include "../../cmake-build-debug-gcc-14/vcpkg_installed/x64-linux/include/boost/multiprecision/integer.hpp"

#include "tuple.hpp"
#include "card_amount_tables.hpp"

namespace config {
    using result_t = boost::multiprecision::uint512_t;
    using result_pair_t = std::pair<result_t, result_t>;
    using coefficient_t = uint32_t;

    template<size_t N>
    using coefficient_tuple_t = typename utils::tuple::repeated_tuple<coefficient_t, N>::type;

    namespace kingdom {
        using query_amounts_t = coefficient_tuple_t<std::tuple_size_v<kingdom::amount_queries_t>>;
        using special_amounts_t = coefficient_tuple_t<std::tuple_size_v<kingdom::special_predicates_t >>;
        using amounts_t = coefficient_tuple_t<std::tuple_size_v<kingdom::amount_queries_t> + std::tuple_size_v<kingdom::special_predicates_t>>;
    }

    namespace landscapes {
        using query_amounts_t = coefficient_tuple_t<std::tuple_size_v<landscapes::supply_amount_queries_t>>;
        using special_amounts_t = coefficient_tuple_t<std::tuple_size_v<landscapes::supply_special_predicates_t>>;
        using amounts_t = coefficient_tuple_t<std::tuple_size_v<landscapes::supply_amount_queries_t> + std::tuple_size_v<landscapes::supply_special_predicates_t>>;
    }
}