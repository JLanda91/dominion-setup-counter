from json import dump
from os import chdir
from pandas import ExcelFile, Index, DataFrame, Series
from pathlib import Path
from pystache import render
from itertools import chain


def load_card_list(generator_config):
    def tracked_type_tuple(tup: tuple, tracked_kingdom_card_types):
        tracked_types = tuple(t.upper() for t in tup if len(t) > 0 and t.upper() in tracked_kingdom_card_types)
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
    print(f"Kingdom card list has length {len(kingdom_card_list)}, ", end='')

    # Don't view the rows with expansions that are not supported
    macro_trans = str.maketrans({' ': '_'})
    unsupported_expansions = kingdom_card_list.loc[~kingdom_card_list['Expansion'].str.upper().str.translate(macro_trans).isin(generator_config["supported_expansions"])]["Expansion"].unique()
    kingdom_card_list = kingdom_card_list.loc[kingdom_card_list['Expansion'].str.upper().str.translate(macro_trans).isin(generator_config["supported_expansions"])]
    print(f"of which {len(kingdom_card_list)} are cards from currently supported expansions")
    print(f"Unsupported expansions found in kingdom card list: ", 'none' if not unsupported_expansions else ', '.join(unsupported_expansions))

    # Map cost strings to their respective cost groups
    kingdom_card_list["Cost"] = kingdom_card_list["Cost"].map(lambda c: next((cg["name"] for cg in generator_config["cost_groups"] if c in cg["strings"])))
    kingdom_card_list.rename(columns={'Cost': 'Cost Group'}, inplace=True)

    # Map Expansion to their macro name
    kingdom_card_list["Expansion"] = kingdom_card_list["Expansion"].map(lambda expansion : expansion.upper().translate(macro_trans))

    # create column 'Types', with sorted tuples containing only the tracked card types. Log the ignored types
    types_columns = list(col for col in kingdom_card_list.columns if col.startswith("Type"))
    kingdom_card_list["Types"] = list(tracked_type_tuple(t, generator_config["tracked_kingdom_card_types"]) for t in zip(*(kingdom_card_list[col] for col in types_columns)))
    untracked_types = set(t for t in chain.from_iterable((kingdom_card_list[col] for col in types_columns)) if len(t) > 0 and t.upper() not in generator_config["tracked_kingdom_card_types"])
    print("Untracked kingdom card types found in card list:", ", ".join(untracked_types))

    # Delete Type<n> columns
    kingdom_card_list.drop(columns=types_columns, inplace=True)

    # Map special column to bool
    kingdom_card_list["Special"] = kingdom_card_list["Special"].map(lambda x: x.lower() == 'true')

    # Group by (count) the card types with same expansion, edition, cost group and tracked types
    groupby_cols = ["Expansion", "Edition", "Cost Group", "Types"]
    non_special_counts = kingdom_card_list.loc[~kingdom_card_list["Special"]][groupby_cols + ["Name"]].groupby(groupby_cols).count()
    non_special_counts.rename(columns={'Name': 'Count'}, inplace=True)

    # Add to result
    result["kingdom_regular"] = full_amount_sort(non_special_counts)
    result["kingdom_special"] = kingdom_card_list.loc[kingdom_card_list["Special"]][["Name", "Expansion", "Edition"]]
    result["kingdom_queries"] = type_cost_sort(result["kingdom_regular"].groupby(["Types", "Cost Group"]).sum())

    # adding landscape cards list to the result dictionary.

    # Parse and set nas to empty string
    landscape_card_list = excel_file.parse(sheet_name="Landscapes", header=0, index_col=None, dtype=str)
    landscape_card_list.fillna('', inplace=True)
    print(f"Landscape card list has length {len(landscape_card_list)}, ", end='')

    # Don't view the rows with expansions that are not supported
    unsupported_expansions = landscape_card_list.loc[~landscape_card_list['Expansion'].str.upper().str.translate(macro_trans).isin(generator_config["supported_expansions"])]["Expansion"].unique()
    landscape_card_list = landscape_card_list.loc[landscape_card_list['Expansion'].str.upper().str.translate(macro_trans).isin(generator_config["supported_expansions"])]
    print(f"of which {len(landscape_card_list)} are cards from currently supported expansions")
    print(f"Unsupported expansions found in landscape card list:", 'none' if not unsupported_expansions else ', '.join(unsupported_expansions))

    # Map Expansion and Type to their macro name
    landscape_card_list["Expansion"] = landscape_card_list["Expansion"].map(lambda expansion : expansion.upper().translate(macro_trans))
    landscape_card_list["Type"] = landscape_card_list["Type"].map(lambda t : t.upper().translate(macro_trans))

    # Map special column to bool
    landscape_card_list["Special"] = landscape_card_list["Special"].map(lambda x: x.lower() == 'true')

    # Group by (count) the card types with same expansion, edition, cost group and tracked types
    groupby_cols = ["Expansion", "Type"]
    non_special_counts = landscape_card_list.loc[~landscape_card_list["Special"]][groupby_cols + ["Name"]].groupby(groupby_cols).count()
    non_special_counts.rename(columns={'Name': 'Count'}, inplace=True)
    result["landscapes_regular"] = expansion_sort(non_special_counts)
    result["landscapes_special"] = landscape_card_list.loc[landscape_card_list["Special"]][["Name", "Expansion"]]
    result["landscapes_types"] = Series(result["landscapes_regular"].index.get_level_values("Type").unique())

    for k, df in result.items():
        exported_df = df
        if "Cost Group" in df.index.names:
            index = df.index.names
            df_reset = df.reset_index()
            df_reset["Types"] = df_reset["Types"].map(lambda t: " - ".join(t))
            df_reset.set_index(index, inplace=True)
            exported_df = df_reset
        exported_df.to_excel(Path.cwd().parent / "config" / f"{k}.xlsx", index=(len(exported_df.index.names) > 1))

    return result


template_names = (
    "card_amount_tables.hpp",
    "default_expansion_editions.hpp",
    "amount_getters.hpp",
    "amount_getters.cpp",
)


def load_header_templates():
    return dict((template_name, open(Path.cwd() / "templates" / f"{template_name}.mustache").read()) for template_name in template_names)


def render_config_files(template, template_vars):
    chdir(Path.cwd() / "templates")
    for template_name in template_names:
        with open(Path.cwd().parent.parent / "config" / f"{template_name}", "w") as f:
            f.write(render(template[template_name], template_vars))
    chdir(Path.cwd().parent.parent / "config")
    with open("template_vars.json", "w") as f:
        dump(template_vars, f, indent=4)
    chdir(Path.cwd().parent)
