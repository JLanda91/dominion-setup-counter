cmake_minimum_required(VERSION 3.27)
project(DominionSetupCounter CXX CUDA)

find_package(fmt CONFIG REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_package(OpenMP)
find_package(TBB CONFIG)

set(Python_ROOT_DIR "${PROJECT_SOURCE_DIR}")

message(STATUS "${Python_EXECUTABLE}")
message(STATUS "${Python_ROOT_DIR}")
message(STATUS "${CMAKE_CXX_FLAGS}")


if (DEFINED CMAKE_CUDA_COMPILER)
    add_subdirectory(cuda_utils)

    add_executable(cumain cumain.cu)
    target_link_libraries(cumain card_data utils)
endif()


include_directories(${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/include)

add_subdirectory(card_data)
add_subdirectory(utils)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(tst)
endif()

#if(DEFINED ENV{DOMINION_SETUP_COUNTER_STATIC})
#    message(STATUS "Link to std: static")
#    target_link_libraries(dominion_setup_counter -static)
#else()
#    message(STATUS "Link to std: dynamic")
#endif()
