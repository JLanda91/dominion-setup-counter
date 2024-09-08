cmake_minimum_required(VERSION 3.27)
project(DominionSetupCounter)

set(CMAKE_CXX_STANDARD 23)

find_package(boost_multiprecision CONFIG REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(magic_enum CONFIG REQUIRED)

set(Python_ROOT_DIR "${PROJECT_SOURCE_DIR}")

message(STATUS "${Python_EXECUTABLE}")
message(STATUS "${Python_ROOT_DIR}")

set(WARNINGS "-Wall -Wextra -Wsign-conversion -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 ${WARNINGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fPIC ${WARNINGS} -Werror")
message(STATUS "${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "${CMAKE_CXX_FLAGS_RELEASE}")

add_executable(dominion_setup_counter
        main.cpp
        config.hpp
        utils/tuple.hpp
        utils/constrained_product_generator.hpp
        utils/math.hpp
        utils/enum_mask.hpp
)

include_directories(vcpkg_installed/x64-linux/include)

include_directories(utils)
include_directories(config)

add_subdirectory(config)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(tst)
endif()

#if(DEFINED ENV{DOMINION_SETUP_COUNTER_STATIC})
#    message(STATUS "Link to std: static")
#    target_link_libraries(dominion_setup_counter -static)
#else()
#    message(STATUS "Link to std: dynamic")
#endif()
