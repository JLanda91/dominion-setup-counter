#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fmt/format.h>

#include <utils/column_major_table.hpp>

using table_t = utils::ColumnMajorTable<int, int, 5, 3>;

static constexpr table_t::row_label_t kRowLabels{123,24,-83200,0,9};
static constexpr table_t::column_label_t kColumnLabels{3211,0,-186913};
static constexpr table_t::data_t kData{1,2,3,4,5,6,254,8,9,10,11,12,13,14,15};

static constexpr table_t kTable{
    kRowLabels,
    kColumnLabels,
    kData
};


namespace {
    TEST(column_major_table, column_iteration){
        auto data_it = kData.cbegin();
        for (const auto& column : kTable.columns()) {
            for (const auto& v : column) {
                ASSERT_THAT(v, testing::Eq(*data_it));
                ++data_it;
            }
        }
        ASSERT_THAT(data_it, testing::Eq(kData.cend()));
    }

    TEST(column_major_table, row_iteration){
        auto i = 0u;
        auto num_values = 0u;
        for (const auto& row : kTable.rows()) {
            for (const auto& v : row) {
                ASSERT_THAT(v, testing::Eq(kData[i]));
                i += 5;
                if(i >= kData.size()){
                    i %= (5 * 3);
                    ++i;
                }
                num_values++;
            }
        }
        ASSERT_THAT(num_values, testing::Eq(5*3));
    }

    TEST(column_major_table, print) {
        fmt::print("{}", kTable);
    }
}