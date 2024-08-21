from itertools import chain, product

from pandas import Index, MultiIndex


class TemplateVars:

    _table_enums = {
        "": ("expansion", ),
        "kingdom": ("edition_modifier", "primary_type", "secondary_type", "cost"),
        "landscapes": ("type", ),
    }

    _groups = {
        "kingdom": {
            "primary_type": [
                {
                    "name": primary_type,
                    "members": (primary_type.upper(),)
                } for primary_type in ("action", "treasure", "other")
            ],
            "secondary_type": [
                {
                    "name": secondary_type,
                    "members": (secondary_type.upper(), )
                } for secondary_type in ("regular", "looter", "fate", "doom", "liaison", )
            ],
            "cost": [
                {
                    "name": cost,
                    "members": (cost.upper(), )
                } for cost in ("two", "three", "four", )
            ] + [
                {
                    "name": "other",
                    "members": ("ONE", "FIVE", "SIX", "SEVEN", "EIGHT", "POTION", "DEBT", )
                }
            ]
        },
        "landscapes": {
            "type": [
                {
                    "name": "setup",
                    "members": ("EVENT", "LANDMARK", "PROJECT", "WAY",)
                }
            ],
        },
    }

    _special = {
        "kingdom": [
            {
                "name": "KNIGHTS",
                "expansion": "DARK_AGES",
                "edition_modifier": "NONE"
            },
            {
                "name": "FERRYMAN",
                "expansion": "CORNUCOPIA_GUILDS",
                "edition_modifier": "UPDATE_PACK"
            },
            {
                "name": "DRUID",
                "expansion": "NOCTURNE",
                "edition_modifier": "NONE"
            },
        ],
        "landscapes": [
            {
                "name": "OBELISK",
                "expansion": "EMPIRES",
            },
            {
                "name": "WAY_OF_THE_MOUSE",
                "expansion": "MENAGERIE",
            },
        ]
    }

    @staticmethod
    def _uniques_from_ordered_list(seq):
        seen = set()
        seen_add = seen.add
        return list(x for x in seq if not (x in seen or seen_add(x)))

    @staticmethod
    def _expansion_name_from_special_card_type(special_card_type: str):
        match special_card_type.lower():
            case "looter":
                return "Dark Ages"
            case "fate" | "doom":
                return "Nocturne"
            case "liaison":
                return "Allies"
            case _:
                raise KeyError(f"No expansion for special card type {special_card_type}")

    @staticmethod
    def _edition_modifier(excel_expansion_name: str):
        if excel_expansion_name.lower().endswith("(up)"):
            return "UPDATE_PACK"
        if excel_expansion_name.lower().endswith("(rc)"):
            return "REMOVED"
        return "NONE"

    @staticmethod
    def _cost_macro(cost_char):
        return {
            '1': 'ONE',
            '2': 'TWO',
            '3': 'THREE',
            '4': 'FOUR',
            '5': 'FIVE',
            '6': 'SIX',
            '7': 'SEVEN',
            '8': 'EIGHT',
            'P': 'POTION',
            'D': 'DEBT',
        }[str(cost_char)]

    @staticmethod
    def _remove_edition(excel_expansion_name: str):
        lower_name = excel_expansion_name.lower()
        for edition_substr in ("(up)", "(rc)"):
            if edition_substr in lower_name:
                return lower_name.split(edition_substr)[0].rstrip()
        return lower_name

    @staticmethod
    def _macro_name(inp):
        return str(inp).upper().translate(str.maketrans({' ': '_'}))

    @staticmethod
    def _is_kingdom_section(section_name):
        match section_name.lower():
            case "action" | "treasure" | "other":
                return True
            case _:
                return False

    @staticmethod
    def _is_landscape_section(section_name):
        match section_name.lower():
            case "landscapes":
                return True
            case _:
                return False

    @staticmethod
    def _nnz_filter(col_val_tup):
        return col_val_tup[1] != 0

    def __init__(self, excel_config):
        self._data = {
            "tables": dict((table, {
                "data": list()
            }) for table in self._table_enums.keys() if table != ""),
        }

        for table_name, table_data in excel_config.items():
            if isinstance(table_data.keys(), MultiIndex):
                self.add_expansion_table(table_data)
            elif isinstance(table_data.keys(), Index):
                self.add_simple_table(table_name.lower(), table_data)
        self.add_table_sizes()
        self.add_enums()
        self.add_queries()
        self.add_specials()
        self.add_amount_indices()

    def add_simple_table(self, table_name, table_data):
        kingdom_table = self._data["tables"]["kingdom"]
        for row in table_data.itertuples():
            kingdom_table["data"].extend(({
                "expansion": self._macro_name(self._expansion_name_from_special_card_type(table_name)),
                "edition_modifier": "NONE",
                "primary_type": row[0].upper(),
                "secondary_type": self._macro_name(table_name),
                "cost": self._cost_macro(col_name),
                "amount": amount
            } for col_name, amount in filter(self._nnz_filter, zip(table_data.columns, row[1:]))))

    def add_expansion_table(self, table_data):
        self._data["tables"] = dict((table_name, {"data": list()}) for table_name in filter(lambda x: x != "", self._table_enums.keys()))
        self._data["default_editions"] = list()

        kingdom_table = self._data["tables"]["kingdom"]
        landscapes_table = self._data["tables"]["landscapes"]

        self._data["default_editions"] = list()
        for lower_expansion_edition in (x.lower() for x in table_data.index):
            if lower_expansion_edition.endswith("(up)"):
                self._data["default_editions"].append({
                    "expansion": self._macro_name(self._remove_edition(lower_expansion_edition)),
                    "edition_modifier": "UPDATE_PACK"
                })
            elif not lower_expansion_edition.endswith("(rc)"):
                self._data["default_editions"].append({
                    "expansion": self._macro_name(lower_expansion_edition),
                    "edition_modifier": "NONE"
                })

        for section_name in table_data.keys().unique(level=0):
            section_data = table_data[section_name]
            if self._is_kingdom_section(section_name):
                for row in section_data.itertuples():
                    kingdom_table["data"].extend(({
                        "expansion": self._macro_name(self._remove_edition(row[0])),
                        "edition_modifier": self._edition_modifier(row[0]),
                        "primary_type": self._macro_name(section_name),
                        "secondary_type": "REGULAR",
                        "cost": self._cost_macro(col_name),
                        "amount": amount
                    } for col_name, amount in filter(self._nnz_filter, zip(section_data.columns, row[1:]))))
            elif self._is_landscape_section(section_name):
                for row in section_data.itertuples():
                    landscapes_table["data"].extend(({
                        "expansion": row[0].upper(),
                        "type": self._macro_name(col_name),
                        "amount": amount
                    } for col_name, amount in filter(self._nnz_filter, zip(section_data.columns, row[1:]))))
            else:
                raise KeyError(f"Section name {section_name} has no handling")

    def add_enums(self):
        def add_enum_entry(field_name, namespace, values):
            to_append = {
                "name": ''.join(x for x in field_name.title() if x != '_'),
                "size": len(values),
                "values": values
            }
            if namespace != "":
                to_append["namespace"] = namespace
            self._data["enums"].append(to_append)

        self._data["enums"] = list()
        kingdom_table = self._data["tables"]["kingdom"]
        landscapes_table = self._data["tables"]["landscapes"]

        add_enum_entry("expansion", "", self._uniques_from_ordered_list((kingdom_row["expansion"] for kingdom_row in kingdom_table["data"])))
        for field_name in self._table_enums["kingdom"]:
            add_enum_entry(field_name, "kingdom", self._uniques_from_ordered_list((kingdom_row[field_name] for kingdom_row in kingdom_table["data"])))
        for field_name in self._table_enums["landscapes"]:
            add_enum_entry(field_name, "landscapes", sorted(self._uniques_from_ordered_list((landscapes_row[field_name] for landscapes_row in landscapes_table["data"]))))

    def add_table_sizes(self):
        for table in self._data["tables"].values():
            table["size"] = len(table["data"])

    def add_queries(self):
        def is_match(q: dict, table_elem: dict):
            return all((table_elem[q_key] in q_value for q_key, q_value in q.items()))

        for name in [x for x in self._table_enums.keys() if x != ""]:
            group = self._groups[name]
            attrs = group.keys()
            groups = group.values()
            table = self._data["tables"][name]
            table_data = table["data"]
            table["queries"] = list()
            for t in product(*groups):
                query = dict(zip(attrs, (group["members"] for group in t)))
                if any(map(lambda e: is_match(query, e), table_data)):
                    table["queries"].append({
                        "name": "_".join((group["name"] for group in t)).upper(),
                        "query": query
                    })
            table["num_queries"] = len(table["queries"])

    def add_specials(self):
        self._data["specials"] = dict((ns, {
            "num_specials": len(self._special[ns]),
            "data": self._special[ns],
        }) for ns in (x for x in self._table_enums if x != ""))

    def add_amount_indices(self):
        self._data["amount_indices"] = list({
            "namespace": ns,
            "data": list({"lower": x["name"].lower(), "upper": x["name"]} for x in chain(self._data["tables"][ns]["queries"], self._special[ns]))
        } for ns in (x for x in self._table_enums if x != ""))

    def to_dict(self):
        return self._data
