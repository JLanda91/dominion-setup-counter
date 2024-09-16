class BaseGenerator:
    def __init__(self, card_lists, generator_config):
        self._kingdom_regular_df = card_lists["kingdom_regular"]
        self._kingdom_queries_df = card_lists["kingdom_queries"]
        self._kingdom_special_df = card_lists["kingdom_special"]
        self._landscapes_supply_regular_df = card_lists["landscapes_supply_regular"]
        self._landscapes_other_table_df = card_lists["landscapes_other_table"]
        self._landscapes_other_queries_df = card_lists["landscapes_other_queries"]
        self._landscapes_supply_config = generator_config["supply_landscape_groups"]
        self._landscapes_other_types = generator_config["other_landscape_types"]
        self._landscapes_supply_queries_df = card_lists["landscapes_supply_queries"]
        self._landscapes_supply_special_df = card_lists["landscapes_supply_special"]
        self._generator_bounds_tests = generator_config["generator_bounds_tests"]
