import cog


_underscore_trans = str.maketrans({' ': '_'})


def upper_snake_case(input_str: str):
    return input_str.upper().translate(_underscore_trans)


def lower_snake_case(input_str: str):
    return input_str.lower().translate(_underscore_trans)


def full_query_name(tracked_types, cost_group):
    return ('none' if len(tracked_types) == 0 else '_'.join(tracked_types)) + f"_{cost_group}"


def create_enum_class(name: str, values, underlying_type=None, create_sized_enum=True):
    cog.outl(f"""enum class {name} {f": {underlying_type}" if underlying_type is not None else ""} {{""")
    for x in values[:-1]:
        cog.outl(f"\t{upper_snake_case(x)},")
    cog.outl(f"\t{upper_snake_case(values[-1])}")
    cog.outl("""};
    """)
    if create_sized_enum:
        cog.outl(f"""using Sized{name} = utils::enums::SizedEnum<{name}, {len(values)}uz>;
        """)


def create_singleton(name: str, array_element_type: str, size: int, iterable, tuple_printer):
    cog.outl(f"""using {name}_t = std::array<{array_element_type}, {size}uz>;
    """)
    cog.outl(f"""static constexpr auto {name}() noexcept -> const {name}_t& {{
    static constexpr {name}_t singleton {{{{""")
    for t in iterable:
        tuple_printer(t)
    cog.outl("\t}};")
    cog.outl("\treturn singleton;")
    cog.outl("""}
    """)
