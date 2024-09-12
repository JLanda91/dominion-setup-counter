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

        return [
            {  # Looter
                "name": "looter",
                "tuple_index": filter_queries(lambda tt, cg: 'Looter' in tt),
            },
            {  # Fate
                "name": "fate",
                "tuple_index": filter_queries(lambda tt, cg: 'Fate' in tt),
            },
            {  # Doom
                "name": "doom",
                "tuple_index": filter_queries(lambda tt, cg: 'Doom' in tt),
            },
            {  # Liaison
                "name": "liaison",
                "tuple_index": filter_queries(lambda tt, cg: 'Liaison' in tt),
            },
            {  # Loot
                "name": "loot",
                "tuple_index": filter_queries(lambda tt, cg: 'Loot' in tt),
            },
            {  # Young Witch
                "name": "two_three",
                "tuple_index": filter_queries(lambda tt, cg: cg in ('Two', 'Three')),
            },
            {  # Ferryman
                "name": "three_four",
                "tuple_index": filter_queries(lambda tt, cg: cg in ('Three', 'Four')),
                "count_unused": True,
            },
            {  # Riverboat
                "name": "action_nonduration_five",
                "tuple_index": filter_queries(
                    lambda tt, cg: (cg == 'Five') and ('Action' in tt) and (
                            'Duration' not in tt)),
                "count_unused": True,
            },
            {  # Obelisk
                "name": "action",
                "tuple_index": filter_queries(lambda tt, cg: 'Action' in tt),
            },
            {  # Way Of The Mouse
                "name": "action_two_three",
                "tuple_index": filter_queries(
                    lambda tt, cg: ('Action' in tt) and (cg in ('Two', 'Three'))),
                "count_unused": True,
            },
            {  # Approaching Army
                "name": "attack",
                "tuple_index": filter_queries(lambda tt, cg: 'Attack' in tt),
                "count_unused": True,
            },
            {  # Traits
                "name": "action_or_treasure",
                "tuple_index": filter_queries(
                    lambda tt, cg: any(map(lambda t: t in tt, ("Action", "Treasure",)))),
            },
        ]

    def create_kingdom_amount_getter_declarations(self):
        for amount_getter in self._kingdom_amount_getters():
            if amount_getter.get("count_unused", False):
                cog.outl(f"""auto unused_{amount_getter["name"]}_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t;""")
            else:
                cog.outl(f"""auto {amount_getter["name"]}_total(const amounts_t& t) noexcept -> coefficient_t;""")
        cog.outl()

    def create_kingdom_amount_getter_definitions(self):
        for amount_getter in self._kingdom_amount_getters():
            if amount_getter.get("count_unused", False):
                cog.outl(f"""auto unused_{amount_getter["name"]}_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t {{
    return enum_index_get_sum_diff<""")

            else:
                cog.outl(f"""auto {amount_getter["name"]}_total(const amounts_t& t) noexcept -> coefficient_t {{
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