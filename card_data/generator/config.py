from typing import List
from pathlib import Path
from json import load

from generator.log import code_generator_log


class StringGroup:
    def __init__(self, name: str, strings: List[str]):
        self._name = name
        self._strings = strings

    @property
    def name(self) -> str:
        return self._name

    @property
    def strings(self) -> List[str]:
        return self._strings


class RowPredicate:
    def __init__(self, name: str, predicate: str):
        self._name = name
        self._predicate = eval(predicate)

    @property
    def name(self) -> str:
        return self._name

    def __call__(self, row) -> bool:
        return self._predicate(row)



class CodeGeneratorConfig:
    _num_objects = 5

    def __init__(self, parsed_dict: dict):
        code_generator_log.info("Loading code generator configuration...")
        self._supported_expansions = parsed_dict["supported_expansions"]
        self._tracked_kingdom_card_types = parsed_dict["tracked_kingdom_card_types"]
        self._supply_lanscape_groups = [StringGroup(**obj) for obj in parsed_dict["supply_landscape_groups"]]
        self._supply_kingdom_card_type_predicates = [RowPredicate(**obj) for obj in parsed_dict["supply_kingdom_card_type_predicates"]]
        self._supply_kingdom_venn_predicates = [RowPredicate(**obj) for obj in parsed_dict["supply_kingdom_venn_predicates"]]
        self._other_landscape_types = parsed_dict["other_landscape_types"]

        num_objects_passed = len(parsed_dict)
        num_members = len(self.__dict__)
        if num_objects_passed != num_members:
            raise ValueError(f"Expected a config json with {num_members} objects, but {num_objects_passed} were found. Did you forget to set a member here?")

        code_generator_log.info(f"""\tSupported expansions: {', '.join(self._supported_expansions)}""")
        code_generator_log.info(f"""\tTracking card types: {', '.join(self._tracked_kingdom_card_types)}""")
        code_generator_log.info(f"""\tSupply kingdom Venn region predicates: {', '.join((f"{x.name}" for x in self._supply_kingdom_venn_predicates))}""")
        code_generator_log.info(f"""\tSupply kingdom card type predicates: {', '.join((f"{x.name}" for x in self.supply_kingdom_card_type_predicates))}""")
        code_generator_log.info(f"""\tSupply landscape card groups: {', '.join((f"{x.name}" for x in self._supply_lanscape_groups))}""")
        code_generator_log.info(f"""\tOther landscape types: {', '.join((f"{x}" for x in self._other_landscape_types))}\n""")

    def supply_kingdom_card_type(self, row):
        return next(p.name for p in self._supply_kingdom_card_type_predicates if p(row))

    def supply_kingdom_venn_mask(self, row) -> int:
        return sum((p(row) << i) for i, p in enumerate(self._supply_kingdom_venn_predicates))

    @property
    def supported_expansions(self):
        return self._supported_expansions

    @property
    def tracked_kingdom_card_types(self) -> List[str]:
        return self._tracked_kingdom_card_types

    @property
    def supply_landscape_groups(self) -> List[StringGroup]:
        return self._supply_lanscape_groups

    @property
    def supply_kingdom_card_type_predicates(self) -> List[RowPredicate]:
        return self._supply_kingdom_card_type_predicates

    @property
    def supply_kingdom_venn_predicates(self) -> List[RowPredicate]:
        return self._supply_kingdom_venn_predicates

    @property
    def other_landscape_types(self) -> List[str]:
        return self._other_landscape_types

    @classmethod
    def from_file(cls, file_path: Path):
        if file_path.is_file():
            with open(file_path, "r") as f:
                return cls(load(f))
        else:
            raise ValueError(f"Cannot open {file_path}: Does not exist or not a file")
