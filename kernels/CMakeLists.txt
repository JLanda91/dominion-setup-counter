add_library(base_launcher
        base_stream_launcher.cu
        base_stream_launcher.cuh
)
target_include_directories(base_launcher INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)
target_link_libraries(base_launcher PRIVATE sparkplug::sparkplug)

add_library(constant_memory
        constant_memory.cu
        constant_memory.cuh
)
target_include_directories(constant_memory INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)
target_link_libraries(constant_memory PRIVATE
        sparkplug::sparkplug
        data_equivalence_classes
        data_type_at_reduction_pairs
)

add_library(aggregate_by_type_mask_and_num_action_or_treasure
        aggregate_by_type_mask_and_num_action_or_treasure.cu
        aggregate_by_type_mask_and_num_action_or_treasure.cuh
)
target_include_directories(aggregate_by_type_mask_and_num_action_or_treasure INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)
target_link_libraries(aggregate_by_type_mask_and_num_action_or_treasure PRIVATE
        base_launcher
        constant_memory
        data_definitions_big_uint
        fmt::fmt
)
