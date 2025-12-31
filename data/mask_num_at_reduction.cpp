#include <zstd.h>

#include <array>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

#include "mask_num_at_reduction.hpp"



namespace {
    constexpr auto kNumBytesPerDistribution = 6u;
    using decompressed_data_t = std::array<uint8_t, kNumBytesPerDistribution * data::kNumECSegmentDistributions>;

    decompressed_data_t read_from_zstd() {
        std::vector<uint8_t> compressed_data{};

        {
            // open zstd file
            std::ifstream zstd_f(MASK_NUM_AT_DATA_FILE, std::ios::binary);
            if (!zstd_f) {
                std::ostringstream err{};
                err << "No such file: " << MASK_NUM_AT_DATA_FILE;
                throw std::runtime_error(err.str());
            }

            // tell size
            zstd_f.seekg(0, std::ios::end);
            size_t compressed_size = zstd_f.tellg();
            zstd_f.seekg(0, std::ios::beg);

            // resize and read
            compressed_data.resize(compressed_size);
            zstd_f.read(reinterpret_cast<char*>(compressed_data.data()), compressed_size);
        }

        decompressed_data_t decompressed_data{};

        auto result = ZSTD_decompress(
            decompressed_data.data(),
            decompressed_data.size(),
            compressed_data.data(),
            compressed_data.size()
        );

        if (ZSTD_isError(result))
            throw std::runtime_error(ZSTD_getErrorName(result));

        if (result != std::size(decompressed_data))
            throw std::runtime_error("Decompressed size mismatch");

        return decompressed_data;
    }

    data::reduce_mask_and_num_action_or_treasure::host_type unpack_distributions(const decompressed_data_t& decompressed_data) {
        data::reduce_mask_and_num_action_or_treasure::host_type result{};

        const uint8_t* p_input = decompressed_data.data();
        for (std::size_t i = 0; i < data::kNumECSegmentDistributions; ++i) {
            std::vector<uint8_t> init(11, 0);
            std::size_t k = 0;
            for (std::size_t j = 0; j < 5; ++j) {
                init[k++] = *p_input & 0xF;
                init[k++] = *p_input >> 4;
                p_input++;
            }
            init[k] = *p_input & 0xF;
            uint8_t continuation_no = *p_input >> 4;
            p_input++;
            result[i] = data_definitions::reduce_mask_and_num_action_or_treasure::EquivalenceClassSegmentDistribution(init, continuation_no);
        }
        return result;
    }
}

namespace data {

    const reduce_mask_and_num_action_or_treasure::host_type kECSegmentDistributions = unpack_distributions(read_from_zstd());

    const ec_segment_continuations::host_type kECSegmentContinuations = {
        1,  1,  2,  2,  1,  2,  2,  2,  1,  2,  2,  2,  1,  2,  2,  2,  1,  2,  2,  3,  2,  2,  2,  3,  1,  2,  2,  3,
        2,  1,  2,  2,  2,  2,  2,  5,  2,  2,  2,  4,  2,  2,  2,  4,  2,  2,  2,  3,  2,  3,  3,  7,  2,  3,  3,  5,
        2,  3,  3,  4,  2,  3,  2,  3,  2,  3,  3,  6,  2,  3,  3,  5,  2,  3,  3,  6,  2,  2,  2,  4,  2,  5,  5, 10,
        2,  4,  4,  7,  2,  4,  5,  6,  2,  3,  3,  4,  3,  7,  7, 11,  3,  5,  5,  7,  3,  6,  6,  7,  2,  4,  4,  4,
        6, 10, 10, 12,  4,  6,  6,  7,  4,  6,  7,  6,  3,  4,  4,  3,  2,  2,  2,  4,  2,  2,  2,  3,  2,  2,  2,  4,
        2,  2,  2,  3,  2,  3,  3,  7,  2,  3,  3,  5,  2,  3,  3,  4,  2,  3,  2,  3,  2,  4,  4,  8,  2,  4,  3,  5,
        2,  4,  4,  5,  2,  2,  2,  3,  3,  6,  7,  8,  3,  4,  4,  5,  3,  4,  4,  5,  2,  3,  3,  2,  3,  7,  7, 11,
        3,  5,  5,  7,  3,  6,  6,  7,  3,  4,  4,  4,  6, 10, 10, 12,  4,  6,  6,  7,  4,  6,  7,  6,  3,  4,  4,  3,
        7, 11, 12, 13,  5,  7,  7,  6,  5,  7,  7,  7,  4,  4,  4,  3, 10, 12, 12, 10,  6,  7,  6,  5,  6,  7,  6,  4,
        4,  4,  4,  2
    };
}