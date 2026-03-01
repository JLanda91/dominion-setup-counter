#pragma once

#include <vector>

#include "big_uint.cuh"

namespace data_definitions::reduce_mask_and_num_action_or_treasure {
    struct alignas(8) EquivalenceClassSegmentDistribution {
        uint8_t data[6];
        uint8_t continuation_no;

        EquivalenceClassSegmentDistribution() = default;

        EquivalenceClassSegmentDistribution(const std::vector<uint8_t>& init, uint8_t c) : data{} {
            data[0] = (init[0] & 0xF) | ((init[1] & 0xF) << 4 );
            data[1] = (init[2] & 0xF) | ((init[3] & 0xF) << 4 );
            data[2] = (init[4] & 0xF) | ((init[5] & 0xF) << 4 );
            data[3] = (init[6] & 0xF) | ((init[7] & 0xF) << 4 );
            data[4] = (init[8] & 0xF) | ((init[9] & 0xF) << 4 );
            data[5] = (init[10] & 0xF);
            continuation_no = c;
        }
    };

    static_assert(std::is_trivially_copyable_v<EquivalenceClassSegmentDistribution>);
    static_assert(std::is_trivially_constructible_v<EquivalenceClassSegmentDistribution>);
    static_assert(alignof(EquivalenceClassSegmentDistribution) == 8);

    struct MultiplierTypeMaskAndNumActionTreasureTable {
        data_definitions::u128_t data[256][11];
    };
}