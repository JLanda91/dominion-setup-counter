#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstdint>

#include <card_data/kingdom/card_type.hpp>
#include <card_data/kingdom/membership_mask.hpp>

using namespace card_data;

struct Nonzero{
    kingdom::MembershipMask mask;
    kingdom::CardType card_type;
    uint8_t amount;
};

static constexpr std::size_t kNumNonZeros = 53uz;
static constexpr std::size_t n = 100000;

inline void add_host(const uint8_t (&arr)[kNumNonZeros * n], uint64_t& total) {
	for(const auto& elem : arr) {
        total += static_cast<uint64_t>(elem);
	}
}


auto main() -> int {
//    HDVector<uint8_t> x(kNumNonZeros * 100'000, 0);
    const Nonzero nnz[kNumNonZeros] = {
        {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::YoungWitch, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Knights, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Druid, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Ferryman, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Riverboat, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Looter, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::Looter, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Looter, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1000010), kingdom::CardType::Fate, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Fate, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Fate, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Fate, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Fate, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b0000010), kingdom::CardType::Doom, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::Doom, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Doom, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Doom, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Doom, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1000000), kingdom::CardType::Liaison, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1000001), kingdom::CardType::Liaison, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Liaison, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Liaison, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::Liaison, 3u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Liaison, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1100000), kingdom::CardType::Omen, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::Omen, 3u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Omen, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::Omen, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1000000), kingdom::CardType::Loot, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1000001), kingdom::CardType::Loot, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1100010), kingdom::CardType::Loot, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::Loot, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::Loot, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b0000000), kingdom::CardType::None, 8u},
        {kingdom::MembershipMask::FromUnsigned(0b0000001), kingdom::CardType::None, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b0000010), kingdom::CardType::None, 1u},
        {kingdom::MembershipMask::FromUnsigned(0b0001000), kingdom::CardType::None, 6u},
        {kingdom::MembershipMask::FromUnsigned(0b0001001), kingdom::CardType::None, 5u},
        {kingdom::MembershipMask::FromUnsigned(0b1000000), kingdom::CardType::None, 26u},
        {kingdom::MembershipMask::FromUnsigned(0b1000001), kingdom::CardType::None, 5u},
        {kingdom::MembershipMask::FromUnsigned(0b1000010), kingdom::CardType::None, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1001000), kingdom::CardType::None, 9u},
        {kingdom::MembershipMask::FromUnsigned(0b1001001), kingdom::CardType::None, 5u},
        {kingdom::MembershipMask::FromUnsigned(0b1100000), kingdom::CardType::None, 43u},
        {kingdom::MembershipMask::FromUnsigned(0b1100010), kingdom::CardType::None, 12u},
        {kingdom::MembershipMask::FromUnsigned(0b1100101), kingdom::CardType::None, 37u},
        {kingdom::MembershipMask::FromUnsigned(0b1100111), kingdom::CardType::None, 2u},
        {kingdom::MembershipMask::FromUnsigned(0b1101000), kingdom::CardType::None, 93u},
        {kingdom::MembershipMask::FromUnsigned(0b1101010), kingdom::CardType::None, 13u},
        {kingdom::MembershipMask::FromUnsigned(0b1101101), kingdom::CardType::None, 65u},
        {kingdom::MembershipMask::FromUnsigned(0b1101111), kingdom::CardType::None, 8u},
        {kingdom::MembershipMask::FromUnsigned(0b1110000), kingdom::CardType::None, 88u},
        {kingdom::MembershipMask::FromUnsigned(0b1110010), kingdom::CardType::None, 26u}
    };

    uint8_t x[n * kNumNonZeros];

    std::cout << "Launching kernel" << '\n';

    uint64_t total = 0ul;

    #pragma acc data copyin(nnz) create(x)
    {

      	for (int k = 0; k < 5; ++k){

        	#pragma acc parallel loop collapse(2) present(nnz, x)
    		for (auto i = 0u; i < kNumNonZeros; ++i) {
    			for (auto j = 0u; j < n; ++j) {
        			x[i * n + j] = nnz[i].amount;
    			}
    		}

            #pragma acc update host(x)

            add_host(x, total);
      	}
    }

    std::cout << total << '\n';

}