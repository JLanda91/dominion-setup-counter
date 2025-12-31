#pragma once

namespace data_definitions::kingdom {

    enum class EffectiveCardType : uint8_t {
        Loot = 0,
        Omen = 1,
        Knights = 2,
        Druid = 3,
        Looter = 4,
        Fate = 5,
        Doom = 6,
        Liaison = 7,
        None = 8,
        YoungWitch = 9,
        Ferryman = 10,
        Riverboat = 11,
    };

    inline constexpr uint8_t kNumEffectiveCardTypes = 12uz;
    inline constexpr uint8_t kNumEffectiveCardTypesWithFactors = 9uz;

    enum class MultiplierType : uint8_t {
        Loot = 0,
        Omen = 1,
        Knights = 2,
        Druid = 3,
        Looter = 4,
        Fate = 5,
        Doom = 6,
        Liaison = 7,
        Other = 8,
    };

    struct EquivalenceClass {
        uint8_t size{};
        uint64_t binom_data[11]{};

        constexpr EquivalenceClass() = default;

        constexpr EquivalenceClass(uint8_t s)
            : size(s) {
            for (uint8_t n = 0; n <= 10; ++n) {
                binom_data[n] = [](uint8_t n, uint8_t k) -> uint64_t {
                    if(k > n){ return 0; }
                    if(k == n || k == 0) { return 1; }
                    uint64_t result = 1;
                    const int max_j = k > n/2 ? n-k: k;
                    for(auto j = 0; j < max_j; ++j){
                        result *= (n - j);
                        result /= j+1;
                    }
                    return result;
                }(size, n);
            }
        }
    };

    struct EquivalenceClassSegment {
        MultiplierType effective_card_type{};
        bool is_action_treasure{};
        uint8_t begin{};
        uint8_t end{};
    };
}

