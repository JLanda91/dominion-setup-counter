#pragma once

#include <boost/multiprecision/integer.hpp>
#include <fmt/format.h>

#ifdef OVERFLOW_CHECKED
using result_t = boost::multiprecision::checked_uint256_t;
#else
using result_t = boost::multiprecision::uint256_t;
#endif

using namespace boost::multiprecision::literals;

template<>
struct fmt::formatter<result_t> : formatter<std::string> {
    auto format(const result_t& obj, format_context& ctx) const {
        return formatter<std::string>::format(obj.str(), ctx);
    }
};
