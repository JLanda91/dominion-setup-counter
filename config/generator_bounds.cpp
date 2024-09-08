#include "generator_bounds.hpp"

namespace config {
    template<std::size_t N, typename EnumMask>
    static void populate_special(coefficient_tuple_t<N>& tup, const EnumMask& mask, const std::array<typename EnumMask::enum_t, N>& tests){
        auto impl = [&mask, &tests]<std::size_t ... I>(std::index_sequence<I...>, coefficient_tuple_t<N>& result){
            (..., (std::get<I>(result) = (coefficient_t)(mask.test(tests[I]))));
        };
        return impl(std::make_index_sequence<N>{}, tup);
    }

    coefficient_t filter_kingdom_table(const kingdom::table_t& table, const kingdom::ExpansionEditionFilter& expansion_edition, const kingdom::TableQuery& query) noexcept {
        return std::ranges::fold_left(table |
                                      std::views::filter([&expansion_edition, &query](const kingdom::row_t& row) { return
                                              expansion_edition.test(row.expansion_edition_) &&
                                              query.tracked_types_mask_ == row.tracked_types_mask_ &&
                                              query.cost_group_ == row.cost_group_;}) |
                                      std::views::transform([](const kingdom::row_t& row) { return row.amount_; }),
                                      coefficient_t{}, std::plus{});
    }

    coefficient_t filter_landscapes_table(const landscapes::table_t& table, const landscapes::ExpansionFilter& expansion, const landscapes::TableQuery& query) noexcept {
        return std::ranges::fold_left(table |
                                      std::views::filter([&expansion, &query](const landscapes::row_t& row){ return
                                              expansion.test(row.expansion_) &&
                                              query.type_.test(row.type_);}) |
                                      std::views::transform([](const landscapes::row_t& row){return row.amount_;}),
                                      coefficient_t{}, std::plus{});
    }

    kingdom::query_amounts_t kingdom_query_bounds(const kingdom::ExpansionEditionFilter& l) noexcept {
        auto impl = [&l]<std::size_t ... I>(std::index_sequence<I...>){
            kingdom::query_amounts_t result{};
            const auto& amount_queries = kingdom::amount_queries();
            (..., (std::get<I>(result) = filter_kingdom_table(kingdom::table(), l, std::get<I>(amount_queries))));
            return result;
        };
        return impl(std::make_index_sequence<std::tuple_size_v<kingdom::queries_t>>{});
    }

    kingdom::special_amounts_t kingdom_special_bounds(const kingdom::ExpansionEditionFilter& l) noexcept {
        kingdom::special_amounts_t result{};
        populate_special(result, l, kingdom::special_tests());
        return result;
    }

    landscapes::query_amounts_t landscape_query_bounds(const landscapes::ExpansionFilter& l) noexcept {
        auto impl = [&l]<std::size_t ... I>(std::index_sequence<I...>){
            landscapes::query_amounts_t result{};
            const auto& amount_queries = landscapes::amount_queries();
            (..., (std::get<I>(result) = filter_landscapes_table(landscapes::table(), l, std::get<I>(amount_queries))));
            return result;
        };
        return impl(std::make_index_sequence<std::tuple_size_v<landscapes::queries_t>>{});
    }

    landscapes::special_amounts_t landscapes_special_bounds(const landscapes::ExpansionFilter& l) noexcept {
        landscapes::special_amounts_t result{};
        populate_special(result, l, landscapes::special_tests());
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

    GeneratorBounds from_expansions_editions(const kingdom::ExpansionEditionFilter& l) noexcept {
        const auto el = expansion_only(l);

        return GeneratorBounds{
            .kingdom_ = std::tuple_cat(kingdom_query_bounds(l), kingdom_special_bounds(l)),
            .landscapes_ = std::tuple_cat(landscape_query_bounds(el), landscapes_special_bounds(el))
        };
    }
}