#pragma once

#include <fmt/core.h>

#include <sstream>

template<typename T>
concept FmtFormattable = requires(T t, fmt::format_context& ctx) {
    { fmt::formatter<T>{}.format(t, ctx) };
};
