add_library(cuda_utils STATIC
    auto_cuda_stream.cu
    auto_cuda_stream.cuh
    cuda_check.cuh
    scalar.cuh
    big_uint.cuh
)

set_target_properties(cuda_utils PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(cuda_utils PUBLIC fmt::fmt)