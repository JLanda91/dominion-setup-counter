#pragma once

#include <format>
#include <sstream>

template<typename ResultType>
struct std::formatter<ResultType> : std::formatter<std::string> {
    constexpr auto parse(std::format_parse_context& ctx) const {
        return ctx.begin();
    }

    auto format(const ResultType& obj, std::format_context& ctx) const {
        std::ostringstream oss{};
        oss << obj;
        return std::format_to(ctx.out(), "{}", oss.str());
    }
};
