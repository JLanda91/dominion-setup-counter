#include "amount_getters.hpp"

namespace config {
    namespace kingdom {
        auto looter_total(const amounts_t& t) noexcept -> coefficient_t {
            return enum_index_get_sum<
        		AmountIndex::ACTION_ATTACK_LOOTER_FOUR,
        		AmountIndex::ACTION_ATTACK_LOOTER_FIVE,
        		AmountIndex::ACTION_LOOTER_FOUR
        	>(t);
        }

        auto fate_total(const amounts_t& t) noexcept -> coefficient_t {
            return enum_index_get_sum<
        		AmountIndex::ACTION_FATE_TWO,
        		AmountIndex::ACTION_FATE_THREE,
        		AmountIndex::ACTION_FATE_FOUR,
        		AmountIndex::ACTION_FATE_FIVE,
        		AmountIndex::ATTACK_FATE_TREASURE_FIVE
        	>(t);
        }

        auto doom_total(const amounts_t& t) noexcept -> coefficient_t {
            return enum_index_get_sum<
        		AmountIndex::ACTION_ATTACK_DOOM_FOUR,
        		AmountIndex::ACTION_ATTACK_DOOM_FIVE,
        		AmountIndex::ACTION_DOOM_THREE,
        		AmountIndex::ACTION_DOOM_FIVE,
        		AmountIndex::ATTACK_DOOM_FIVE
        	>(t);
        }

        auto liaison_total(const amounts_t& t) noexcept -> coefficient_t {
            return enum_index_get_sum<
        		AmountIndex::ACTION_DURATION_LIAISON_THREE,
        		AmountIndex::ACTION_LIAISON_TWO,
        		AmountIndex::ACTION_LIAISON_THREE,
        		AmountIndex::ACTION_LIAISON_FOUR,
        		AmountIndex::ACTION_LIAISON_FIVE,
        		AmountIndex::DURATION_LIAISON_TREASURE_FIVE,
        		AmountIndex::LIAISON_TREASURE_TWO
        	>(t);
        }

        auto loot_total(const amounts_t& t) noexcept -> coefficient_t {
            return enum_index_get_sum<
        		AmountIndex::ACTION_ATTACK_DURATION_LOOT_FIVE,
        		AmountIndex::ACTION_DURATION_LOOT_TWO,
        		AmountIndex::ACTION_LOOT_FIVE,
        		AmountIndex::LOOT_TREASURE_TWO,
        		AmountIndex::LOOT_TREASURE_FIVE,
        		AmountIndex::LOOT_TREASURE_OTHER
        	>(t);
        }

        auto two_three_total(const amounts_t& t) noexcept -> coefficient_t {
            return enum_index_get_sum<
        		AmountIndex::NONE_TWO,
        		AmountIndex::NONE_THREE,
        		AmountIndex::ACTION_TWO,
        		AmountIndex::ACTION_THREE,
        		AmountIndex::ACTION_ATTACK_TWO,
        		AmountIndex::ACTION_ATTACK_THREE,
        		AmountIndex::ACTION_ATTACK_DURATION_THREE,
        		AmountIndex::ACTION_DOOM_THREE,
        		AmountIndex::ACTION_DURATION_TWO,
        		AmountIndex::ACTION_DURATION_THREE,
        		AmountIndex::ACTION_DURATION_LIAISON_THREE,
        		AmountIndex::ACTION_DURATION_LOOT_TWO,
        		AmountIndex::ACTION_FATE_TWO,
        		AmountIndex::ACTION_FATE_THREE,
        		AmountIndex::ACTION_LIAISON_TWO,
        		AmountIndex::ACTION_LIAISON_THREE,
        		AmountIndex::DURATION_TWO,
        		AmountIndex::DURATION_THREE,
        		AmountIndex::DURATION_TREASURE_TWO,
        		AmountIndex::DURATION_TREASURE_THREE,
        		AmountIndex::LIAISON_TREASURE_TWO,
        		AmountIndex::LOOT_TREASURE_TWO,
        		AmountIndex::TREASURE_TWO,
        		AmountIndex::TREASURE_THREE
        	>(t);
        }

