set(CMAKE_VERBOSE_MAKEFILE ON)

add_library(data_equivalence_classes
        equivalence_classes.cpp
        equivalence_classes.hpp
        array_type.hpp
)
target_include_directories(data_equivalence_classes INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)
target_link_libraries(data_equivalence_classes PRIVATE data_definitions_kingdom)

add_library(data_type_at_reduction_pairs
        type_at_reduction_pairs.cu
        type_at_reduction_pairs.cuh
        array_type.hpp
)
target_include_directories(data_type_at_reduction_pairs INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)
target_link_libraries(data_type_at_reduction_pairs PRIVATE data_definitions_reduce_type_and_is_action_or_treasure)

add_library(data_mask_num_at_reduction
        mask_num_at_reduction.cpp
        mask_num_at_reduction.hpp
        array_type.hpp
)
target_include_directories(data_mask_num_at_reduction INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)
target_link_libraries(data_mask_num_at_reduction PRIVATE data_definitions_reduce_mask_and_num_action_or_treasure zstd::libzstd)
target_compile_definitions(data_mask_num_at_reduction PUBLIC MASK_NUM_AT_DATA_FILE="${CMAKE_CURRENT_SOURCE_DIR}/mask_num_at_reduction.zstd")