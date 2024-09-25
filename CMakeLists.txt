cmake_minimum_required(VERSION 3.27)
project(DominionSetupCounter)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_VERBOSE_MAKEFILE OFF)

find_package(boost_multiprecision CONFIG REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(magic_enum CONFIG REQUIRED)

set(Python_ROOT_DIR "${PROJECT_SOURCE_DIR}")

message(STATUS "${Python_EXECUTABLE}")
message(STATUS "${Python_ROOT_DIR}")
message(STATUS "${CMAKE_CXX_FLAGS}")

add_executable(dominion_setup_counter
        main.cpp
        config.hpp
        utils/tuple.hpp
        utils/constrained_product_generator.hpp
        utils/math.hpp
        utils/enum_mask.hpp
)

add_executable(pim
        pim.cpp
        config.hpp
        utils/tuple.hpp
        utils/constrained_product_generator.hpp
        utils/math.hpp
        utils/enum_mask.hpp
)
target_include_directories(pim PRIVATE utils card_lists/lib)
target_link_libraries(pim PRIVATE card_lists)

include_directories(${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/include)

add_subdirectory(card_lists)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(tst)
endif()

#if(DEFINED ENV{DOMINION_SETUP_COUNTER_STATIC})
#    message(STATUS "Link to std: static")
#    target_link_libraries(dominion_setup_counter -static)
#else()
#    message(STATUS "Link to std: dynamic")
#endif()
