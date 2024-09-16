import cog

from generator_base import BaseGenerator
from generator_utils import create_enum_class, create_singleton, upper_snake_case, full_query_name


class CardAmountTablesGenerator(BaseGenerator):
    def __init__(self, card_lists, generator_config):
        super().__init__(card_lists, generator_config)

    def create_kingdom_table(self):
        def tuple_printer(df_tup):
            (expansion, edition_modifier, cost_group, tracked_types), amount = df_tup
            cog.out(f"\t\t{{ {{ Expansion::{upper_snake_case(expansion)}, EditionModifier::{upper_snake_case(edition_modifier)} }}, {{ ")
            for tracked_type in tracked_types:
                cog.out(f"TrackedType::{upper_snake_case(tracked_type)}, ")
            cog.outl(f"}}, CostGroup::{upper_snake_case(cost_group)}, {amount} }},")
        create_singleton("table", "table_row_t", len(self._kingdom_regular_df), self._kingdom_regular_df.itertuples(), tuple_printer)

    def create_kingdom_amount_queries(self):
        def tuple_printer(df_tup):
            (tracked_types, cost_group), _ = df_tup
            cog.out(f"\t\t{{ {{ ")
            for tracked_type in tracked_types:
                cog.out(f"TrackedType::{upper_snake_case(tracked_type)}, ")
            cog.outl(f"}}, CostGroup::{upper_snake_case(cost_group)} }}, //\t{full_query_name(tracked_types, cost_group).upper()}")
        create_singleton("amount_queries", "TableQuery", len(self._kingdom_queries_df), self._kingdom_queries_df.itertuples(), tuple_printer)

    def create_kingdom_special_predicates(self):
        def tuple_printer(df_tup):
            _, name, expansion, edition_modifier = df_tup
            cog.outl(f"\t\t{{ Expansion::{upper_snake_case(expansion)}, EditionModifier::{upper_snake_case(edition_modifier)} }}, //\t{upper_snake_case(name)}")
        create_singleton("special_predicates", "typename ExpansionEditionFilter::enum_t", len(self._kingdom_special_df), self._kingdom_special_df.itertuples(), tuple_printer)

    def create_kingdom_amount_index(self):
        amount_query_names = list(full_query_name(tracked_types, cost_group).upper() for (tracked_types, cost_group), _ in self._kingdom_queries_df.itertuples())
        special_names = list(upper_snake_case(name) for _, name, expansion, edition_modifier in self._kingdom_special_df.itertuples())
        create_enum_class('AmountIndex', amount_query_names + special_names, 'std::size_t', False)

    def create_landscapes_supply_table(self):
        def tuple_printer(df_tup):
            (expansion, landscape_type), amount = df_tup
            cog.outl(f"\t\t{{ Expansion::{upper_snake_case(expansion)}, Type::{upper_snake_case(landscape_type)}, {amount} }},")
        create_singleton("table", "table_row_t", len(self._landscapes_supply_regular_df), self._landscapes_supply_regular_df.itertuples(), tuple_printer)

    def create_landscapes_other_table(self):
        def tuple_printer(df_tup):
            (expansion, landscape_type), amount = df_tup
            cog.outl(f"\t\t{{ Expansion::{upper_snake_case(expansion)}, Type::{upper_snake_case(landscape_type)}, {amount} }},")
        create_singleton("table", "table_row_t", len(self._landscapes_other_table_df), self._landscapes_other_table_df.itertuples(), tuple_printer)

    def create_landscapes_supply_queries(self):
        def tuple_printer(supply_landscape_group):
            cog.out("\t\t{ { ")
            for landscape_type in supply_landscape_group["strings"]:
                cog.out(f"Type::{upper_snake_case(landscape_type)}, ")
            cog.outl("} },")
        create_singleton("amount_queries", "TableQuery", len(self._landscapes_supply_config), self._landscapes_supply_config, tuple_printer)

    def create_landscapes_supply_specials(self):
        def tuple_printer(df_tup):
            _, name, expansion = df_tup
            cog.outl(f"\t\tExpansion::{upper_snake_case(expansion)},\t// {upper_snake_case(name)}")
        create_singleton("special_predicates", "typename ExpansionFilter::enum_t", len(self._landscapes_supply_special_df), self._landscapes_supply_special_df.itertuples(), tuple_printer)

    def create_landscapes_amount_index(self):
        amount_query_names = list(upper_snake_case(landscape_group["name"]) for landscape_group in self._landscapes_supply_config)
        special_names = list(upper_snake_case(name) for _, name, expansion in self._landscapes_supply_special_df.itertuples())
        create_enum_class('AmountIndex', amount_query_names + special_names, 'std::size_t', False)

