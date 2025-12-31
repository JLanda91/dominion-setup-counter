add_library(card_data_headers INTERFACE
        extra_setup/card.cuh
        extra_setup/effective_card_type_mask.cuh
#        extra_setup/state.hpp
        kingdom/venn_diagram_region_mask.cuh
        ../data_definitions/kingdom.hpp
        functional.cuh
)
target_include_directories(card_data_headers INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)

add_library(card_data_format_host
        format.cu
)
target_compile_options(card_data_format_host PRIVATE "-x c++")
target_include_directories(card_data_format_host INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)
target_link_libraries(card_data_format_host PRIVATE card_data_headers fmt::fmt)

add_library(card_data_functional_host
        functional.cu
)
target_compile_options(card_data_functional_host PRIVATE "-x c++")
target_include_directories(card_data_functional_host INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)
target_link_libraries(card_data_functional_host PRIVATE card_data_headers)


