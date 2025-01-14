cmake_minimum_required(VERSION 3.27)
project(DominionSetupCounter)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_VERBOSE_MAKEFILE OFF)

find_package(boost_multiprecision CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(Python_ROOT_DIR "${PROJECT_SOURCE_DIR}")

message(STATUS "${Python_EXECUTABLE}")
message(STATUS "${Python_ROOT_DIR}")
message(STATUS "${CMAKE_CXX_FLAGS}")

add_executable(main
        main.cpp
)

target_link_libraries(main PRIVATE card_data fmt::fmt)

add_executable(pim
        pim.cpp
        config.hpp
        utils/tuple.hpp
        utils/constrained_product_generator.hpp
        utils/math.hpp
)

target_link_libraries(pim PRIVATE card_data)

include_directories(${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/include)

add_subdirectory(card_data)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(tst)
endif()

#if(DEFINED ENV{DOMINION_SETUP_COUNTER_STATIC})
#    message(STATUS "Link to std: static")
#    target_link_libraries(dominion_setup_counter -static)
#else()
#    message(STATUS "Link to std: dynamic")
#endif()
