#include <magic_enum.hpp>

#include "generator_bounds.hpp"

namespace config {
    template<std::size_t N, typename EnumMask>
    static void populate_special(coefficient_tuple_t<N>& tup, const EnumMask& mask, const std::array<typename EnumMask::enum_t, N>& tests){
        auto impl = [&mask, &tests]<std::size_t ... I>(std::index_sequence<I...>, coefficient_tuple_t<N>& result) noexcept {
            (..., (std::get<I>(result) = (coefficient_t)(mask.test(tests[I]))));
        };
        return impl(std::make_index_sequence<N>{}, tup);
    }

    coefficient_t filter_kingdom_table(const kingdom::table_t& table, const kingdom::ExpansionEditionFilter& expansion_edition, const kingdom::TableQuery& query) noexcept {
        return std::ranges::fold_left(table |
                                      std::views::filter([&expansion_edition, &query](const kingdom::table_row_t& row) { return
                                              expansion_edition.test(row.expansion_edition_) &&
                                              query.tracked_types_mask_ == row.tracked_types_mask_ &&
                                              query.cost_group_ == row.cost_group_;}) |
                                      std::views::transform([](const kingdom::table_row_t& row) { return row.amount_; }),
                                      coefficient_t{}, std::plus{});
    }

    coefficient_t filter_landscapes_supply_table(const landscapes::supply::table_t& table, const landscapes::ExpansionFilter& expansion, const landscapes::supply::TableQuery& query) noexcept {
        return std::ranges::fold_left(table |
                                      std::views::filter([&expansion, &query](const landscapes::supply::table_row_t& row){ return
                                              expansion.test(row.expansion_) &&
                                              query.type_.test(row.type_);}) |
                                      std::views::transform([](const landscapes::supply::table_row_t& row){return row.amount_;}),
                                      coefficient_t{}, std::plus{});
    }

    coefficient_t filter_landscapes_other_table(const landscapes::other::table_t& table, const landscapes::ExpansionFilter& expansion, const landscapes::other::TableQuery& query) noexcept {
        return std::ranges::fold_left(table |
                                      std::views::filter([&expansion, &query](const landscapes::other::table_row_t& row){ return
                                              expansion.test(row.expansion_) &&
                                              query.type_.test(row.type_);}) |
                                      std::views::transform([](const landscapes::other::table_row_t& row){return row.amount_;}),
                                      coefficient_t{}, std::plus{});
    }

    kingdom::query_amounts_t kingdom_regular_bounds(const kingdom::ExpansionEditionFilter& l) noexcept {
        auto impl = [&l]<std::size_t ... I>(std::index_sequence<I...>) noexcept  {
            kingdom::query_amounts_t result{};
            const auto& amount_queries = kingdom::amount_queries();
            (..., (std::get<I>(result) = filter_kingdom_table(kingdom::table(), l, std::get<I>(amount_queries))));
            return result;
        };
        return impl(std::make_index_sequence<std::tuple_size_v<kingdom::amount_queries_t>>{});
    }

    kingdom::special_amounts_t kingdom_special_bounds(const kingdom::ExpansionEditionFilter& l) noexcept {
        kingdom::special_amounts_t result{};
        populate_special(result, l, kingdom::special_predicates());
        return result;
    }

    landscapes::ExpansionFilter expansion_only(const kingdom::ExpansionEditionFilter& l){
        auto result = landscapes::ExpansionFilter{}.reset();
        for(auto expansion : magic_enum::enum_values<Expansion>()){
            if(l.test({expansion, kingdom::EditionModifier::NONE})){
                result.set(expansion);
            }
        }
        return result;
    }

    landscapes::supply::query_amounts_t landscape_regular_bounds(const landscapes::ExpansionFilter& l) noexcept {
        auto impl = [&l]<std::size_t ... I>(std::index_sequence<I...>) noexcept {
            landscapes::supply::query_amounts_t result{};
            const auto& amount_queries = landscapes::supply::amount_queries();
            (..., (std::get<I>(result) = filter_landscapes_supply_table(landscapes::supply::table(), l,
                                                                        std::get<I>(amount_queries))));
            return result;
        };
        return impl(std::make_index_sequence<std::tuple_size_v<landscapes::supply::amount_queries_t>>{});
    }

    landscapes::supply::special_amounts_t landscapes_special_bounds(const landscapes::ExpansionFilter& l) noexcept {
        landscapes::supply::special_amounts_t result{};
        populate_special(result, l, landscapes::supply::special_predicates());
        return result;
    }

    landscapes::other::amounts_t landscapes_other_bounds(const landscapes::ExpansionFilter& l) noexcept {
        auto impl = [&l]<std::size_t ... I>(std::index_sequence<I...>) noexcept {
            landscapes::other::amounts_t result;
            (..., (std::get<I>(result) = filter_landscapes_other_table(landscapes::other::table(), l, landscapes::other::TableQuery{{(landscapes::other::Type)I}})));
            return result;
        };
        return impl(std::make_index_sequence<std::tuple_size_v<landscapes::other::amounts_t>>{});
    }

    GeneratorBounds from_expansions_editions(const kingdom::ExpansionEditionFilter& l) noexcept {
        const auto el = expansion_only(l);

        return GeneratorBounds{
            .kingdom_ = std::tuple_cat(kingdom_regular_bounds(l), kingdom_special_bounds(l)),
            .landscapes_supply_ = std::tuple_cat(landscape_regular_bounds(el), landscapes_special_bounds(el)),
            .landscapes_other_ = landscapes_other_bounds(el)
        };
    }
}