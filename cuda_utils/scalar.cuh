#pragma once

#include "cuda_check.cuh"

template<typename T>
class HostScalar {
public:
    HostScalar() {
        CUDA_CHECK(cudaMallocHost(&h_data_, sizeof(T)), "HostScalar cudaMallocHost failed");
    }

    void copy_from_device(const T* src, cudaStream_t& stream) const {
        CUDA_CHECK(cudaMemcpyAsync(h_data_, src, sizeof(T), cudaMemcpyDeviceToHost, stream), "HostScalar cudaMemcpyAsync failed");
    }

    [[nodiscard]] auto get() const -> T* {
        return h_data_;
    }

    [[nodiscard]] auto operator*() const -> const T& {
        return *h_data_;
    }

    ~HostScalar() {
        CUDA_CHECK(cudaFreeHost(h_data_), "HostScalar cudaFreeHost failed");
    }

private:
    T* h_data_ = nullptr;
};

template<typename T>
class DeviceScalar {
public:
    DeviceScalar() {
        CUDA_CHECK(cudaMalloc(&d_data_, sizeof(T)), "DeviceScalar cudaMalloc failed");
    }

    void copy_from_host(const T* src, cudaStream_t& stream) const {
        CUDA_CHECK(cudaMemcpyAsync(d_data_, src, sizeof(T), cudaMemcpyHostToDevice, stream), "DeviceScalar cudaMemcpyAsync failed");
    }

    [[nodiscard]] auto get() const -> T* {
        fmt::println("DeviceScalar::get"); fflush(stdout);
        return d_data_;
    }

    ~DeviceScalar() {
        CUDA_CHECK(cudaFree(d_data_), "DeviceScalar cudaFree failed");
    }

private:
    T* d_data_ = nullptr;
};

template<typename T>
class Scalar {
public:
    Scalar(const T& v) {
        CUDA_CHECK(cudaMallocHost(&h_data_, sizeof(T)), "Scalar cudaMallocHost failed");
        CUDA_CHECK(cudaMalloc(&d_data_, sizeof(T)), "Scalar cudaMalloc failed");
        *h_data_ = v;
    }

    [[nodiscard]] auto dev_ptr() const -> T* {
        return d_data_;
    }

    [[nodiscard]] auto value() const -> const T& {
        return *h_data_;
    }

    void update_host(cudaStream_t& stream) const {
        CUDA_CHECK(cudaMemcpyAsync(h_data_, d_data_, sizeof(T), cudaMemcpyDeviceToHost, stream), "Scalar cudaMemcpyDeviceToHost failed");
    }

    void update_device(cudaStream_t& stream) const {
        CUDA_CHECK(cudaMemcpyAsync(d_data_, h_data_, sizeof(T), cudaMemcpyHostToDevice, stream), "Scalar cudaMemcpyHostToDevice failed");
    }

    ~Scalar() {
        CUDA_CHECK(cudaFreeHost(h_data_), "HostScalar cudaFreeHost failed");
        CUDA_CHECK(cudaFree(d_data_), "Scalar cudaFree failed");
    }

private:
    T* h_data_ = nullptr;
    T* d_data_ = nullptr;
};
