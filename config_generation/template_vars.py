class TemplateVars:

    @staticmethod
    def _edition_modifier(excel_expansion_name: str):
        match excel_expansion_name:
            case "1ST":
                return "NONE"
            case "2ND":
                return "UPDATE_PACK"
            case "1RC":
                return "REMOVED"
            case _:
                raise KeyError(f"No edition modifier conversion for {excel_expansion_name}")

    @staticmethod
    def _macro_name(inp):
        return str(inp).upper().translate(str.maketrans({' ': '_'}))

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
        self._data["default_editions"] = self._default_editions()
        self._data["amount_getters"] = self._amount_getters()

    def _kingdom_table(self):
        table = self._card_list["kingdom_regular"]
        queries = self._card_list["kingdom_queries"]
        return {
            "size": len(table),
            "data": list({
                             "expansion": self._macro_name(expansion),
                             "edition_modifier": self._edition_modifier(edition),
                             "cost_group": cost_group,
                             "tracked_types": tracked_types,
                             "amount": amount
                         } for (expansion, edition, cost_group, tracked_types), amount in table.itertuples()),
            "num_queries": len(queries),
            "queries": list({
                                "name": f"{'_'.join(tracked_types) if tracked_types else 'NONE'}_{cost_group}",
                                "tracked_types": tracked_types,
                                "cost_group": cost_group
                            } for (tracked_types, cost_group), _ in queries.itertuples())
        }

    def _landscapes_table(self):
        table = self._card_list["landscapes_regular"]
        return {
            "size": len(table),
            "data": list({
                         "expansion": self._macro_name(expansion),
                         "type": landscape_type,
                         "amount": amount
                     } for (expansion, landscape_type), amount in table.itertuples()),
            "num_queries": 1,
            "queries": [{
                    "name": "REGULAR",
                    "types": self._generator_config["supply_landscape_types"],
            }, ]
        }

    def _kingdom_specials(self):
        source_table = self._card_list["kingdom_special"]
        return {
            "namespace": "kingdom",
            "size": len(source_table),
            "data": list({
                             "name": self._macro_name(name),
                             "expansion": self._macro_name(expansion),
                             "edition_modifier": self._edition_modifier(edition),
                         } for _, name, expansion, edition in source_table.itertuples())
        }

    def _landscapes_specials(self):
        source_table = self._card_list["landscapes_special"]
        return {
            "namespace": "landscapes",
            "size": len(source_table),
            "data": list({
                             "name": self._macro_name(name),
                             "expansion": self._macro_name(expansion),
                         } for _, name, expansion in source_table.itertuples())
        }

    def _enums(self):
        result = list()
        result.append(
            {
                "name": "Expansion",
                "size": len(self._generator_config["supported_expansions"]),
                "data": self._generator_config["supported_expansions"]
            })
        unique_editions = set(r[1] for r in self._card_list["kingdom_regular"].index)
        result.append(
            {
                "ns": "kingdom",
                "name": "EditionModifier",
                "size": len(unique_editions),
                "data": list(self._edition_modifier(e) for e in unique_editions)
            })
        result.append(
            {
                "ns": "kingdom",
                "name": "CostGroup",
                "size": len(self._generator_config["cost_groups"]),
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
                "size": len(self._card_list["landscapes_types"]),
                "data": self._card_list["landscapes_types"].to_list()
            }
        )
        return result

    def _default_editions(self):
        iterable = self._uniques_from_ordered_list(list(
            (x["expansion"], x["edition_modifier"]) for x in self._data["tables"]["kingdom"]["data"] if
            x["edition_modifier"] != "REMOVED"))
        return list({
                        "expansion": expansion,
                        "edition_modifier": edition_modifier
                    } for expansion, edition_modifier in iterable)

    def _amount_getters(self):
        def kingdom_query_filter_str(q_filter):
            return ',\n\t\t\t'.join(f'AmountIndex::{q["name"]}' for q in self._data["tables"]["kingdom"]["queries"] if q_filter(q))

        return [
            {   # Looter
                "name": "looter",
                "tuple_indices": kingdom_query_filter_str(lambda q: 'LOOTER' in q["tracked_types"]),
            },
            {   # Fate
                "name": "fate",
                "tuple_indices": kingdom_query_filter_str(lambda q: 'FATE' in q["tracked_types"]),
            },
            {   # Doom
                "name": "doom",
                "tuple_indices": kingdom_query_filter_str(lambda q: 'DOOM' in q["tracked_types"]),
            },
            {   # Liaison
                "name": "liaison",
                "tuple_indices": kingdom_query_filter_str(lambda q: 'LIAISON' in q["tracked_types"]),
            },
            {   # Loot
                "name": "loot",
                "tuple_indices": kingdom_query_filter_str(lambda q: 'LOOT' in q["tracked_types"]),
            },
            {   # Young Witch
                "name": "two_three",
                "tuple_indices": kingdom_query_filter_str(lambda q: q["cost_group"] in ('TWO', 'THREE')),
            },
            {   # Ferryman
                "name": "three_four",
                "tuple_indices": kingdom_query_filter_str(lambda q:  q["cost_group"] in ('THREE', 'FOUR')),
                "count_unused": True,
            },
            {   # Riverboat
                "name": "action_nonduration_five",
                "tuple_indices": kingdom_query_filter_str(lambda q:  (q["cost_group"] == 'FIVE') and ('ACTION' in q["tracked_types"]) and ('DURATION' not in q["tracked_types"])),
                "count_unused": True,
            },
            {   # Obelisk
                "name": "action",
                "tuple_indices": kingdom_query_filter_str(lambda q: 'ACTION' in q["tracked_types"]),
            },
            {   # Way Of The Mouse
                "name": "action_two_three",
                "tuple_indices": kingdom_query_filter_str(lambda q: ('ACTION' in q["tracked_types"]) and (q["cost_group"] in ('TWO', 'THREE'))),
                "count_unused": True,
            },
            {   # Approaching Army
                "name": "attack",
                "tuple_indices": kingdom_query_filter_str(lambda q: 'ATTACK' in q["tracked_types"]),
                "count_unused": True,
            },
        ]

    def to_dict(self):
        return self._data
