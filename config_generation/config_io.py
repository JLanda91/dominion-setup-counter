from json import dump
from os import chdir
from pandas import ExcelFile, Index
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


    excel_file_name = Path.cwd() / "card_list.xlsx"
    excel_file = ExcelFile(excel_file_name)
    result = {}

    # adding kingdom cards list to the result dictionary.

    # Parse and set nas to empty string
    sheet_data = excel_file.parse(sheet_name="Kingdom", header=0, index_col=None, dtype=str)
    sheet_data.fillna('', inplace=True)
    print(f"Card list has length {len(sheet_data)}, ", end='')

    # Don't view the rows with expansions that are not supported
    trans_table = str.maketrans({' ': '_'})
    unsupported_expansions = sheet_data.loc[~sheet_data['Expansion'].str.upper().str.translate(trans_table).isin(generator_config["supported_expansions"])]["Expansion"].unique()
    sheet_data = sheet_data.loc[sheet_data['Expansion'].str.upper().str.translate(trans_table).isin(generator_config["supported_expansions"])]
    print(f"of which {len(sheet_data)} are cards from currently supported expansions")
    print(f"Unsupported expansions found in card list: ", *unsupported_expansions)

    # Map cost strings to their respective cost groups
    sheet_data["Cost"] = sheet_data["Cost"].map(lambda c: next((cg["name"] for cg in generator_config["cost_groups"] if c in cg["strings"])))
    sheet_data.rename(columns={'Cost': 'Cost Group'}, inplace=True)

    # Map Expansion to their macro name
    sheet_data["Expansion"] = sheet_data["Expansion"].map(lambda expansion : expansion.upper().translate(str.maketrans({' ': '_'})))

    # create column 'Types', with sorted tuples containing only the tracked card types. Log the ignored types
    types_columns = list(col for col in sheet_data.columns if col.startswith("Type"))
    sheet_data["Types"] = list(tracked_type_tuple(t, generator_config["tracked_kingdom_card_types"]) for t in zip(*(sheet_data[col] for col in types_columns)))
    untracked_types = set(t for t in chain.from_iterable((sheet_data[col] for col in types_columns)) if len(t) > 0 and t.upper() not in generator_config["tracked_kingdom_card_types"])
    print("Untracked kingdom card types:", ", ".join(untracked_types))

    # Delete Type<n> columns
    sheet_data.drop(columns=types_columns, inplace=True)

    # Map special column to bool and map cost strings to the configurated cost groups
    sheet_data["Special"] = list(map(lambda x: x.lower() == 'true', sheet_data["Special"]))

    # Group by (count) the card types with same expansion, edition, cost group and tracked types
    groupby_cols = ["Expansion", "Edition", "Cost Group", "Types"]
    non_special_counts = sheet_data.loc[~sheet_data["Special"]][groupby_cols + ["Name"]].groupby(groupby_cols).count()
    non_special_counts.rename(columns={'Name': 'Count'}, inplace=True)

    # Add to result
    result["kingdom_regular"] = full_amount_sort(non_special_counts)
    result["kingdom_special"] = sheet_data.loc[sheet_data["Special"]][["Name", "Expansion", "Edition"]]
    result["kingdom_queries"] = type_cost_sort(result["kingdom_regular"].groupby(["Types", "Cost Group"]).sum())

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
