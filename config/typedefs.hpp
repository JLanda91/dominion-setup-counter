#pragma once

#include <boost/multiprecision/integer.hpp>

#include "tuple.hpp"
#include "card_amount_tables.hpp"

namespace config {
    using result_t = boost::multiprecision::uint512_t;
    using result_pair_t = std::pair<result_t, result_t>;
    using coefficient_t = uint32_t;

    template<size_t N>
    using coefficient_tuple_t = typename utils::tuple::repeated_tuple<coefficient_t, N>::type;

    namespace kingdom {
        using query_amounts_t = coefficient_tuple_t<std::tuple_size_v<kingdom::queries_t>>;
        using special_amounts_t = coefficient_tuple_t<std::tuple_size_v<kingdom::special_tests_t>>;
        using amounts_t = coefficient_tuple_t<std::tuple_size_v<kingdom::queries_t> + std::tuple_size_v<kingdom::special_tests_t>>;
    }

    namespace landscapes {
        using query_amounts_t = coefficient_tuple_t<std::tuple_size_v<landscapes::queries_t>>;
        using special_amounts_t = coefficient_tuple_t<std::tuple_size_v<landscapes::special_tests_t>>;
        using amounts_t = coefficient_tuple_t<std::tuple_size_v<landscapes::queries_t> + std::tuple_size_v<landscapes::special_tests_t>>;
    }
}