        auto unused_three_four_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t {
            return enum_index_get_sum_diff<
        		AmountIndex::NONE_THREE,
        		AmountIndex::NONE_FOUR,
        		AmountIndex::ACTION_THREE,
        		AmountIndex::ACTION_FOUR,
        		AmountIndex::ACTION_ATTACK_THREE,
        		AmountIndex::ACTION_ATTACK_FOUR,
        		AmountIndex::ACTION_ATTACK_DOOM_FOUR,
        		AmountIndex::ACTION_ATTACK_DURATION_THREE,
        		AmountIndex::ACTION_ATTACK_DURATION_FOUR,
        		AmountIndex::ACTION_ATTACK_LOOTER_FOUR,
        		AmountIndex::ACTION_DOOM_THREE,
        		AmountIndex::ACTION_DURATION_THREE,
        		AmountIndex::ACTION_DURATION_FOUR,
        		AmountIndex::ACTION_DURATION_LIAISON_THREE,
        		AmountIndex::ACTION_FATE_THREE,
        		AmountIndex::ACTION_FATE_FOUR,
        		AmountIndex::ACTION_LIAISON_THREE,
        		AmountIndex::ACTION_LIAISON_FOUR,
        		AmountIndex::ACTION_LOOTER_FOUR,
        		AmountIndex::ACTION_OMEN_FOUR,
        		AmountIndex::DURATION_THREE,
        		AmountIndex::DURATION_TREASURE_THREE,
        		AmountIndex::DURATION_TREASURE_FOUR,
        		AmountIndex::TREASURE_THREE,
        		AmountIndex::TREASURE_FOUR
        	>(t, max);
        }

        auto unused_action_nonduration_five_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t {
            return enum_index_get_sum_diff<
        		AmountIndex::ACTION_FIVE,
        		AmountIndex::ACTION_ATTACK_FIVE,
        		AmountIndex::ACTION_ATTACK_DOOM_FIVE,
        		AmountIndex::ACTION_ATTACK_LOOTER_FIVE,
        		AmountIndex::ACTION_ATTACK_OMEN_FIVE,
        		AmountIndex::ACTION_DOOM_FIVE,
        		AmountIndex::ACTION_FATE_FIVE,
        		AmountIndex::ACTION_LIAISON_FIVE,
        		AmountIndex::ACTION_LOOT_FIVE,
        		AmountIndex::ACTION_OMEN_FIVE,
        		AmountIndex::ACTION_TREASURE_FIVE
        	>(t, max);
        }

        auto action_total(const amounts_t& t) noexcept -> coefficient_t {
            return enum_index_get_sum<
        		AmountIndex::ACTION_TWO,
        		AmountIndex::ACTION_THREE,
        		AmountIndex::ACTION_FOUR,
        		AmountIndex::ACTION_FIVE,
        		AmountIndex::ACTION_OTHER,
        		AmountIndex::ACTION_ATTACK_TWO,
        		AmountIndex::ACTION_ATTACK_THREE,
        		AmountIndex::ACTION_ATTACK_FOUR,
        		AmountIndex::ACTION_ATTACK_FIVE,
        		AmountIndex::ACTION_ATTACK_OTHER,
        		AmountIndex::ACTION_ATTACK_DOOM_FOUR,
        		AmountIndex::ACTION_ATTACK_DOOM_FIVE,
        		AmountIndex::ACTION_ATTACK_DURATION_THREE,
        		AmountIndex::ACTION_ATTACK_DURATION_FOUR,
        		AmountIndex::ACTION_ATTACK_DURATION_FIVE,
        		AmountIndex::ACTION_ATTACK_DURATION_OTHER,
        		AmountIndex::ACTION_ATTACK_DURATION_LOOT_FIVE,
        		AmountIndex::ACTION_ATTACK_LOOTER_FOUR,
        		AmountIndex::ACTION_ATTACK_LOOTER_FIVE,
        		AmountIndex::ACTION_ATTACK_OMEN_FIVE,
        		AmountIndex::ACTION_DOOM_THREE,
        		AmountIndex::ACTION_DOOM_FIVE,
        		AmountIndex::ACTION_DURATION_TWO,
        		AmountIndex::ACTION_DURATION_THREE,
        		AmountIndex::ACTION_DURATION_FOUR,
        		AmountIndex::ACTION_DURATION_FIVE,
        		AmountIndex::ACTION_DURATION_OTHER,
        		AmountIndex::ACTION_DURATION_LIAISON_THREE,
        		AmountIndex::ACTION_DURATION_LOOT_TWO,
        		AmountIndex::ACTION_FATE_TWO,
        		AmountIndex::ACTION_FATE_THREE,
        		AmountIndex::ACTION_FATE_FOUR,
        		AmountIndex::ACTION_FATE_FIVE,
        		AmountIndex::ACTION_LIAISON_TWO,
        		AmountIndex::ACTION_LIAISON_THREE,
        		AmountIndex::ACTION_LIAISON_FOUR,
        		AmountIndex::ACTION_LIAISON_FIVE,
        		AmountIndex::ACTION_LOOT_FIVE,
        		AmountIndex::ACTION_LOOTER_FOUR,
        		AmountIndex::ACTION_OMEN_FOUR,
        		AmountIndex::ACTION_OMEN_FIVE,
        		AmountIndex::ACTION_OMEN_OTHER,
        		AmountIndex::ACTION_TREASURE_FIVE
        	>(t);
        }

