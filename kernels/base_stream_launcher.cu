#include <sparkplug/util/cuda/check_cuda_fn_error.cuh>

#include "base_stream_launcher.cuh"


namespace kernels {
    BaseStreamLauncher::BaseStreamLauncher(cudaStream_t stream) : stream_(stream) {
        cudaEventCreate(&start_);
        cudaEventCreate(&stop_);
    }

    void BaseStreamLauncher::RecordStart() {
        is_started_ = true;
        sparkplug::util::cuda::check_cuda_fn_error(
            "Recording start",
            cudaEventRecord,
            start_,
            stream_);
    }

    float BaseStreamLauncher::RecordStopSyncAndGetElapsedTime() {
        sparkplug::util::cuda::check_cuda_fn_error(
            "Recording stop",
            cudaEventRecord,
            stop_,
            stream_);

        if (!is_started_) {
            throw std::runtime_error("Recording not started");
        }

        sparkplug::util::cuda::check_cuda_fn_error(
            "Event sync",
            cudaEventSynchronize,
            stop_);

        float elapsed_time_ms{};
        sparkplug::util::cuda::check_cuda_fn_error(
            "Determine kernel time",
            cudaEventElapsedTime,
            &elapsed_time_ms,
            start_,
            stop_);

        return elapsed_time_ms;
    }
}
