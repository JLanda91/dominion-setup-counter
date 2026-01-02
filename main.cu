#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <fmt/format.h>
#include <fmt/chrono.h>

#include <sparkplug/util/cuda/stream.cuh>

#include "data/mask_num_at_reduction.hpp"

#include "kernels/constant_memory.cuh"
#include "kernels/aggregate_by_type_mask_and_num_action_or_treasure.cuh"


[[maybe_unused]] static constexpr int NUM_SM = 38;


void ec53() {
    // Create an auto stream initialize cmem with EC data, EC offsets, and reduction items
    sparkplug::util::cuda::Stream stream{};
    kernels::initialize_cmem(stream);


    // auto h_intra_result = thrust::host_vector<data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult>(GRID_SIZE, {});
    // thrust::device_vector<data_definitions::reduce_type_and_is_action_or_treasure::ReductionResult> d_intra_result = h_intra_result;

    kernels::AggregateByTypeMaskAndNumActionOrTreasure aggregate_by_type_mask_and_num_action_or_treasure{stream};

    thrust::device_vector<data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution> ec_segment_distributions(
        std::begin(data::kECSegmentDistributions),
        std::end(data::kECSegmentDistributions));

    for (auto block_size = 32; block_size <= 128; block_size*=2) {
        constexpr auto num_work_items_per_block = 25000;

        const auto num_blocks_per_sm = 64*1024/(block_size * 43);
        const auto total_work_items = num_work_items_per_block * num_blocks_per_sm * NUM_SM;

        const auto num_blocks = num_blocks_per_sm * NUM_SM;
        const auto kernel_time_ms = aggregate_by_type_mask_and_num_action_or_treasure.Launch(num_blocks, block_size);
        const auto work_item_throughput_rate = static_cast<size_t>(1000.0 * static_cast<float>(total_work_items) / kernel_time_ms);
        fmt::println("Kernel block size: {}", block_size);
        fmt::println("Blocks per SM: {}", num_blocks_per_sm);
        fmt::println("Kernel time: {} ms for {} work items ({} work items per second)", kernel_time_ms , total_work_items, work_item_throughput_rate);
        fmt::println("");
    }




    // h_intra_result = d_intra_result;
    // // h_type_subset_result = d_type_subset_result;
    // for (auto & ec_segment_i : h_intra_result[0].data) {
    //     for (auto & n : ec_segment_i) {
    //         fmt::print("{}, ", n);
    //     }
    //     fmt::print("\n");
    // }
}


int main(){
    ec53();
}