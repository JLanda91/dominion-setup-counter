#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <fmt/format.h>
#include <fmt/chrono.h>

#include <sparkplug/util/cuda/stream.cuh>

#include "data_definitions/big_uint_format.cuh"

#include "data/mask_num_at_reduction.cuh"

#include "kernels/constant_memory.cuh"
#include "kernels/aggregate_by_type_mask_and_num_action_or_treasure.cuh"


[[maybe_unused]] static constexpr int NUM_SM = 38;


void ec53(kernels::NumThreads num_threads) {
    const auto max_active_blocks_per_multiprocessor = kernels::AggregateByTypeMaskAndNumActionOrTreasure::GetMaxActiveBlocksPerMultiprocessor(num_threads);
    fmt::println("Max active blocks (size {}) per multiprocessor: {}", static_cast<std::underlying_type_t<kernels::NumThreads>>(num_threads), max_active_blocks_per_multiprocessor);
    if (max_active_blocks_per_multiprocessor < 16) {
        throw std::runtime_error("No max blocks reached");
    }


    // Create an auto stream initialize cmem with EC data, EC offsets, and reduction items
    sparkplug::util::cuda::Stream stream{};
    kernels::initialize_cmem(stream);

    kernels::AggregateByTypeMaskAndNumActionOrTreasure aggregate_by_type_mask_and_num_action_or_treasure{stream};

    thrust::device_vector<data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution> gd_ec_segment_distributions(
        std::begin(data::kECSegmentDistributions),
        std::end(data::kECSegmentDistributions));

    constexpr auto num_blocks_per_sm = 16;
    constexpr auto total_work_items = kernels::AggregateByTypeMaskAndNumActionOrTreasure::kNumEmulatedItemsPerBlock * num_blocks_per_sm * NUM_SM;
    constexpr auto num_blocks = num_blocks_per_sm * NUM_SM;

    auto gh_phase1_result = thrust::host_vector<data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult>(1, data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult{});
    thrust::device_vector<data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult> gd_phase1_result = gh_phase1_result;

    auto gh_phase2_result = thrust::host_vector<data_definitions::reduce_mask_and_num_action_or_treasure::MultiplierTypeMaskAndNumActionTreasureTable>(1, data_definitions::reduce_mask_and_num_action_or_treasure::MultiplierTypeMaskAndNumActionTreasureTable{});
    thrust::device_vector<data_definitions::reduce_mask_and_num_action_or_treasure::MultiplierTypeMaskAndNumActionTreasureTable> gd_phase2_result = gh_phase2_result;

    const auto kernel_time_ms = aggregate_by_type_mask_and_num_action_or_treasure.Launch(
        num_blocks,
        num_threads,
        thrust::raw_pointer_cast(gd_ec_segment_distributions.data()),
        thrust::raw_pointer_cast(gd_phase1_result.data()),
        thrust::raw_pointer_cast(gd_phase2_result.data()));
    const auto work_item_throughput_rate = static_cast<size_t>(1000.0 * static_cast<float>(total_work_items) / kernel_time_ms);

    gh_phase1_result = gd_phase1_result;

    for (auto & ec_segment_i : gh_phase1_result[0].data) {
        for (auto & n : ec_segment_i) {
            fmt::print("{}, ", n);
        }
        fmt::print("\n");
    }

    fmt::println("");

    auto total = data_definitions::u128_t{};

    gh_phase2_result = gd_phase2_result;
    for (auto & ec_segment_i : gh_phase2_result[0].data) {
        for (auto & n : ec_segment_i) {
            fmt::print("{}, ", n);
            total += n;
        }
        fmt::print("\n");
    }

    fmt::println("");
    fmt::println("Total of the phase 2 table: {}", total);
    fmt::println("");

    fmt::println("Kernel block size: {}", static_cast<std::underlying_type_t<kernels::NumThreads>>(num_threads));
    fmt::println("Blocks per SM: {}", num_blocks_per_sm);
    fmt::println("Kernel time: {} ms for {} work items ({} work items per second)", kernel_time_ms , total_work_items, work_item_throughput_rate);
    fmt::println("");
}


int main(){
#ifdef DOUBLE_BUFFERED_PHASE_TWO
    fmt::println("Phase 2: Double buffered approach");
#else
    fmt::println("Phase 2: Naive approach");
#endif

    ec53(kernels::NumThreads::ThirtyTwo);
    // ec53(kernels::NumThreads::SixtyFour);
}