        auto unused_action_two_three_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t {
            return enum_index_get_sum_diff<
        		AmountIndex::ACTION_TWO,
        		AmountIndex::ACTION_THREE,
        		AmountIndex::ACTION_ATTACK_TWO,
        		AmountIndex::ACTION_ATTACK_THREE,
        		AmountIndex::ACTION_ATTACK_DURATION_THREE,
        		AmountIndex::ACTION_DOOM_THREE,
        		AmountIndex::ACTION_DURATION_TWO,
        		AmountIndex::ACTION_DURATION_THREE,
        		AmountIndex::ACTION_DURATION_LIAISON_THREE,
        		AmountIndex::ACTION_DURATION_LOOT_TWO,
        		AmountIndex::ACTION_FATE_TWO,
        		AmountIndex::ACTION_FATE_THREE,
        		AmountIndex::ACTION_LIAISON_TWO,
        		AmountIndex::ACTION_LIAISON_THREE
        	>(t, max);
        }

        auto unused_attack_total(const amounts_t& t, const amounts_t& max) noexcept -> coefficient_t {
            return enum_index_get_sum_diff<
        		AmountIndex::ACTION_ATTACK_TWO,
        		AmountIndex::ACTION_ATTACK_THREE,
        		AmountIndex::ACTION_ATTACK_FOUR,
        		AmountIndex::ACTION_ATTACK_FIVE,
        		AmountIndex::ACTION_ATTACK_OTHER,
        		AmountIndex::ACTION_ATTACK_DOOM_FOUR,
        		AmountIndex::ACTION_ATTACK_DOOM_FIVE,
        		AmountIndex::ACTION_ATTACK_DURATION_THREE,
        		AmountIndex::ACTION_ATTACK_DURATION_FOUR,
        		AmountIndex::ACTION_ATTACK_DURATION_FIVE,
        		AmountIndex::ACTION_ATTACK_DURATION_OTHER,
        		AmountIndex::ACTION_ATTACK_DURATION_LOOT_FIVE,
        		AmountIndex::ACTION_ATTACK_LOOTER_FOUR,
        		AmountIndex::ACTION_ATTACK_LOOTER_FIVE,
        		AmountIndex::ACTION_ATTACK_OMEN_FIVE,
        		AmountIndex::ATTACK_DOOM_FIVE,
        		AmountIndex::ATTACK_DURATION_OTHER,
        		AmountIndex::ATTACK_FATE_TREASURE_FIVE,
        		AmountIndex::ATTACK_TREASURE_FIVE
        	>(t, max);
        }

