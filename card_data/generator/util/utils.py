_underscore_trans = str.maketrans({' ': '_'})


def lower_snake_case(input_str: str):
    return input_str.lower().translate(_underscore_trans)
