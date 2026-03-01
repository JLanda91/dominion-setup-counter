#pragma once


namespace kernels {
    class BaseStreamLauncher {
    public:
        explicit BaseStreamLauncher(cudaStream_t stream);
        virtual ~BaseStreamLauncher() = default;

    protected:
        cudaStream_t stream_;

        void RecordStart();
        float RecordStopSyncAndGetElapsedTime();

    private:
        cudaEvent_t start_{};
        cudaEvent_t stop_{};
        bool is_started_ = false;
    };

    enum class NumThreads : int {
        ThirtyTwo = 32,
        SixtyFour = 64,
    };
}