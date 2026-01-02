#pragma once


namespace kernels {
    class BaseStreamLauncher {
    public:
        explicit BaseStreamLauncher(cudaStream_t stream);

    protected:
        cudaStream_t stream_;

        void RecordStart();
        float RecordStopSyncAndGetElapsedTime();

    private:
        cudaEvent_t start_{};
        cudaEvent_t stop_{};
        bool is_started_ = false;
    };
}