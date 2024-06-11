cmake_minimum_required(VERSION 3.27)
project(dominion_start_deck_counter)

set(CMAKE_CXX_STANDARD 23)

add_executable(dominion_start_deck_counter main.cpp
        config.hpp
        tuple_utils.hpp
        constrained_product_generator.hpp
        math_utils.hpp
)
