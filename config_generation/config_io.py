from json import dump
from os import chdir
from pandas import ExcelFile
from pathlib import Path
from pystache import render


def load_excel_config():
    excel_file_name = Path.cwd() / "card_amounts.xlsx"
    excel_file = ExcelFile(excel_file_name)
    result = {}
    for sheet_name in excel_file.sheet_names:
        match sheet_name.lower():
            case "regular":
                header = [0, 1]
            case "looter" | "fate" | "doom" | "liaison":
                header = 0
            case _:
                raise KeyError(f"No handling for sheet {sheet_name}")
        sheet_data = excel_file.parse(sheet_name=sheet_name, header=header, index_col=0, dtype=str)
        sheet_data.fillna('0', inplace=True)
        sheet_data = sheet_data.astype('int64')
        result[sheet_name] = sheet_data
    return result


template_names = ("card_amount_tables.hpp", "default_expansion_editions.hpp", "amount_getters.hpp", "amount_getters.cpp")


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
