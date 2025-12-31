cmake_minimum_required(VERSION 3.27)
project(DominionSetupCounter CXX CUDA)

find_package(fmt CONFIG REQUIRED)
find_package(zstd CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(sparkplug REQUIRED)

set(Python_ROOT_DIR "${PROJECT_SOURCE_DIR}")

message(STATUS "${Python_EXECUTABLE}")
message(STATUS "${Python_ROOT_DIR}")
message(STATUS "${CMAKE_CXX_FLAGS}")

#add_subdirectory(card_data)
add_subdirectory(data)
add_subdirectory(data_definitions)

add_executable(main main.cu)
set_target_properties(main PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON
)
target_link_libraries(main PRIVATE
        fmt::fmt
        sparkplug::sparkplug
        data_equivalence_classes
        data_type_at_reduction_pairs)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    add_subdirectory(test)
endif()
