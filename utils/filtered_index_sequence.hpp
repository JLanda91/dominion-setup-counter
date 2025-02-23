#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

namespace utils {

    template<std::size_t N, template<std::size_t> class P>
    class filtered_index_sequence {
        template<std::size_t I, std::size_t... Seq>
        static constexpr auto generate_impl(std::index_sequence<Seq...>) {
            if constexpr (I == N) {
                return std::index_sequence<Seq...>{};
            } else if constexpr (P<I>::value) {
                return generate_impl<I + 1>(std::index_sequence<Seq..., I>{});
            } else {
                return generate_impl<I + 1>(std::index_sequence<Seq...>{});
            }
        }

    public:
        using type = decltype(generate_impl<0u>(std::index_sequence<>{}));
    };

    template<std::size_t N, template<std::size_t> class P>
    using filtered_index_sequence_t = typename filtered_index_sequence<N, P>::type;

}