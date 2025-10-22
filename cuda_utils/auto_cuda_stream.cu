#include "auto_cuda_stream.cuh"
#include "cuda_check.cuh"

AutoCudaStream::AutoCudaStream() {
    CUDA_CHECK(cudaStreamCreate(&stream_), "Failed to create cuda stream");
}

cudaStream_t& AutoCudaStream::operator()() {
    return stream_;
}

AutoCudaStream::~AutoCudaStream() {
    CUDA_CHECK(cudaStreamDestroy(stream_), "Failed to destroy cuda stream");
}