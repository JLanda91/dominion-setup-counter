#pragma once

#include <cstdint>
#include <array>
#include <utility>

#include "sized_enum.hpp"
#include "enum_mask.hpp"

namespace config {
    /*[[[cog
    import cog
    from config_io import generator_config, card_list
    import generator_utils
    generator_utils.create_enum_class('Expansion', generator_config["supported_expansions"], 'uint8_t')
    ]]]*/
    //[[[end]]]

    namespace kingdom {
        /*[[[cog
        generator_utils.create_enum_class('EditionModifier', card_list["kingdom_regular"].index.unique(level=1), 'uint8_t')
        generator_utils.create_enum_class('CostGroup', card_list["kingdom_regular"].index.unique(level=2), 'uint8_t')
        generator_utils.create_enum_class('TrackedType', generator_config["tracked_kingdom_card_types"], 'uint8_t')
        ]]]*/
        //[[[end]]]

        using TrackedTypeMask = utils::table::EnumMask<SizedTrackedType>;

        struct row_t {
            std::pair<Expansion, EditionModifier> expansion_edition_;
            TrackedTypeMask tracked_types_;
            CostGroup cost_group_;
            uint8_t amount_;

            constexpr bool operator==(const row_t& other) const noexcept = default;
        };

        /*[[[cog
        generator_utils.create_kingdom_table(card_list["kingdom_regular"])
        ]]]*/
        //[[[end]]]

        using ExpansionEditionFilter = utils::table::EnumMask<SizedExpansion, SizedEditionModifier>;
        using CostGroupFilter = utils::table::EnumMask<SizedCostGroup>;

        struct TableQuery {
            TrackedTypeMask tracked_types_{};
            CostGroupFilter cost_group_{};
        };

        /*[[[cog
        generator_utils.create_kingdom_amount_queries(card_list["kingdom_queries"])
        generator_utils.create_kingdom_special_predicates(card_list["kingdom_special"])
        generator_utils.create_kingdom_amount_index(card_list["kingdom_queries"], card_list["kingdom_special"])
        ]]]*/
        //[[[end]]]
    }

    namespace landscapes {
        /*[[[cog
        from itertools import chain
        generator_utils.create_enum_class('Type', list(chain.from_iterable(group["strings"] for group in generator_config["supply_landscape_groups"])) + generator_config["tracked_landscape_piles"], 'uint8_t')
        ]]]*/
        //[[[end]]]

        struct row_t {
            Expansion expansion_;
            Type type_;
            uint8_t amount_;

            constexpr bool operator==(const row_t& other) const noexcept = default;
        };

        /*[[[cog
        generator_utils.create_landscapes_supply_table(card_list["landscapes_supply_regular"])
        ]]]*/
        //[[[end]]]

        using ExpansionFilter = utils::table::EnumMask<SizedExpansion>;
        using TypeFilter = utils::table::EnumMask<SizedType>;

        struct TableQuery {
            TypeFilter type_{};
        };

        /*[[[cog
        generator_utils.create_landscapes_supply_queries(generator_config["supply_landscape_groups"])
        generator_utils.create_landscapes_supply_specials(card_list["landscapes_supply_special"])
        generator_utils.create_landscapes_amount_index(generator_config["supply_landscape_groups"], card_list["landscapes_supply_special"])
        ]]]*/
        //[[[end]]]
    }
}