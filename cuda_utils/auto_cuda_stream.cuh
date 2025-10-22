#pragma once

class AutoCudaStream {
public:
    explicit AutoCudaStream();

    cudaStream_t& operator()();

    ~AutoCudaStream();

private:
    cudaStream_t stream_{};
};
