from pathlib import Path

from pandas import ExcelFile, DataFrame
from itertools import chain

from generator.config import CodeGeneratorConfig
from generator.log import code_generator_log


class KingdomVennRegionData:
    def __init__(self, df: DataFrame, code_generator_config: CodeGeneratorConfig):
        code_generator_log.info(f"Loading and transforming kingdom card data ({len(df)} entries)")
        df.fillna('', inplace=True)
        df.drop(columns=["Special"], inplace=True)

        # Don't view the rows with expansions that are not supported
        unsupported_expansions = df.loc[~df['Expansion'].isin(code_generator_config.supported_expansions)]["Expansion"].unique().tolist()
        df = df.loc[df['Expansion'].isin(code_generator_config.supported_expansions)]
        df.drop(columns=["Expansion", "Edition"], inplace=True)
        code_generator_log.info(f"\tOf which {len(df)} are cards from currently supported expansions")
        code_generator_log.info(f"\tUnsupported expansions found in card list: {'none' if not unsupported_expansions else ', '.join(unsupported_expansions)}")

        # Create column 'Types', with sorted tuples containing only the tracked card types. Log the ignored types
        types_columns = list(col for col in df.columns if col.startswith("Type"))
        df["Types"] = list(KingdomVennRegionData._tracked_type_tuple(t, code_generator_config.tracked_kingdom_card_types) for t in zip(*(df[col] for col in types_columns)))
        untracked_types = set(t for t in chain.from_iterable((df[col] for col in types_columns)) if len(t) > 0 and t not in code_generator_config.tracked_kingdom_card_types)
        code_generator_log.info(f"\tUntracked card types in card list: {", ".join(untracked_types)}")

        # Delete Type<n> columns
        df.drop(columns=types_columns, inplace=True)

        # Create columns 'Venn Mask' and card type
        df["Venn Mask"] = df.apply(code_generator_config.supply_kingdom_venn_mask, axis='columns')
        df["Card Type"] = df.apply(code_generator_config.supply_kingdom_card_type, axis='columns')

        group_cols = ["Venn Mask", "Card Type"]
        group_df = df[group_cols + ["Name"]].groupby(group_cols).count()
        group_df.rename(columns={'Name': 'Count'}, inplace=True)
        group_df.reset_index(inplace=True)

        self._data = group_df
        self._code_generator_config = code_generator_config

    def __repr__(self):
        return repr(self._data)

    @property
    def data(self):
        return self._data

    @property
    def code_generator_config(self):
        return self._code_generator_config

    @staticmethod
    def _tracked_type_tuple(tup: tuple, tracked_kingdom_card_types):
        tracked_types = tuple(t for t in tup if len(t) > 0 and t in tracked_kingdom_card_types)
        sorted_tracked_types = tuple(sorted(tracked_types))
        return sorted_tracked_types

    @classmethod
    def from_file(cls, excel_file_path: Path, code_generator_config_file_path: Path):
        if excel_file_path.is_file() and code_generator_config_file_path.is_file():
            return KingdomVennRegionData(
                ExcelFile(excel_file_path).parse(sheet_name="Kingdom", header=0, index_col=None, dtype=str),
                CodeGeneratorConfig.from_file(code_generator_config_file_path))
        else:
            raise ValueError(f"Either {excel_file_path} or {code_generator_config_file_path} does not exist or is not a file")

instance = KingdomVennRegionData.from_file(
    Path.cwd() / "card_list.xlsx",
    Path.cwd() / "config.json")