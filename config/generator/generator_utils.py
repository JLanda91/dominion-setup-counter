import cog


_underscore_trans = str.maketrans({' ': '_'})


def upper_snake_case(input_str: str):
    return input_str.upper().translate(_underscore_trans)


def lower_snake_case(input_str: str):
    return input_str.lower().translate(_underscore_trans)


def full_query_name(tracked_types, cost_group):
    return ('none' if len(tracked_types) == 0 else '_'.join(tracked_types)) + f"_{cost_group}"


def create_enum_class(name: str, values, underlying_type=None, create_sized_enum=True):
    cog.outl(f"""enum class {name} {f": {underlying_type}" if underlying_type is not None else ""} {{""")
    for x in values[:-1]:
        cog.outl(f"\t{upper_snake_case(x)},")
    cog.outl(f"\t{upper_snake_case(values[-1])}")
    cog.outl("""};
    """)
    if create_sized_enum:
        cog.outl(f"""using Sized{name} = utils::enums::SizedEnum<{name}, {len(values)}uz>;
        """)


def create_singleton(name: str, array_element_type: str, size: int, iterable, tuple_printer):
    cog.outl(f"""using {name}_t = std::array<{array_element_type}, {size}uz>;
    """)
    cog.outl(f"""static constexpr auto {name}() noexcept -> const {name}_t& {{
    static constexpr {name}_t singleton {{{{""")
    for t in iterable:
        tuple_printer(t)
    cog.outl("\t}};")
    cog.outl("\treturn singleton;")
    cog.outl("""}
    """)


def create_kingdom_table(kingdom_regular_df):
    def tuple_printer(df_tup):
        (expansion, edition_modifier, cost_group, tracked_types), amount = df_tup
        cog.out(f"\t\t{{ {{ Expansion::{upper_snake_case(expansion)}, EditionModifier::{upper_snake_case(edition_modifier)} }}, {{ ")
        for tracked_type in tracked_types:
            cog.out(f"TrackedType::{upper_snake_case(tracked_type)}, ")
        cog.outl(f"}}, CostGroup::{upper_snake_case(cost_group)}, {amount} }},")
    create_singleton("table", "row_t", len(kingdom_regular_df), kingdom_regular_df.itertuples(), tuple_printer)


def create_kingdom_amount_queries(kingdom_queries_df):
    def tuple_printer(df_tup):
        (tracked_types, cost_group), _ = df_tup
        cog.out(f"\t\t{{ {{ ")
        for tracked_type in tracked_types:
            cog.out(f"TrackedType::{upper_snake_case(tracked_type)}, ")
        cog.outl(f"}}, {{ CostGroup::{upper_snake_case(cost_group)} }} }}, //\t{full_query_name(tracked_types, cost_group).upper()}")
    create_singleton("amount_queries", "TableQuery", len(kingdom_queries_df), kingdom_queries_df.itertuples(), tuple_printer)


def create_kingdom_special_predicates(kingdom_special_df):
    def tuple_printer(df_tup):
        _, name, expansion, edition_modifier = df_tup
        cog.outl(f"\t\t{{ Expansion::{upper_snake_case(expansion)}, EditionModifier::{upper_snake_case(edition_modifier)} }}, //\t{upper_snake_case(name)}")
    create_singleton("special_predicates", "typename ExpansionEditionFilter::enum_t", len(kingdom_special_df), kingdom_special_df.itertuples(), tuple_printer)


def create_kingdom_amount_index(kingdom_queries_df, kingdom_special_df):
    amount_query_names = list(full_query_name(tracked_types, cost_group).upper() for (tracked_types, cost_group), _ in kingdom_queries_df.itertuples())
    special_names = list(upper_snake_case(name) for _, name, expansion, edition_modifier in kingdom_special_df.itertuples())
    create_enum_class('AmountIndex', amount_query_names + special_names, 'std::size_t', False)


def create_landscapes_supply_table(landscapes_supply_regular_df):
    def tuple_printer(df_tup):
        (expansion, landscape_type), amount = df_tup
        cog.outl(f"\t\t{{ Expansion::{upper_snake_case(expansion)}, Type::{upper_snake_case(landscape_type)}, {amount} }},")
    create_singleton("supply_table", "row_t", len(landscapes_supply_regular_df), landscapes_supply_regular_df.itertuples(), tuple_printer)


def create_landscapes_supply_queries(supply_landscape_groups):
    def tuple_printer(supply_landscape_group):
        cog.out("\t\t{ { ")
        for landscape_type in supply_landscape_group["strings"]:
            cog.out(f"Type::{upper_snake_case(landscape_type)}, ")
        cog.outl("} },")
    create_singleton("supply_queries", "TableQuery", len(supply_landscape_groups), supply_landscape_groups, tuple_printer)


def create_landscapes_supply_specials(landscapes_supply_special_df):
    def tuple_printer(df_tup):
        _, name, expansion = df_tup
        cog.outl(f"\t\tExpansion::{upper_snake_case(expansion)},\t// {upper_snake_case(name)}")
    create_singleton("supply_special_tests", "typename ExpansionFilter::enum_t", len(landscapes_supply_special_df), landscapes_supply_special_df.itertuples(), tuple_printer)


def create_landscapes_amount_index(supply_landscape_groups, landscapes_supply_special_df):
    amount_query_names = list(upper_snake_case(landscape_group["name"]) for landscape_group in supply_landscape_groups)
    special_names = list(upper_snake_case(name) for _, name, expansion in landscapes_supply_special_df.itertuples())
    create_enum_class('AmountIndex', amount_query_names + special_names, 'std::size_t', False)
