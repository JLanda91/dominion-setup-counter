from itertools import product, compress

import cog

from generator_base import BaseGenerator
from generator_utils import full_query_name, lower_snake_case, upper_snake_case


class AmountGetterGenerator(BaseGenerator):
    def __init__(self, card_lists, generator_config):
        super().__init__(card_lists, generator_config)

    @staticmethod
    def create_enum_sum_functions():
        cog.outl("""template<AmountIndex ... E>
auto enum_index_get_sum(const amounts_t& t) noexcept -> coefficient_t {
    return (... + (std::get<std::to_underlying(E)>(t)));
}
        
template<AmountIndex ... E>
auto enum_index_get_sum_diff(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t {
    return (... + (std::get<std::to_underlying(E)>(max) - std::get<std::to_underlying(E)>(t)));
}
""")

    def _kingdom_amount_getters(self):
        def filter_queries(q_filter):
            return list(full_query_name(tracked_types, cost_group).upper() for (tracked_types, cost_group), _ in self._kingdom_queries_df.itertuples() if q_filter(tracked_types, cost_group))

        return dict((kingdom_amount_getter_name, {
            "function_name": lower_snake_case(kingdom_amount_getter_attrs["function"]),
            "tuple_index": filter_queries(eval(kingdom_amount_getter_attrs["predicate"])),
            "count_unused": kingdom_amount_getter_attrs.get("count_unused", False)
        }) for kingdom_amount_getter_name, kingdom_amount_getter_attrs in self._kingdom_specials_and_getters.items() if kingdom_amount_getter_attrs["predicate"] is not None)

    def create_kingdom_amount_getter_declarations(self):
        for amount_getter in self._kingdom_amount_getters().values():
            if amount_getter.get("count_unused", False):
                cog.outl(f"""auto unused_{amount_getter["function_name"]}_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t;""")
            else:
                cog.outl(f"""auto {amount_getter["function_name"]}_total(const amounts_t& t) noexcept -> coefficient_t;""")
        cog.outl()

    def create_kingdom_amount_getter_definitions(self):
        for amount_getter in self._kingdom_amount_getters().values():
            if amount_getter.get("count_unused", False):
                cog.outl(f"""auto unused_{amount_getter["function_name"]}_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t {{
    return enum_index_get_sum_diff<""")

            else:
                cog.outl(f"""auto {amount_getter["function_name"]}_total(const amounts_t& t) noexcept -> coefficient_t {{
    return enum_index_get_sum<""")
            for index in amount_getter["tuple_index"][:-1]:
                cog.outl(f"\t\tAmountIndex::{upper_snake_case(index)},")
            cog.outl(f"""\t\tAmountIndex::{upper_snake_case(amount_getter["tuple_index"][-1])}""")
            if amount_getter.get("count_unused", False):
                cog.outl("\t>(t, max);")
            else:
                cog.outl("\t>(t);")
            cog.outl("}\n")

    def create_landscapes_supply_amount_getter_declarations(self):
        for landscape_group in self._landscapes_supply_config:
            cog.outl(f"""auto {lower_snake_case(landscape_group["name"])}_total(const amounts_t& t) noexcept -> coefficient_t;""")
        cog.outl()

    def create_landscapes_supply_amount_getter_definitions(self):
        for landscape_group in self._landscapes_supply_config:
            cog.outl(f"""auto {lower_snake_case(landscape_group["name"])}_total(const amounts_t& t) noexcept -> coefficient_t {{
    return std::get<std::to_underlying(AmountIndex::{upper_snake_case(landscape_group["name"])})>(t);
}}
""")

    def create_kingdom_special_getter_declarations(self):
        for _, name, _, _ in self._kingdom_special_df.itertuples():
            cog.outl(f"""auto has_{lower_snake_case(name)}(const amounts_t& t) noexcept -> bool;""")
        cog.outl()

    def create_kingdom_special_getter_definitions(self):
        for _, name, _, _ in self._kingdom_special_df.itertuples():
            cog.outl(f"""auto has_{lower_snake_case(name)}(const amounts_t& t) noexcept -> bool {{
    return std::get<std::to_underlying(AmountIndex::{upper_snake_case(name)})>(t) > 0u;
}}
""")

    def create_landscapes_supply_special_getter_declarations(self):
        for _, name, _ in self._landscapes_supply_special_df.itertuples():
            cog.outl(f"""auto has_{lower_snake_case(name)}(const amounts_t& t) noexcept -> bool;""")
        cog.outl()

    def create_landscapes_supply_special_getter_definitions(self):
        for _, name, _ in self._landscapes_supply_special_df.itertuples():
            cog.outl(f"""auto has_{lower_snake_case(name)}(const amounts_t& t) noexcept -> bool {{
    return std::get<std::to_underlying(AmountIndex::{upper_snake_case(name)})>(t) > 0u;
}}
""")

    def create_landscapes_other_amount_getter_declarations(self):
        for landscape_type in self._landscapes_other_types:
            cog.outl(f"""auto {lower_snake_case(landscape_type)}_total(const amounts_t& t) noexcept -> coefficient_t;""")
        cog.outl()

    def create_landscapes_other_amount_getter_definitions(self):
        for landscape_type in self._landscapes_other_types:
            cog.outl(f"""auto {lower_snake_case(landscape_type)}_total(const amounts_t& t) noexcept -> coefficient_t {{
    return std::get<std::to_underlying(Type::{upper_snake_case(landscape_type)})>(t);
}}
""")
        cog.outl()

    # def create_ways_picking_unused_cards_definition(self):
    #     special_names = [special for special in (self._kingdom_special_df["Name"].unique().tolist() + self._landscapes_supply_special_df["Name"].unique().tolist())]
    #     special_getters = dict((getter_k, getter_v) for (getter_k, getter_v) in self._kingdom_amount_getters().items() if (getter_k.lower() in (s.lower() for s in special_names)) and getter_v["count_unused"])
    #     print(special_names)
    #     print(*special_getters,sep='\n')
    #     special_arguments = ', '.join(f"""bool has_{lower_snake_case(special_getter_name)}""" for special_getter_name in special_getters.keys())
    #     cog.outl(f"""auto ways_picking_unused_cards(const kingdom::amounts_t& t, const kingdom::amounts_t& max, {special_arguments}) noexcept -> bool {{""")
    #     compressor_list = list(filter(lambda t: sum(t) >= 2, product((False, True), repeat=len(special_getters))))
    #     compressor_list.sort(key=lambda x: sum(x))
    #     for compressor in (tuple(reversed(c)) for c in compressor_list):
    #         inverse = tuple(not x for x in compressor)
    #         intersecting_sets = list(compress(special_getters.keys(), compressor))
    #         single_sets = list(compress(special_getters.keys(), inverse))
    #         print(f"intersecting names: {intersecting_sets}. Single names: {single_sets}")
    #         intersection_index = set.intersection(*(set(special_getters[s]["tuple_index"]) for s in intersecting_sets))
    #         print(f"\t{intersection_index}")
    #
    #     cog.outl("}")