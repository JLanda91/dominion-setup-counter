#pragma once

#include <boost/multiprecision/integer.hpp>
#include <fmt/format.h>

using result_t = boost::multiprecision::uint256_t;
using namespace boost::multiprecision::literals;

template<>
struct fmt::formatter<result_t> : formatter<std::string> {
    auto format(const result_t& obj, fmt::format_context& ctx) const {
        return formatter<std::string>::format(obj.str(), ctx);
    }
};
