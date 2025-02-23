// GENERATED CODE - DO NOT MODIFY

#pragma once

#include <array>
#include <ranges>
#include <utility>

#include "membership_mask.hpp"

namespace card_data::kingdom {
    enum class CardType : uint8_t ;

    /*[[[cog
    from generator.kingdom.card_type_major_table import instance
    instance.generate_card_type_enum_class()
    cog.outl()
    instance.generate_size_constants()
    ]]]*/
    //[[[end]]]

    template<std::size_t NumVennRegions, std::size_t NumCardTypes>
    class CardTypeMajorTable {
    public:
        using data_t = std::array<uint8_t, NumVennRegions * NumCardTypes>;
        using row_label_t = std::array<MembershipMask, NumVennRegions>;

        static constexpr auto kNumVennRegions = NumVennRegions;
        static constexpr auto kNumCardTypes = NumCardTypes;

        explicit constexpr CardTypeMajorTable(const row_label_t& row_labels, const data_t& data) : row_labels_{row_labels}, data_{data} {}

        constexpr auto row_labels() const noexcept -> const row_label_t& {
            return row_labels_;
        }

        constexpr auto card_type_column(CardType cardType) const noexcept {
            const auto begin = data_.cbegin() + NumVennRegions * std::to_underlying(cardType);
            const auto end = begin + NumVennRegions;
            return std::ranges::subrange(begin, end, NumVennRegions);
        }

        constexpr auto card_type_columns() const noexcept {
            return std::views::iota(0uz, NumCardTypes) | std::views::transform([this](auto i){ return card_type_column(static_cast<CardType>(i));});
        }

        constexpr auto venn_region_row(std::size_t row) const noexcept {
            return data_ | std::views::drop(row) | std::views::stride(NumVennRegions);
        }

        constexpr auto venn_region_rows() const noexcept {
            return std::views::iota(0uz, NumVennRegions) | std::views::transform([this](auto i){ return venn_region_row(i);});
        }

    private:
        row_label_t row_labels_;
        data_t data_;
    };

    /*[[[cog
    instance.generate_sized_alias()
    cog.outl()
    instance.generate_singleton()
    ]]]*/
    //[[[end]]]
}
