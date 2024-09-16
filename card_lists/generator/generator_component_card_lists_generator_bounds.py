import cog

from generator_base import BaseGenerator
from generator_utils import full_query_name, upper_snake_case


class ComponentCardListsGeneratorBoundsGenerator(BaseGenerator):
    def __init__(self, card_lists, generator_config):
        super().__init__(card_lists, generator_config)

    def create_tests(self):
        self._create_test()
        for generator_bounds_test in self._generator_bounds_tests:
            self._create_test(generator_bounds_test["name"], generator_bounds_test["expansion_editions"])

    def _create_test(self, name="all_expansion_editions", expansion_editions=None):
        cog.outl(f"TEST(generator_bounds, {name}) {{")
        if expansion_editions is None:
            cog.outl("\tconst auto& expansion_edition_filter = kAllExpansionEditions;")
        else:
            cog.outl("\tconst auto expansion_edition_filter = ExpansionEditionFilter{")
            for expansion_edition in expansion_editions:
                cog.outl(f"""\t\t{{ Expansion::{upper_snake_case(expansion_edition["expansion"])}, EditionModifier::{upper_snake_case(expansion_edition["edition_modifier"])} }},""")
            cog.outl("\t};")
        cog.outl()
        cog.outl("""\tconst auto actual = from_expansions_editions(expansion_edition_filter);
    ASSERT_THAT(actual.kingdom_, FieldsAre(""")
        if expansion_editions is None:
            for (tracked_types, cost_group), amount in self._kingdom_queries_df.itertuples():
                cog.outl(f"\t\t(coefficient_t){amount}u,\t// {full_query_name(tracked_types, cost_group).upper()}")
        else:
            for (tracked_types, cost_group), _ in self._kingdom_queries_df.itertuples():
                index_predicate = list((expansion_edition["expansion"], expansion_edition["edition_modifier"], cost_group, tracked_types) for expansion_edition in expansion_editions)
                amount = self._kingdom_regular_df[self._kingdom_regular_df.index.isin(index_predicate)]["Count"].sum()
                cog.outl(f"\t\t(coefficient_t){amount}u,\t// {full_query_name(tracked_types, cost_group).upper()}")

        cog.outl()
        if expansion_editions is None:
            for i, (_, name, _, _) in enumerate(self._kingdom_special_df.itertuples()):
                cog.outl(f"\t\t(coefficient_t)1u{',' if i < len(self._kingdom_special_df) -1 else ''}\t// {upper_snake_case(name)}")
        else:
            for i, (_, name, expansion, edition) in enumerate(self._kingdom_special_df.itertuples()):
                amount = int((expansion, edition) in (tuple(ee.values()) for ee in expansion_editions))
                cog.outl(f"\t\t(coefficient_t){amount}u{',' if i < len(self._kingdom_special_df) -1 else ''}\t// {upper_snake_case(name)}")
        cog.outl("""\t));

    ASSERT_THAT(actual.landscapes_supply_, FieldsAre(""")
        if expansion_editions is None:
            for landscape_group in self._landscapes_supply_config:
                amount = self._landscapes_supply_queries_df[self._landscapes_supply_queries_df.index.isin(landscape_group["strings"])]["Count"].sum()
                cog.outl(f"""\t\t(coefficient_t){amount}u,\t// {upper_snake_case(landscape_group["name"])}""")
        else:
            for landscape_group in self._landscapes_supply_config:
                amount = self._landscapes_supply_regular_df[self._landscapes_supply_regular_df.index.get_level_values('Type').isin(landscape_group["strings"]) & self._landscapes_supply_regular_df.index.get_level_values('Expansion').isin((ee["expansion"] for ee in expansion_editions))]["Count"].sum()
                cog.outl(f"""\t\t(coefficient_t){amount}u,\t// {upper_snake_case(landscape_group["name"])}""")
        cog.outl()
        if expansion_editions is None:
            for i, (_, name, _) in enumerate(self._landscapes_supply_special_df.itertuples()):
                cog.outl(f"\t\t(coefficient_t)1u{',' if i < len(self._landscapes_supply_special_df) -1 else ''}\t// {upper_snake_case(name)}")
        else:
            for i, (_, name, expansion) in enumerate(self._landscapes_supply_special_df.itertuples()):
                amount = int(expansion in (ee["expansion"] for ee in expansion_editions))
                cog.outl(f"\t\t(coefficient_t){amount}u{',' if i < len(self._landscapes_supply_special_df) -1 else ''}\t// {upper_snake_case(name)}")
        cog.outl("""\t));

    ASSERT_THAT(actual.landscapes_other_, FieldsAre(""")
        if expansion_editions is None:
            for i, landscape_type in enumerate(self._landscapes_other_types):
                amount = self._landscapes_other_queries_df[self._landscapes_other_queries_df.index.get_level_values('Type') == landscape_type]["Count"].sum()
                cog.outl(f"""\t\t(coefficient_t){amount}u{',' if i < len(self._landscapes_other_queries_df) -1 else ''}\t// {upper_snake_case(landscape_type)}""")
        else:
            for i, landscape_type in enumerate(self._landscapes_other_types):
                amount = self._landscapes_other_table_df[(self._landscapes_other_table_df.index.get_level_values('Type') == landscape_type) & (self._landscapes_other_table_df.index.get_level_values('Expansion').isin((ee["expansion"] for ee in expansion_editions)))]["Count"].sum()
                cog.outl(f"""\t\t(coefficient_t){amount}u{',' if i < len(self._landscapes_other_queries_df) -1 else ''}\t// {upper_snake_case(landscape_type)}""")
        cog.outl("""\t));
}
""")