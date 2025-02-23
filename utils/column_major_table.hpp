#pragma once

#include <fmt/format.h>

#include <cstdint>
#include <array>
#include <ranges>
#include <utility>
#include <sstream>

#include "format.hpp"


namespace utils {
    template<FmtFormattable RowLabel, FmtFormattable ColumnLabel, std::size_t NumRows, std::size_t NumColumns>
    class ColumnMajorTable {
    public:
        using row_label_t = std::array<RowLabel, NumRows>;
        using column_label_t = std::array<ColumnLabel, NumColumns>;
        using data_t = std::array<uint8_t, NumRows * NumColumns>;

        explicit constexpr ColumnMajorTable(const row_label_t& row_labels, const column_label_t& col_labels, const data_t& data) : row_labels_{row_labels}, col_labels_{col_labels}, data_{data} {}

        constexpr auto row_labels() const noexcept -> const row_label_t& {
            return row_labels_;
        }

        constexpr auto column_labels() const noexcept -> const column_label_t& {
            return col_labels_;
        }

        constexpr auto column(std::size_t col) const {
            if (col >= NumColumns) {
                throw std::out_of_range(fmt::format("ColumnMajorTable::column({}) requested", col));
            }
            const auto begin = data_.cbegin() + NumRows * col;
            const auto end = begin + NumRows;
            return std::ranges::subrange(begin, end, NumRows);
        }

        constexpr auto columns() const noexcept {
            return std::views::iota(0uz, NumColumns) | std::views::transform([this](auto i){ return column(i);});
        }

        constexpr auto row(std::size_t row) const {
            if (row >= NumRows) {
                throw std::out_of_range(fmt::format("ColumnMajorTable::row({}) requested", row));
            }
            return data_ | std::views::drop(row) | std::views::stride(NumRows);
        }

        constexpr auto rows() const noexcept {
            return std::views::iota(0uz, NumRows) | std::views::transform([this](auto i){ return row(i);});
        }

    private:
        row_label_t row_labels_;
        column_label_t col_labels_;
        data_t data_;
    };
}

template<FmtFormattable RowLabel, FmtFormattable ColumnLabel, std::size_t NumRows, std::size_t NumColumns>
struct fmt::formatter<utils::ColumnMajorTable<RowLabel, ColumnLabel, NumRows, NumColumns>> : formatter<std::string> {
    constexpr auto format(const utils::ColumnMajorTable<RowLabel, ColumnLabel, NumRows, NumColumns>& obj, fmt::format_context& ctx) const {

        const auto row_names = obj.row_labels() | std::views::transform([](const auto& x){return fmt::format("{}",x);}) | std::views::as_const;
        const auto column_names = obj.column_labels() | std::views::transform([](const auto& x){return fmt::format("{}",x);}) | std::views::as_const;

        const auto row_name_widths = row_names | std::views::transform([](std::string_view sv){return std::size(sv);}) | std::views::as_const;
        const auto max_row_name_length = *std::ranges::max_element(row_name_widths);

        const auto column_widths = std::views::iota(0uz, NumColumns) | std::views::transform([&obj, &column_names](std::size_t j){
            const auto cell_lengths = obj.column(j) | std::views::transform([](uint8_t x){return std::size(std::to_string(x));});
            const auto max_cell_length = *std::ranges::max_element(cell_lengths);
            return std::max(max_cell_length, std::size(column_names[j]));
        }) | std::ranges::to<std::vector>();

        std::ostringstream oss{};
        oss << fmt::vformat("{:{}}", fmt::make_format_args("", max_row_name_length));
        for(auto j = 0uz; j < NumColumns; ++j){
            oss << fmt::vformat(" {:>{}}", fmt::make_format_args(column_names[j], column_widths.at(j)));
        }
        oss << '\n';
        for(auto i = 0uz; i < NumRows; ++i){
            const auto& row_data = obj.row(i);
            oss << fmt::vformat("{:{}}", fmt::make_format_args(row_names[i], max_row_name_length));
            for(auto j = 0uz; j < NumColumns; ++j){
                oss << fmt::vformat(" {:>{}}", fmt::make_format_args(row_data[j], column_widths.at(j)));
            }
            oss << '\n';
        }
        oss << '\n';
        return formatter<std::string>::format(oss.str(), ctx);
    }
};
