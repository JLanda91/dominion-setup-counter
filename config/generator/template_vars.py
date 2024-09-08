class TemplateVars:
    @staticmethod
    def _edition_modifier(excel_expansion_name: str):
        match excel_expansion_name:
            case "1ST":
                return "None"
            case "2ND":
                return "Update Pack"
            case "1RC":
                return "Removed"
            case _:
                raise KeyError(f"No edition modifier conversion for {excel_expansion_name}")

    @staticmethod
    def _uniques_from_ordered_list(seq):
        seen = set()
        seen_add = seen.add
        return list(x for x in seq if not (x in seen or seen_add(x)))

    def __init__(self, card_list, generator_config):
        self._card_list = card_list
        self._generator_config = generator_config
        self._data = dict()
        self._data["tables"] = {"kingdom": self._kingdom_table(), "landscapes": self._landscapes_table()}
        self._data["specials"] = {"kingdom": self._kingdom_specials(), "landscapes": self._landscapes_specials()}
        self._data["enums"] = self._enums()
        self._data["amount_getters"] = {"kingdom": self._kingdom_amount_getters(),
                                        "landscapes": self._landscapes_amount_getters()}
        self._data["tuple_indices"] = self._tuple_indices()
        self._data["generator_bounds_tests"] = self._generator_bounds_tests()

    def _kingdom_table(self):
        table = self._card_list["kingdom_regular"]
        queries = self._card_list["kingdom_queries"]
        return {
            "data": list({
                             "expansion": expansion,
                             "edition_modifier": self._edition_modifier(edition),
                             "cost_group": cost_group,
                             "tracked_types": tracked_types,
                             "amount": amount
                         } for (expansion, edition, cost_group, tracked_types), amount in table.itertuples()),
            "queries": list({
                                "name": f"{' '.join(tracked_types) if tracked_types else 'none'}_{cost_group}",
                                "tracked_types": tracked_types,
                                "cost_group": cost_group,
                            } for (tracked_types, cost_group), _ in queries.itertuples())
        }

    def _landscapes_table(self):
        table = self._card_list["landscapes_regular"]
        return {
            "data": list({
                             "expansion": expansion,
                             "type": landscape_type,
                             "amount": amount
                         } for (expansion, landscape_type), amount in table.itertuples()),
            "queries": [{
                "name": supply_landscape_group["name"],
                "types": supply_landscape_group["strings"],
            } for supply_landscape_group in self._generator_config["supply_landscape_groups"]]
        }

    def _kingdom_specials(self):
        source_table = self._card_list["kingdom_special"]
        return {
            "data": list({
                             "name": name,
                             "expansion": expansion,
                             "edition_modifier": self._edition_modifier(edition),
                         } for _, name, expansion, edition in source_table.itertuples())
        }

    def _landscapes_specials(self):
        source_table = self._card_list["landscapes_special"]
        return {
            "data": list({
                             "name": name,
                             "expansion": expansion,
                         } for _, name, expansion in source_table.itertuples())
        }

    def _enums(self):
        result = list()
        result.append(
            {
                "name": "Expansion",
                "data": self._generator_config["supported_expansions"]
            })
        unique_editions = set(r[1] for r in self._card_list["kingdom_regular"].index)
        result.append(
            {
                "ns": "kingdom",
                "name": "EditionModifier",
                "data": list(self._edition_modifier(e) for e in unique_editions)
            })
        result.append(
            {
                "ns": "kingdom",
                "name": "CostGroup",
                "data": list(cg["name"] for cg in self._generator_config["cost_groups"])
            })
        result.append(
            {
                "ns": "kingdom",
                "name": "TrackedType",
                "size": len(self._generator_config["tracked_kingdom_card_types"]),
                "data": self._generator_config["tracked_kingdom_card_types"]
            })
        result.append(
            {
                "ns": "landscapes",
                "name": "Type",
                "size": len(self._card_list["landscapes_queries"]),
                "data": list(self._card_list["landscapes_queries"].index.get_level_values('Type'))
            }
        )
        return result

    def _kingdom_amount_getters(self):
        def filter_queries(q_filter):
            return ',\n\t\t\t\t'.join(
                f'AmountIndex::{q["name"]}' for q in self._data["tables"]["kingdom"]["queries"] if q_filter(q))

        return [
            {  # Looter
                "name": "looter",
                "tuple_index": filter_queries(lambda q: 'LOOTER' in q["tracked_types"]),
            },
            {  # Fate
                "name": "fate",
                "tuple_index": filter_queries(lambda q: 'FATE' in q["tracked_types"]),
            },
            {  # Doom
                "name": "doom",
                "tuple_index": filter_queries(lambda q: 'DOOM' in q["tracked_types"]),
            },
            {  # Liaison
                "name": "liaison",
                "tuple_index": filter_queries(lambda q: 'LIAISON' in q["tracked_types"]),
            },
            {  # Loot
                "name": "loot",
                "tuple_index": filter_queries(lambda q: 'LOOT' in q["tracked_types"]),
            },
            {  # Young Witch
                "name": "two_three",
                "tuple_index": filter_queries(lambda q: q["cost_group"] in ('TWO', 'THREE')),
            },
            {  # Ferryman
                "name": "three_four",
                "tuple_index": filter_queries(lambda q: q["cost_group"] in ('THREE', 'FOUR')),
                "count_unused": True,
            },
            {  # Riverboat
                "name": "action_nonduration_five",
                "tuple_index": filter_queries(
                    lambda q: (q["cost_group"] == 'FIVE') and ('ACTION' in q["tracked_types"]) and (
                            'DURATION' not in q["tracked_types"])),
                "count_unused": True,
            },
            {  # Obelisk
                "name": "action",
                "tuple_index": filter_queries(lambda q: 'ACTION' in q["tracked_types"]),
            },
            {  # Way Of The Mouse
                "name": "action_two_three",
                "tuple_index": filter_queries(
                    lambda q: ('ACTION' in q["tracked_types"]) and (q["cost_group"] in ('TWO', 'THREE'))),
                "count_unused": True,
            },
            {  # Approaching Army
                "name": "attack",
                "tuple_index": filter_queries(lambda q: 'ATTACK' in q["tracked_types"]),
                "count_unused": True,
            },
            {  # Traits
                "name": "action_or_treasure",
                "tuple_index": filter_queries(
                    lambda q: any(map(lambda t: t in q["tracked_types"], ("ACTION", "TREASURE",)))),
            },
        ]

    def _landscapes_amount_getters(self):
        queries = self._data["tables"]["landscapes"]["queries"]
        return [
            {
                "name": query["name"].lower(),
                "tuple_index": f"AmountIndex::{query['name']}"
            } for query in queries
        ]

    def _tuple_indices(self):
        return [
            {
                "ns": ns,
                "names": list(q["name"] for q in self._data["tables"][ns]["queries"]) + list(
                    q["name"].upper() for q in self._data["specials"][ns]["data"])
            } for ns in ("kingdom", "landscapes",)
        ]

    def _generator_bounds_tests(self):
        kingdom_regular_table = self._card_list["kingdom_regular"]
        kingdom_queries_table = self._card_list["kingdom_queries"]
        kingdom_special_table = self._card_list["kingdom_special"]
        landscapes_special_table = self._card_list["landscapes_special"]
        result = [
                     {
                         "name": "all_expansion_editions",
                         "all": True,
                         "kingdom_outcome": list(
                             {
                                 "name": f"{'_'.join(tracked_types) if tracked_types else 'NONE'}_{cost_group}",
                                 "amount": amount
                             } for (tracked_types, cost_group), amount in kingdom_queries_table.itertuples())
                                            + list({"name": name.upper(), "amount": 1} for _, name, _, _ in
                                                   kingdom_special_table.itertuples()),
                         "landscapes_outcome": list(
                             {"name": query["name"], "amount": sum(
                                 row["amount"] for row in self._data["tables"]["landscapes"]["data"] if
                                 row["type"] in query["types"])} for query in
                             self._data["tables"]["landscapes"]["queries"]) + list(
                             {"name": name.upper(), "amount": 1} for _, name, expansion in
                             landscapes_special_table.itertuples()),
                     }
                 ] + [
                     {
                         "name": test["name"],
                         "expansion_editions": test["expansion_editions"],
                         "kingdom_outcome": [
                                                {
                                                    "name": query["name"],
                                                    "amount": sum(
                                                        row["amount"] for row in self._data["tables"]["kingdom"]["data"]
                                                        if
                                                        (row["expansion"], row["edition_modifier"]) in (tuple(t.values()) for t
                                                                                                        in test[
                                                                                                            "expansion_editions"]) and
                                                        row["cost_group"] == query["cost_group"] and row[
                                                            "tracked_types"] == query["tracked_types"])
                                                } for query in self._data["tables"]["kingdom"]["queries"]
                                            ] + [
                                                {
                                                    "name": special["macro"],
                                                    "amount": 1 if (special["expansion"],
                                                                    special["edition_modifier"]) in (tuple(t.values()) for t in
                                                                                                     test[
                                                                                                         "expansion_editions"]) else 0
                                                } for special in self._data["specials"]["kingdom"]["data"]
                                            ],
                         "landscapes_outcome": [
                                                   {
                                                       "name": query["name"],
                                                       "amount": sum(row["amount"] for row in
                                                                     self._data["tables"]["landscapes"]["data"] if
                                                                     row["expansion"] in (t["expansion"] for t in test[
                                                                         "expansion_editions"]) and row["type"] in
                                                                     query["types"])
                                                   } for query in self._data["tables"]["landscapes"]["queries"]
                                               ] + [
                                                   {
                                                       "name": special["macro"],
                                                       "amount": 1 if special["expansion"] in (t["expansion"] for t in
                                                                                               test[
                                                                                                   "expansion_editions"]) else 0
                                                   } for special in self._data["specials"]["landscapes"]["data"]
                                               ]
                     } for test in self._generator_config["generator_bounds_tests"]
                 ]
        for test in result:
            test["kingdom_outcome"][-1]["last_element"] = True
            test["landscapes_outcome"][-1]["last_element"] = True
        return result

    def to_dict(self):
        return self._data



template_vars = TemplateVars()