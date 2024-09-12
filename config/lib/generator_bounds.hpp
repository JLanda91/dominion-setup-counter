#pragma once

#include "typedefs.hpp"
#include "card_amount_tables.hpp"

namespace config {
    struct GeneratorBounds {
        kingdom::amounts_t kingdom_;
        landscapes::amounts_t landscapes_;
    };

    GeneratorBounds from_expansions_editions(const kingdom::ExpansionEditionFilter& l) noexcept;

    coefficient_t filter_kingdom_table(const kingdom::table_t& table, const kingdom::ExpansionEditionFilter& expansion_edition, const kingdom::TableQuery& query) noexcept;

    coefficient_t filter_landscapes_table(const landscapes::supply_table_t& table, const landscapes::ExpansionFilter& expansion, const landscapes::TableQuery& query) noexcept;
}
