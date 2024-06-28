cmake_minimum_required(VERSION 3.27)
project(dominion_start_deck_counter)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_VERBOSE_MAKEFILE ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    include_directories("C:\\Program Files\\Boost\\boost_1_85_0")
endif()

add_executable(dominion_setup_counter main.cpp
        config.hpp
        tuple_utils.hpp
        constrained_product_generator.hpp
        math_utils.hpp
)

if(DEFINED ENV{DOMINION_SETUP_COUNTER_STATIC})
    message(STATUS "Link to std: static")
    target_link_libraries(dominion_setup_counter -static)
else()
    message(STATUS "Link to std: dynamic")
endif()
