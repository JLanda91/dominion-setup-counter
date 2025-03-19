#pragma once

#include <concepts>
#include <algorithm>
#include <cstdint>
#include <ranges>
#include <unordered_map>

namespace utils::math {
    template<typename U, typename T>
    consteval U factorial(T n){
        auto result = (U)1;
        for(auto j = (U)2u; j <= n; ++j){
            result *= j;
        }
        return result;
    }

    template<typename U, typename T>
    constexpr U binomial(T n, T k){
        if(k > n){
            return (U)0;
        }
        auto result = (U)1;
        for(auto j = (U)0u; j < std::min((U)k, (U)(n-k)); ++j){
            result *= (U)(n - j);
            result /= (j+1);
        }
        return result;
    }

    template<typename U, typename T>
    constexpr U binomial_product(const T& n_collection, const T& k_collection){
        auto impl = [&]<size_t ... I>(std::index_sequence<I...>) constexpr -> U {
            return (... * (binomial<U>(std::get<I>(n_collection), std::get<I>(k_collection))));
        };
        return impl(std::make_index_sequence<std::tuple_size<T>::value>{});
    }

    template<typename T>
    concept input_byte_range = std::ranges::input_range<T> && std::same_as<std::ranges::range_value_t<T>, uint8_t>;

    class BinomialStore64 {
        using value_t = std::array<uint64_t, 11>;
        using data_t = std::unordered_map<uint8_t, value_t>;

    public:
        BinomialStore64() = default;
        explicit BinomialStore64(const input_byte_range auto& n_collection){
            for(const auto n : n_collection){
                if (data_.contains(n)){
                    continue;
                }
                value_t values{};
                for(auto k = (uint8_t)0; k <= 10; ++k){
                    values[k] = binomial<uint64_t>(n, k);
                }
                data_[n] = std::move(values);
            }
        }

        auto operator[](uint8_t n, uint8_t k) const -> const uint64_t& {
            return data_.at(n)[k];
        }

    private:
        data_t data_{};
    };

}