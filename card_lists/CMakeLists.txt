#SET(CARD_LISTS_DIR "${PROJECT_SOURCE_DIR}/card_lists/")
#SET(CARD_LISTS_LIBRARY_DIR "${CARD_LISTS_DIR}/lib")
#SET(CARD_LISTS_GENERATOR_DIR "${CARD_LISTS_DIR}/generator")
#SET(CARD_LISTS_GENERATOR_TEMPLATE_DIR "${CARD_LISTS_GENERATOR_DIR}/templates")
#SET(TEST_DIR "${CARD_LISTS_DIR}/../tst")
#
#MESSAGE(STATUS "card list dir = ${CARD_LISTS_DIR}")
#MESSAGE(STATUS "card list library dir = ${CARD_LISTS_LIBRARY_DIR}")
#MESSAGE(STATUS "card list generator dir = ${CARD_LISTS_GENERATOR_DIR}")
#MESSAGE(STATUS "card list generator template dir = ${CARD_LISTS_GENERATOR_TEMPLATE_DIR}")
#MESSAGE(STATUS "project test dir = ${TEST_DIR}")

add_subdirectory(generator)

set(CMAKE_VERBOSE_MAKEFILE on)

add_custom_command(
        COMMAND
        "cogapp"
        ARGS
        "-d"
        "@generated_files.txt"
        WORKING_DIRECTORY
        "${PROJECT_SOURCE_DIR}/card_lists/generator"
        DEPENDS
        "${PROJECT_SOURCE_DIR}/card_lists/generator/templates/amount_getters.cpp"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/templates/amount_getters.hpp"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/templates/card_amount_tables.hpp"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/templates/component.card_lists.generator_bounds.cpp"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/card_list.xlsx"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/code_generator_config.json"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/generated_files.txt"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/config_io.py"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/generator_amount_getter.py"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/generator_base.py"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/generator_card_amount_tables.py"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/generator_component_card_lists_generator_bounds.py"
        "${PROJECT_SOURCE_DIR}/card_lists/generator/generator_utils.py"
        OUTPUT
        "${PROJECT_SOURCE_DIR}/card_lists/lib/card_amount_tables.hpp"
        "${PROJECT_SOURCE_DIR}/card_lists/lib/amount_getters.hpp"
        "${PROJECT_SOURCE_DIR}/card_lists/lib/amount_getters.cpp"
        "${PROJECT_SOURCE_DIR}/tst/component.card_lists.generator_bounds.cpp"
        VERBATIM
        USES_TERMINAL
)

add_library(card_lists
        lib/amount_getters.cpp
        lib/amount_getters.hpp
        lib/default_expansion_editions.hpp
        lib/generator_bounds.cpp
        lib/generator_bounds.hpp
        lib/typedefs.hpp
)

target_include_directories(card_lists PRIVATE "../utils")