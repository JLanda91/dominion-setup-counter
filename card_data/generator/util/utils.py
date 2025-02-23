_underscore_trans = str.maketrans({' ': '_'})
_remove_space_trans = str.maketrans({' ': ''})


def lower_snake_case(input_str: str):
    return input_str.lower().translate(_underscore_trans)

def pascal_case(input_str: str):
    return input_str.translate(_remove_space_trans)
