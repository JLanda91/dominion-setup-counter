from json import load, dump
from pathlib import Path
from os import chdir

from config_io import load_header_templates, render_config_files, load_card_list
from template_vars import TemplateVars


def load_generator_config():
    with open(Path.cwd() / "code_generation_config.json") as f:
        return load(f)


def main():
    chdir(Path.cwd() / "config_generation")
    generator_config = load_generator_config()
    card_list = load_card_list(generator_config)
    templates = load_header_templates()
    template_vars = TemplateVars(card_list, generator_config)
    render_config_files(templates, template_vars.to_dict())
    # chdir(Path.cwd().parent / "config")
    # with open(Path.cwd() / "template_vars.json", "w") as f:
    #     dump(template_vars.to_dict(), f, indent=4)


if __name__ == '__main__':
    main()
