#pragma once

#include "typedefs.hpp"
#include "card_amount_tables.hpp"

namespace config {
    struct GeneratorBounds {
        kingdom::amounts_t kingdom_;
        landscapes::supply::amounts_t landscapes_supply_;
        landscapes::other::amounts_t landscapes_other_;
    };

    GeneratorBounds from_expansions_editions(const kingdom::ExpansionEditionFilter& l) noexcept;

    coefficient_t filter_kingdom_table(const kingdom::table_t& table, const kingdom::ExpansionEditionFilter& expansion_edition, const kingdom::TableQuery& query) noexcept;

    coefficient_t filter_landscapes_supply_table(const landscapes::supply::table_t& table, const landscapes::ExpansionFilter& expansion, const landscapes::supply::TableQuery& query) noexcept;

    coefficient_t filter_landscapes_other_table(const landscapes::other::table_t& table, const landscapes::ExpansionFilter& expansion, const landscapes::other::TableQuery& query) noexcept;
}
