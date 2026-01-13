#pragma once

#include <fmt/format.h>

#include <string>
#include <algorithm>

#include "big_uint.cuh"

template<std::size_t N>
struct fmt::formatter<data_definitions::big_uint<N>> : formatter<std::string> {
private:
    static uint64_t divide_by_ten(uint64_t* limbs) {
        __uint128_t remainder = 0;
        for (ssize_t i = N-1; i >= 0; --i) {
            const __uint128_t current = (remainder << 64) | limbs[i];
            limbs[i] = static_cast<uint64_t>(current / 10);
            remainder = current % 10;
        }
        return static_cast<uint64_t>(remainder);
    }

    static bool is_zero(const uint64_t* limbs) {
        return std::all_of(limbs, limbs + N, [](uint64_t limb) { return limb == 0; });
    }

    static std::string big_number_to_decimal(const uint64_t* limbs) {
        if (is_zero(limbs)) {
            return "0";
        }

        uint64_t tmp[N];
        std::copy_n(limbs, N, tmp);
        std::string result;
        result.reserve(21 * N);

        do {
            const uint64_t digit = divide_by_ten(tmp);
            result.push_back('0' + digit);
        } while (!is_zero(tmp));

        std::reverse(result.begin(), result.end());
        return result;
    }

public:
    auto format(const data_definitions::big_uint<N>& obj, format_context& ctx) const {
        return formatter<std::string>::format(big_number_to_decimal(obj.limbs), ctx);
    }
};