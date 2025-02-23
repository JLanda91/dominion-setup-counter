add_library(utils INTERFACE
        constrained_product_generator.hpp
        format.hpp
        math.hpp
        sized_enum.hpp
        filtered_index_sequence.hpp
        column_major_table.hpp
        result_type.hpp
)

target_include_directories(utils INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
)

target_link_libraries(utils INTERFACE
        fmt::fmt-header-only
)