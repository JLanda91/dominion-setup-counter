find_package(GTest REQUIRED)

message(STATUS "Including tests")

macro(gtest)
    set(options)
    set(one_value_args NAME)
    set(multi_value_args LIBS)
    cmake_parse_arguments(TEST "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    target_compile_options(${TEST_NAME} PRIVATE -fsanitize=address)
    target_link_options(${TEST_NAME} PRIVATE -fsanitize=address -static-libasan)
    target_link_libraries(${TEST_NAME} GTest::gtest_main GTest::gmock_main ${TEST_LIBS})
    gtest_discover_tests(${TEST_NAME})
endmacro(gtest)

gtest(NAME unit.utils.enum_mask)
gtest(NAME unit.config.generator_bounds LIBS config)
gtest(NAME component.config.generator_bounds LIBS config)