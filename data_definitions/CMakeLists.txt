add_library(data_definitions_big_uint INTERFACE
        big_uint.cuh
)
target_include_directories(data_definitions_big_uint INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)

add_library(data_definitions_big_uint_format INTERFACE
        big_uint_format.cuh
)
target_include_directories(data_definitions_big_uint_format INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)
target_link_libraries(data_definitions_big_uint_format INTERFACE data_definitions_big_uint)

add_library(data_definitions_kingdom INTERFACE
        kingdom.hpp
)
target_include_directories(data_definitions_kingdom INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)

add_library(data_definitions_reduce_type_and_is_action_or_treasure INTERFACE
        reduce_type_and_is_action_or_treasure.cuh
)
target_include_directories(data_definitions_reduce_type_and_is_action_or_treasure INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)

add_library(data_definitions_reduce_mask_and_num_action_or_treasure INTERFACE
        reduce_mask_and_num_action_or_treasure.hpp
)
target_include_directories(data_definitions_reduce_mask_and_num_action_or_treasure INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)


#target_link_libraries(data_definitions INTERFACE fmt::fmt )