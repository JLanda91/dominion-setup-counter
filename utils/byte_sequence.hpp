#pragma once

#include <cstdint>
#include <utility>

namespace utils {
    template<uint8_t ... I>
    using byte_sequence = std::integer_sequence<uint8_t, I...>;

    template<uint8_t ... I>
    using incremented_byte_sequence = byte_sequence<(I + 1)...>;

    template<uint8_t N>
    struct make_byte_sequence {
    private:
        template<uint8_t ... I>
        static constexpr auto generate_impl(std::integer_sequence<uint8_t, I...>) {
            return incremented_byte_sequence<I...>{};
        }
    public:
        using type = decltype(generate_impl(std::make_integer_sequence<uint8_t, N>{}));
    };

    template<uint8_t N>
    using make_incremented_byte_sequence_t = typename make_byte_sequence<N>::type;

    static_assert(std::is_same_v<make_incremented_byte_sequence_t<3>, byte_sequence<1, 2, 3>>);
}