from os import chdir
from pathlib import Path
from argparse import ArgumentParser

from config_io_old import load_excel_config, load_header_templates, render_config_files
from template_vars_old import TemplateVars


def main():
    arg_parser = ArgumentParser(
        prog='ConfigGenerator',
        description='Generates the C++ headers with the card amount tables'
    )
    arg_parser.add_argument('project_dir', type=str)
    args = arg_parser.parse_args()
    chdir(args.project_dir)

    print("Generating config from excel...")
    chdir(Path.cwd() / "config_generation")
    excel_config = load_excel_config()
    templates = load_header_templates()
    template_vars = TemplateVars(excel_config).to_dict()
    render_config_files(templates, template_vars)
    chdir(Path.cwd().parent)


if __name__ == "__main__":
    main()
