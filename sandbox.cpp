#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/chrono.h>
#include <omp.h>

#include <vector>
#include <ranges>
#include <cstdint>
#include <chrono>

#include <card_data/kingdom/card_type_major_table.hpp>


static_assert(card_data::kingdom::card_type_major_table().row_labels().size() == 20);

int main() {
    std::chrono::steady_clock::duration d{};

    d += std::chrono::hours(2u);
    d += std::chrono::minutes(3u);
    d += std::chrono::seconds(137u);

    fmt::println("Solution found in {:%H hours %M minutes %S seconds}", std::chrono::duration_cast<std::chrono::seconds>(d));
}