add_library(utils INTERFACE
        format.hpp
        math.hpp
        sized_enum.hpp
        result_type.hpp
)

target_include_directories(utils INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
)

target_link_libraries(utils INTERFACE
        fmt::fmt-header-only
)