from json import load

from pandas import ExcelFile
from pathlib import Path
from itertools import chain


def load_card_list(generator_config):
    def edition_modifier(excel_expansion_name: str):
        match excel_expansion_name:
            case "1ST":
                return "None"
            case "2ND":
                return "Update Pack"
            case "1RC":
                return "Removed"
            case _:
                raise KeyError(f"No edition modifier conversion for {excel_expansion_name}")

    def tracked_type_tuple(tup: tuple, tracked_kingdom_card_types):
        tracked_types = tuple(t for t in tup if len(t) > 0 and t in tracked_kingdom_card_types)
        sorted_tracked_types = tuple(sorted(tracked_types))
        return sorted_tracked_types

    def full_amount_sort(df):
        index_names = df.index.names
        expansion_order = dict((exp, i) for i, exp in enumerate(generator_config["supported_expansions"]))
        edition_order = {'1ST': 0, '2ND': 1, '1RC': 2}
        cost_group_order = dict((cg["name"], i) for i, cg in enumerate(generator_config["cost_groups"]))

        df_reset = df.reset_index()
        df_reset["Expansion sorted"] = df_reset["Expansion"].map(expansion_order)
        df_reset["Edition sorted"] = df_reset["Edition"].map(edition_order)
        df_reset["Cost Group sorted"] = df_reset["Cost Group"].map(cost_group_order)
        sorted_cols = ["Expansion sorted", "Edition sorted", "Cost Group sorted"]
        return df_reset.sort_values(by=sorted_cols).drop(columns=sorted_cols).set_index(index_names)

    def type_cost_sort(df):
        index_names = df.index.names
        cost_group_order = dict((cg["name"], i) for i, cg in enumerate(generator_config["cost_groups"]))
        df_reset = df.reset_index()
        df_reset["Cost Group sorted"] = df_reset["Cost Group"].map(cost_group_order)
        return df_reset.sort_values(by=["Types", "Cost Group sorted"]).drop(columns=["Cost Group sorted"]).set_index(index_names)

    def expansion_sort(df):
        index_names = df.index.names
        expansion_order = dict((exp, i) for i, exp in enumerate(generator_config["supported_expansions"]))
        df_reset = df.reset_index()
        df_reset["Expansion sorted"] = df_reset["Expansion"].map(expansion_order)
        return df_reset.sort_values(by=["Expansion sorted"]).drop(columns=["Expansion sorted"]).set_index(index_names)


    excel_file_name = Path.cwd() / "card_list.xlsx"
    excel_file = ExcelFile(excel_file_name)
    result = {}

    # adding kingdom cards list to the result dictionary.

    # Parse and set nas to empty string
    kingdom_card_list = excel_file.parse(sheet_name="Kingdom", header=0, index_col=None, dtype=str)
    kingdom_card_list.fillna('', inplace=True)
    print("KINGDOM:")
    print("==================")
    print(f"Card list has length {len(kingdom_card_list)}, ", end='')

    # Don't view the rows with expansions that are not supported
    unsupported_expansions = kingdom_card_list.loc[~kingdom_card_list['Expansion'].isin(generator_config["supported_expansions"])]["Expansion"].unique()
    kingdom_card_list = kingdom_card_list.loc[kingdom_card_list['Expansion'].isin(generator_config["supported_expansions"])]
    print(f"of which {len(kingdom_card_list)} are cards from currently supported expansions")
    print(f"Unsupported expansions found in card list: ", 'none' if not unsupported_expansions else ', '.join(unsupported_expansions))

    # Map edition to their string
    kingdom_card_list["Edition"] = kingdom_card_list["Edition"].map(edition_modifier)

    # Map cost strings to their respective cost groups
    kingdom_card_list["Cost"] = kingdom_card_list["Cost"].map(lambda c: next((cg["name"] for cg in generator_config["cost_groups"] if c in cg["strings"])))
    kingdom_card_list.rename(columns={'Cost': 'Cost Group'}, inplace=True)

    # create column 'Types', with sorted tuples containing only the tracked card types. Log the ignored types
    types_columns = list(col for col in kingdom_card_list.columns if col.startswith("Type"))
    kingdom_card_list["Types"] = list(tracked_type_tuple(t, generator_config["tracked_kingdom_card_types"]) for t in zip(*(kingdom_card_list[col] for col in types_columns)))
    untracked_types = set(t for t in chain.from_iterable((kingdom_card_list[col] for col in types_columns)) if len(t) > 0 and t not in generator_config["tracked_kingdom_card_types"])
    print("Untracked card types found in card list:", ", ".join(untracked_types))

    # Delete Type<n> columns
    kingdom_card_list.drop(columns=types_columns, inplace=True)

    # Map special column to bool
    kingdom_card_list["Special"] = kingdom_card_list["Special"].map(lambda x: x.lower() == 'true')

    # Group by (count) the card types with same expansion, edition, cost group and tracked types
    groupby_cols = ["Expansion", "Edition", "Cost Group", "Types"]
    supply_non_special = kingdom_card_list.loc[~kingdom_card_list["Special"]][groupby_cols + ["Name"]].groupby(groupby_cols).count()
    supply_non_special.rename(columns={'Name': 'Count'}, inplace=True)

    # Add to result
    result["kingdom_regular"] = full_amount_sort(supply_non_special)
    result["kingdom_special"] = kingdom_card_list.loc[kingdom_card_list["Special"]][["Name", "Expansion", "Edition"]]
    result["kingdom_queries"] = type_cost_sort(result["kingdom_regular"].groupby(["Types", "Cost Group"]).sum())

    # adding landscape cards list to the result dictionary.
    supported_supply_types = list(chain.from_iterable(landscape_group["strings"] for landscape_group in generator_config["supply_landscape_groups"]))
    supported_other_types = generator_config["other_landscape_types"]
    supported_all_types = supported_supply_types + supported_other_types
    print(f"Landscape supply types are: {', '.join(supported_supply_types)}")
    print(f"""Landscape other types are: {', '.join(supported_other_types)}""")

    # Parse and set nas to empty string
    landscape_card_list = excel_file.parse(sheet_name="Landscapes", header=0, index_col=None, dtype=str)
    landscape_card_list.fillna('', inplace=True)

    # Log if there are any unsupported entries
    unsupported_expansions = landscape_card_list.loc[~landscape_card_list['Expansion'].isin(generator_config["supported_expansions"])]["Expansion"].unique()
    print(f"Unsupported expansions found in landscape card list:", 'none' if not unsupported_expansions else ', '.join(unsupported_expansions))
    unsupported_types = landscape_card_list.loc[~landscape_card_list['Type'].isin(supported_all_types)]["Type"].unique()
    print(f"Unsupported types found in landscape card list:", 'none' if not unsupported_types else ', '.join(unsupported_types))

    # Don't view the rows with expansions that are not supported
    print(f"Landscape card list has length {len(landscape_card_list)}, ", end='')
    landscape_card_list = landscape_card_list.loc[landscape_card_list['Expansion'].isin(generator_config["supported_expansions"]) & landscape_card_list['Type'].isin(supported_all_types)]
    print(f"of which {len(landscape_card_list)} are cards from currently supported expansions and types")

    # Map special column to bool
    landscape_card_list["Special"] = landscape_card_list["Special"].map(lambda x: x.lower() == 'true')

    # Group by (count) the card types with same expansion, edition, cost group and tracked types
    groupby_cols = ["Expansion", "Type"]
    supply_non_special = landscape_card_list.loc[~landscape_card_list["Special"] & landscape_card_list["Type"].isin(supported_supply_types)][groupby_cols + ["Name"]].groupby(groupby_cols).count()
    supply_non_special.rename(columns={'Name': 'Count'}, inplace=True)
    result["landscapes_supply_regular"] = expansion_sort(supply_non_special)
    result["landscapes_supply_special"] = landscape_card_list.loc[landscape_card_list["Special"] & landscape_card_list["Type"].isin(supported_supply_types)][["Name", "Expansion"]]
    result["landscapes_supply_queries"] = result["landscapes_supply_regular"].groupby(["Type"])['Count'].sum().reset_index().set_index('Type')

    other_landscapes = landscape_card_list.loc[landscape_card_list["Type"].isin(generator_config["other_landscape_types"])][groupby_cols + ["Name"]].groupby(groupby_cols).count()
    other_landscapes.rename(columns={'Name': 'Count'}, inplace=True)
    result["landscapes_other_table"] = expansion_sort(other_landscapes)
    result["landscapes_other_queries"] = result["landscapes_other_table"].groupby(["Type"])['Count'].sum().reset_index().set_index('Type')

    for k, df in result.items():
        exported_df = df
        if "Cost Group" in df.index.names:
            index = df.index.names
            df_reset = df.reset_index()
            df_reset["Types"] = df_reset["Types"].map(lambda t: " - ".join(t))
            df_reset.set_index(index, inplace=True)
            exported_df = df_reset
        exported_df.to_json(Path.cwd() / "output_tables" / f"{k}.json", orient='table', indent=4)

    return result


def load_code_generator_config():
    with open(Path.cwd() / "code_generator_config.json") as f:
        return load(f)


generator_config = load_code_generator_config()
card_list = load_card_list(generator_config)
