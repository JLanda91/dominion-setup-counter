#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <bitset>

#include "sized_enum.hpp"
#include "enum_mask.hpp"

namespace {
    using testing::TestWithParam;
    using testing::Each;
    using testing::ContainerEq;

    using utils::table::EnumMask;
    using utils::enums::SizedEnum;

    enum class Row : unsigned {
        FIRST,
        SECOND,
        THIRD,
    };

    using SizedRow = SizedEnum<Row, 3uz>;

    enum class Column : unsigned {
        FIRST,
        SECOND,
        THIRD,
        FOURTH,
        FIFTH,
    };

    using SizedColumn = SizedEnum<Column, 5uz>;

    using enum_mask_1d_t = EnumMask<SizedRow>;
    static_assert(enum_mask_1d_t::size == 3uz);

    TEST(EnumMask_1d, default_none){
        auto enum_mask = enum_mask_1d_t{};
        const auto actual = enum_mask.bool_view() | std::ranges::to<std::vector>();
        ASSERT_THAT(actual, Each(false));
    }

    TEST(EnumMask_1d, all){
        auto enum_mask = enum_mask_1d_t{}.set();
        const auto actual = enum_mask.bool_view() | std::ranges::to<std::vector>();
        ASSERT_THAT(actual, Each(true));
    }

    TEST(EnumMask_1d, list_init){
        auto enum_mask = enum_mask_1d_t{Row::FIRST, Row::THIRD};
        const auto actual = enum_mask.bool_view() | std::ranges::to<std::vector>();
        const std::vector<bool> expected{true, false, true};
        ASSERT_THAT(actual, ContainerEq(expected));
    }

    TEST(EnumMask_1d, static_test){
        constexpr auto enum_mask = enum_mask_1d_t{Row::FIRST, Row::THIRD};
        static_assert(enum_mask.test(Row::FIRST));
        static_assert(enum_mask.test(Row::THIRD));
        static_assert(enum_mask.count() == 2uz);
    }

    using enum_mask_2d_t = EnumMask<SizedRow, SizedColumn>;
    static_assert(enum_mask_2d_t::size == 15uz);

    TEST(EnumMask_2d, default_none){
        auto enum_mask = enum_mask_2d_t{};
        const auto actual = enum_mask.bool_view() | std::ranges::to<std::vector>();
        ASSERT_THAT(actual, Each(false));
    }

    TEST(EnumMask_2d, all){
        auto enum_mask = enum_mask_2d_t{}.set();
        const auto actual = enum_mask.bool_view() | std::ranges::to<std::vector>();
        ASSERT_THAT(actual, Each(true));
    }

    TEST(EnumMask_2d, list_init){
        auto enum_mask = enum_mask_2d_t{{Row::FIRST, Column::SECOND}, {Row::FIRST, Column::THIRD}, {Row::SECOND, Column::FIRST}, {Row::SECOND, Column::FIFTH}};
        const auto actual = enum_mask.bool_view() | std::ranges::to<std::vector>();
        const std::vector<bool> expected{
                false, true, true, false, false,
                true, false, false, false, true,
                false, false, false, false, false,
        };;
        ASSERT_THAT(actual, ContainerEq(expected));
    }

    TEST(EnumMask_2d, static_test){
        constexpr auto enum_mask = enum_mask_2d_t{{Row::FIRST, Column::SECOND}, {Row::FIRST, Column::THIRD}, {Row::SECOND, Column::FIRST}, {Row::SECOND, Column::FOURTH}, };
        static_assert(enum_mask.test({Row::FIRST, Column::SECOND}));
        static_assert(enum_mask.test({Row::FIRST, Column::THIRD}));
        static_assert(enum_mask.test({Row::SECOND, Column::FIRST}));
        static_assert(enum_mask.test({Row::SECOND, Column::FOURTH}));
        static_assert(enum_mask.count() == 4uz);
    }

}