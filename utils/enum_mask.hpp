#pragma once

#include <magic_enum.hpp>

#include <array>
#include <ranges>
#include <utility>
#include <cstdint>
#include <bitset>
#include <algorithm>
#include <concepts>

namespace utils::table{
    template<typename ... SizedEnum> requires (... && (std::is_scoped_enum_v<typename SizedEnum::type>))
    class EnumMask {
    public:
        static constexpr std::size_t size = (... * SizedEnum::size);
        static constexpr std::size_t num_enums = sizeof...(SizedEnum);
        using mask_t = std::bitset<size>;
        using enum_t = std::tuple<typename SizedEnum::type ...>;

        constexpr EnumMask() noexcept : mask_(mask_t{}.set()) {}

        constexpr EnumMask(std::initializer_list<enum_t> l) noexcept {
            for (const auto& x : l) {
                set(x);
            }
        }

        constexpr EnumMask& none() noexcept {
            mask_.reset();
            return *this;
        }

        [[nodiscard]] constexpr auto count() const noexcept {
            return mask_.count();
        }

        constexpr EnumMask& set(const enum_t& e) noexcept {
            mask_.set(EnumMask::index(e));
            return *this;
        }

        [[nodiscard]] constexpr auto test(const enum_t& e) const noexcept {
            return mask_.test(index(e));
        }

        constexpr auto bool_view() const noexcept {
            return std::views::iota(0uz, size) | std::views::transform([this](const auto i){return mask_.test(i);});
        }

    private:
        constexpr EnumMask(const mask_t& mask) noexcept : mask_(mask) {}

        static constexpr std::array<std::size_t, sizeof...(SizedEnum)> sizes{SizedEnum::size ...};

        static constexpr auto block_sizes = [](){
            std::array<std::size_t, sizeof...(SizedEnum) + 1uz> result{};
            result[sizeof...(SizedEnum)] = 1uz;
            for(const auto i : std::views::iota(0uz, sizeof...(SizedEnum)) | std::views::reverse){
                result[i] = result[i+1uz] * sizes[i];
            }
            return result;
        }();

        static constexpr std::size_t index(const enum_t& e) noexcept {
            constexpr auto impl = []<std::size_t ... I>(std::index_sequence<I...>, const enum_t& e) -> std::size_t {
                return (... + (std::to_underlying(std::get<I>(e)) * block_sizes[I+1uz]));
            };
            return impl(std::make_index_sequence<num_enums>(), e);
        }

        mask_t mask_;
    };

}