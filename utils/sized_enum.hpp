#pragma once

#include <cstdint>

namespace utils::enums {

    template<typename E, std::size_t N>
    struct SizedEnum {
        using type = E;
        static constexpr std::size_t size = N;
    };

}