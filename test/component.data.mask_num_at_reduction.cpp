#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fmt/format.h>

#include <data/mask_num_at_reduction.hpp>

namespace {
    TEST(mask_num_at_reduction, print_data) {
        const std::size_t start = std::size(data::kECSegmentDistributions) - 100;
        const std::size_t end = std::size(data::kECSegmentDistributions);

        for (std::size_t i = start; i < end; ++i) {
            fmt::print("{:5}: data = (", i);
            for (const auto& data_elem : data::kECSegmentDistributions[i].data) {
                fmt::print("{:03} ", data_elem);
            }
            fmt::println("), continuation no = {:2}", data::kECSegmentDistributions[i].continuation_no);
        }
    }
}