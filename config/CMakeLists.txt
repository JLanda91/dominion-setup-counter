add_custom_command(
    OUTPUT
        "${PROJECT_SOURCE_DIR}/config/card_amount_tables.hpp"
        "${PROJECT_SOURCE_DIR}/config/generator_bounds.hpp"
        "${PROJECT_SOURCE_DIR}/config/default_expansion_editions.hpp"
    BYPRODUCTS
        "${PROJECT_SOURCE_DIR}/config/template_vars_old.json"
    COMMAND
        "${Python_EXECUTABLE}"
    ARGS
        "${PROJECT_SOURCE_DIR}/config_generation/generate_config_old.py"
        "${PROJECT_SOURCE_DIR}"
    DEPENDS
        "${PROJECT_SOURCE_DIR}/config_generation/*.py"
        "${PROJECT_SOURCE_DIR}/config_generation/*.xlsx"
        "${PROJECT_SOURCE_DIR}/config_generation/templates/*.mustache"
    VERBATIM
    USES_TERMINAL
)

#add_custom_target(generated_config
#    DEPENDS
#        "${PROJECT_SOURCE_DIR}/config/card_amount_tables.hpp"
#        "${PROJECT_SOURCE_DIR}/config/generator_bounds.hpp"
#        "${PROJECT_SOURCE_DIR}/config/default_expansion_editions.hpp"
#)

add_library(config
        generator_bounds.cpp
        generator_bounds.hpp
        typedefs.hpp
        amount_getters.cpp
        amount_getters.hpp
)