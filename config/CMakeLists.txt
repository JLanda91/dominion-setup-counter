SET(CONFIG_DIR "${PROJECT_SOURCE_DIR}/config")
SET(CONFIG_LIBRARY_DIR "${CONFIG_DIR}/lib")
SET(CONFIG_GENERATOR_DIR "${CONFIG_DIR}/generator")
SET(CONFIG_GENERATOR_TEMPLATE_DIR "${CONFIG_GENERATOR_DIR}/templates")
SET(PROJECT_TEST_DIR "${PROJECT_SOURCE_DIR}/tst")

add_custom_command(
        COMMAND
        "cogapp"
        ARGS
        "-d"
        "@generated_files.txt"
        WORKING_DIRECTORY
        "${CONFIG_GENERATOR_DIR}"
        DEPENDS
        "${CONFIG_GENERATOR_TEMPLATE_DIR}/*"
        "${CONFIG_GENERATOR_DIR}/*.py"
        "${CONFIG_GENERATOR_DIR}/card_list.xlsx"
        "${CONFIG_GENERATOR_DIR}/code_generator_config.json"
        OUTPUT
        "${CONFIG_LIBRARY_DIR}/card_amount_tables.hpp"
        "${CONFIG_LIBRARY_DIR}/amount_getters.hpp"
        "${CONFIG_LIBRARY_DIR}/amount_getters.cpp"
        "${PROJECT_TEST_DIR}/component.config.generator_bounds.cpp"
        VERBATIM
        USES_TERMINAL
)

add_library(config
        lib/amount_getters.cpp
        lib/amount_getters.hpp
        lib/card_amount_tables.hpp
        lib/default_expansion_editions.hpp
        lib/generator_bounds.cpp
        lib/generator_bounds.hpp
        lib/typedefs.hpp
)

target_include_directories(config PRIVATE "${PROJECT_SOURCE_DIR}/utils")