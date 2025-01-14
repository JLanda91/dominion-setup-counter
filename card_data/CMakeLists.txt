set(OUTPUT_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/card_data")
set(GENERATOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generator")
set(GENERATOR_TEMPLATES_DIR "${GENERATOR_DIR}/templates")

add_custom_command(
        COMMAND
        "cogapp"
        ARGS
        "-d"
        "@generated_files.txt"
        WORKING_DIRECTORY
        "${GENERATOR_DIR}"
        DEPENDS
        "${GENERATOR_DIR}/generated_files.txt"
        "${GENERATOR_DIR}/kingdom_venn_region_data.py"
        "${GENERATOR_DIR}/card_list.xlsx"
        "${GENERATOR_DIR}/config.json"
        "${GENERATOR_DIR}/config.py"
        "${GENERATOR_DIR}/util/utils.py"

        "${GENERATOR_TEMPLATES_DIR}/kingdom/card_type_tally.hpp"
        "${GENERATOR_TEMPLATES_DIR}/kingdom/membership_mask.hpp"
        "${GENERATOR_TEMPLATES_DIR}/kingdom/venn_region.hpp"

        "${GENERATOR_DIR}/kingdom/card_type_tally.py"
        "${GENERATOR_DIR}/kingdom/membership_mask.py"
        "${GENERATOR_DIR}/kingdom/venn_region.py"

        OUTPUT
        "${OUTPUT_LIB_DIR}/kingdom/card_type_tally.hpp"
        "${OUTPUT_LIB_DIR}/kingdom/membership_mask.hpp"
        "${OUTPUT_LIB_DIR}/kingdom/venn_region.hpp"
        VERBATIM
        USES_TERMINAL
)

add_library(card_data INTERFACE
        card_data/kingdom/card_type_tally.hpp
        card_data/kingdom/membership_mask.hpp
        card_data/kingdom/venn_region.hpp
)

target_include_directories(card_data INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
