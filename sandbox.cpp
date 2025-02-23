#include <fmt/core.h>
#include <fmt/ranges.h>
#include <omp.h>

#include <vector>
#include <ranges>
#include <cstdint>
#include <chrono>

#include <card_data/kingdom/card_type_major_table.hpp>


static_assert(card_data::kingdom::card_type_major_table().row_labels().size() == 20);

int main() {
    fmt::println("{}\n", card_data::kingdom::card_type_major_table(), "\n");


    return 0;
}