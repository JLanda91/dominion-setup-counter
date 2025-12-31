
function(gtest)
    cmake_parse_arguments(
        GTEST_TARGET        # prefix
        ""                  # options
        "NAME"      # single arg
        "LIBS;DEFS"              # lists
        ${ARGN}
    )

    add_executable(${GTEST_TARGET_NAME} "${GTEST_TARGET_NAME}.cpp")
    set_target_properties(${GTEST_TARGET_NAME} PROPERTIES
            CUDA_SEPARABLE_COMPILATION ON
    )
    target_link_libraries(${GTEST_TARGET_NAME} PRIVATE GTest::gtest_main GTest::gmock_main ${GTEST_TARGET_LIBS})

    add_test(NAME ${GTEST_TARGET_NAME} COMMAND ${GTEST_TARGET_NAME})
endfunction()


gtest(
        NAME component.data.mask_num_at_reduction
        LIBS data_mask_num_at_reduction fmt::fmt
)

