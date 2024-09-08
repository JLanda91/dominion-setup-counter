#add_custom_command(
#    OUTPUT
#        "${PROJECT_SOURCE_DIR}/config/card_amount_tables.hpp"
#        "${PROJECT_SOURCE_DIR}/config/generator_bounds.hpp"
#        "${PROJECT_SOURCE_DIR}/config/default_expansion_editions.hpp"
#    BYPRODUCTS
#        "${PROJECT_SOURCE_DIR}/config/template_vars.json"
#    COMMAND
#        "${Python_EXECUTABLE}"
#    ARGS
#        "${PROJECT_SOURCE_DIR}/config_generation/generate_config_old.py"
#        "${PROJECT_SOURCE_DIR}"
#    DEPENDS
#        "${PROJECT_SOURCE_DIR}/config_generation/*.py"
#        "${PROJECT_SOURCE_DIR}/config_generation/*.xlsx"
#        "${PROJECT_SOURCE_DIR}/config_generation/templates/*.mustache"
#    VERBATIM
#    USES_TERMINAL
#)

SET(CONFIG_OUTPUT_FOLDER "${PROJECT_SOURCE_DIR}/config")
SET(CONFIG_GENERATION_FOLDER "${CONFIG_OUTPUT_FOLDER}/generator")
SET(CONFIG_GENERATION_TEMPLATE_FOLDER "${CONFIG_GENERATION_FOLDER}/templates")

add_custom_command(
        COMMAND
        "cogapp"
        ARGS
        "-d"
        "@generated_files.txt"
        WORKING_DIRECTORY
        "${CONFIG_GENERATION_FOLDER}"
        DEPENDS
        "${CONFIG_GENERATION_TEMPLATE_FOLDER}/card_amount_tables.hpp"
        OUTPUT
        "${CONFIG_OUTPUT_FOLDER}/card_amount_tables.hpp"
        VERBATIM
        USES_TERMINAL
)

add_library(config
        generator_bounds.cpp
        generator_bounds.hpp
        typedefs.hpp
        amount_getters.cpp
        amount_getters.hpp
        card_amount_tables.save.hpp
)

target_include_directories(config PRIVATE "${PROJECT_SOURCE_DIR}/utils")