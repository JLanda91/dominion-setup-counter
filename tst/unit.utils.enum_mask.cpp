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

    TEST(EnumMask_1d, default_all){
        enum_mask_1d_t am{};
        const auto actual = am.bool_view() | std::ranges::to<std::vector>();
        ASSERT_THAT(actual, Each(true));
    }

    TEST(EnumMask_1d, list_init){
        enum_mask_1d_t am{Row::FIRST, Row::THIRD};
        const auto actual = am.bool_view() | std::ranges::to<std::vector>();
        const std::vector<bool> expected{true, false, true};
        ASSERT_THAT(actual, ContainerEq(expected));
    }

    TEST(EnumMask_1d, static_test){
        constexpr enum_mask_1d_t t{Row::FIRST, Row::THIRD};
        static_assert(t.test(Row::FIRST));
        static_assert(t.test(Row::THIRD));
        static_assert(t.count() == 2uz);
    }

    using enum_mask_2d_t = EnumMask<SizedRow, SizedColumn>;
    static_assert(enum_mask_2d_t::size == 15uz);

    TEST(EnumMask_2d, default_all){
        enum_mask_2d_t am{};
        const auto actual = am.bool_view() | std::ranges::to<std::vector>();
        ASSERT_THAT(actual, Each(true));
    }

    TEST(EnumMask_2d, list_init){
        enum_mask_2d_t am{{Row::FIRST, Column::SECOND}, {Row::FIRST, Column::THIRD}, {Row::SECOND, Column::FIRST}, {Row::SECOND, Column::FIFTH}};
        const auto actual = am.bool_view() | std::ranges::to<std::vector>();
        const std::vector<bool> expected{
                false, true, true, false, false,
                true, false, false, false, true,
                false, false, false, false, false,
        };;
        ASSERT_THAT(actual, ContainerEq(expected));
    }

    TEST(EnumMask_2d, static_test){
        constexpr enum_mask_2d_t t{{Row::FIRST, Column::SECOND}, {Row::FIRST, Column::THIRD}, {Row::SECOND, Column::FIRST}, {Row::SECOND, Column::FOURTH}, };
        static_assert(t.test({Row::FIRST, Column::SECOND}));
        static_assert(t.test({Row::FIRST, Column::THIRD}));
        static_assert(t.test({Row::SECOND, Column::FIRST}));
        static_assert(t.test({Row::SECOND, Column::FOURTH}));
        static_assert(t.count() == 4uz);
    }

}