        auto action_or_treasure_total(const amounts_t& t) noexcept -> coefficient_t {
            return enum_index_get_sum<
        		AmountIndex::ACTION_TWO,
        		AmountIndex::ACTION_THREE,
        		AmountIndex::ACTION_FOUR,
        		AmountIndex::ACTION_FIVE,
        		AmountIndex::ACTION_OTHER,
        		AmountIndex::ACTION_ATTACK_TWO,
        		AmountIndex::ACTION_ATTACK_THREE,
        		AmountIndex::ACTION_ATTACK_FOUR,
        		AmountIndex::ACTION_ATTACK_FIVE,
        		AmountIndex::ACTION_ATTACK_OTHER,
        		AmountIndex::ACTION_ATTACK_DOOM_FOUR,
        		AmountIndex::ACTION_ATTACK_DOOM_FIVE,
        		AmountIndex::ACTION_ATTACK_DURATION_THREE,
        		AmountIndex::ACTION_ATTACK_DURATION_FOUR,
        		AmountIndex::ACTION_ATTACK_DURATION_FIVE,
        		AmountIndex::ACTION_ATTACK_DURATION_OTHER,
        		AmountIndex::ACTION_ATTACK_DURATION_LOOT_FIVE,
        		AmountIndex::ACTION_ATTACK_LOOTER_FOUR,
        		AmountIndex::ACTION_ATTACK_LOOTER_FIVE,
        		AmountIndex::ACTION_ATTACK_OMEN_FIVE,
        		AmountIndex::ACTION_DOOM_THREE,
        		AmountIndex::ACTION_DOOM_FIVE,
        		AmountIndex::ACTION_DURATION_TWO,
        		AmountIndex::ACTION_DURATION_THREE,
        		AmountIndex::ACTION_DURATION_FOUR,
        		AmountIndex::ACTION_DURATION_FIVE,
        		AmountIndex::ACTION_DURATION_OTHER,
        		AmountIndex::ACTION_DURATION_LIAISON_THREE,
        		AmountIndex::ACTION_DURATION_LOOT_TWO,
        		AmountIndex::ACTION_FATE_TWO,
        		AmountIndex::ACTION_FATE_THREE,
        		AmountIndex::ACTION_FATE_FOUR,
        		AmountIndex::ACTION_FATE_FIVE,
        		AmountIndex::ACTION_LIAISON_TWO,
        		AmountIndex::ACTION_LIAISON_THREE,
        		AmountIndex::ACTION_LIAISON_FOUR,
        		AmountIndex::ACTION_LIAISON_FIVE,
        		AmountIndex::ACTION_LOOT_FIVE,
        		AmountIndex::ACTION_LOOTER_FOUR,
        		AmountIndex::ACTION_OMEN_FOUR,
        		AmountIndex::ACTION_OMEN_FIVE,
        		AmountIndex::ACTION_OMEN_OTHER,
        		AmountIndex::ACTION_TREASURE_FIVE,
        		AmountIndex::ATTACK_FATE_TREASURE_FIVE,
        		AmountIndex::ATTACK_TREASURE_FIVE,
        		AmountIndex::DURATION_LIAISON_TREASURE_FIVE,
        		AmountIndex::DURATION_TREASURE_TWO,
        		AmountIndex::DURATION_TREASURE_THREE,
        		AmountIndex::DURATION_TREASURE_FOUR,
        		AmountIndex::DURATION_TREASURE_FIVE,
        		AmountIndex::LIAISON_TREASURE_TWO,
        		AmountIndex::LOOT_TREASURE_TWO,
        		AmountIndex::LOOT_TREASURE_FIVE,
        		AmountIndex::LOOT_TREASURE_OTHER,
        		AmountIndex::TREASURE_TWO,
        		AmountIndex::TREASURE_THREE,
        		AmountIndex::TREASURE_FOUR,
        		AmountIndex::TREASURE_FIVE,
        		AmountIndex::TREASURE_OTHER
        	>(t);
        }

        auto has_young_witch(const amounts_t& t) noexcept -> bool {
            return std::get<std::to_underlying(AmountIndex::YOUNG_WITCH)>(t) > 0u;
        }

        auto has_ferryman(const amounts_t& t) noexcept -> bool {
            return std::get<std::to_underlying(AmountIndex::FERRYMAN)>(t) > 0u;
        }

        auto has_knights(const amounts_t& t) noexcept -> bool {
            return std::get<std::to_underlying(AmountIndex::KNIGHTS)>(t) > 0u;
        }

        auto has_druid(const amounts_t& t) noexcept -> bool {
            return std::get<std::to_underlying(AmountIndex::DRUID)>(t) > 0u;
        }

        auto has_riverboat(const amounts_t& t) noexcept -> bool {
            return std::get<std::to_underlying(AmountIndex::RIVERBOAT)>(t) > 0u;
        }

    }

    namespace landscapes {
        namespace supply {
            auto event_total(const amounts_t& t) noexcept -> coefficient_t {
                return std::get<std::to_underlying(AmountIndex::EVENT)>(t);
            }

            auto trait_total(const amounts_t& t) noexcept -> coefficient_t {
                return std::get<std::to_underlying(AmountIndex::TRAIT)>(t);
            }

            auto other_supply_total(const amounts_t& t) noexcept -> coefficient_t {
                return std::get<std::to_underlying(AmountIndex::OTHER_SUPPLY)>(t);
            }

            auto has_obelisk(const amounts_t& t) noexcept -> bool {
                return std::get<std::to_underlying(AmountIndex::OBELISK)>(t) > 0u;
            }

            auto has_way_of_the_mouse(const amounts_t& t) noexcept -> bool {
                return std::get<std::to_underlying(AmountIndex::WAY_OF_THE_MOUSE)>(t) > 0u;
            }

        }

        namespace other {
            auto hex_total(const amounts_t& t) noexcept -> coefficient_t {
                return std::get<std::to_underlying(Type::HEX)>(t);
            }

            auto boon_total(const amounts_t& t) noexcept -> coefficient_t {
                return std::get<std::to_underlying(Type::BOON)>(t);
            }

            auto ally_total(const amounts_t& t) noexcept -> coefficient_t {
                return std::get<std::to_underlying(Type::ALLY)>(t);
            }

            auto prophecy_total(const amounts_t& t) noexcept -> coefficient_t {
                return std::get<std::to_underlying(Type::PROPHECY)>(t);
            }


        }
    }
}
