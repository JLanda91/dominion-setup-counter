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
add_subdirectory(kernels)

add_executable(main main.cu)
set_target_properties(main PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON
)
target_link_libraries(main PRIVATE
        fmt::fmt
        sparkplug::sparkplug
        aggregate_by_type_mask_and_num_action_or_treasure
        data_mask_num_at_reduction
        data_definitions_big_uint_format)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    add_subdirectory(test)
endif()
