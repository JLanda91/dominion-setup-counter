#pragma once

#include <array>

namespace data {
    template<typename T, std::size_t N>
    struct array {
        static constexpr std::size_t size = N;
        using value_type = T;

        using host_type = std::array<T, N>;
        using device_type = T[N];
    };
}