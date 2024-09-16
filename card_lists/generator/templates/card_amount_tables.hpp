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
    from generator_utils import create_enum_class
    from generator_card_amount_tables import CardAmountTablesGenerator
    generator = CardAmountTablesGenerator(card_list, generator_config)
    create_enum_class('Expansion', generator_config["supported_expansions"], 'uint8_t')
    ]]]*/
    //[[[end]]]
    namespace kingdom {
        /*[[[cog
        create_enum_class('EditionModifier', card_list["kingdom_regular"].index.unique(level=1), 'uint8_t')
        create_enum_class('CostGroup', card_list["kingdom_regular"].index.unique(level=2), 'uint8_t')
        create_enum_class('TrackedType', generator_config["tracked_kingdom_card_types"], 'uint8_t')
        ]]]*/
        //[[[end]]]
        using TrackedTypeMask = utils::table::EnumMask<SizedTrackedType>;

        struct table_row_t {
            std::pair<Expansion, EditionModifier> expansion_edition_;
            TrackedTypeMask tracked_types_mask_;
            CostGroup cost_group_;
            uint8_t amount_;

            constexpr bool operator==(const table_row_t& other) const noexcept = default;
        };

        /*[[[cog
        generator.create_kingdom_table()
        ]]]*/
        //[[[end]]]
        using ExpansionEditionFilter = utils::table::EnumMask<SizedExpansion, SizedEditionModifier>;
        using CostGroupFilter = utils::table::EnumMask<SizedCostGroup>;

        struct TableQuery {
            TrackedTypeMask tracked_types_mask_{};
            CostGroup cost_group_{};
        };

        /*[[[cog
        generator.create_kingdom_amount_queries()
        generator.create_kingdom_special_predicates()
        generator.create_kingdom_amount_index()
        ]]]*/
        //[[[end]]]
    }

    namespace landscapes {
        using ExpansionFilter = utils::table::EnumMask<SizedExpansion>;

        namespace supply {
            /*[[[cog
            from itertools import chain
            create_enum_class('Type', list(chain.from_iterable(group["strings"] for group in generator_config["supply_landscape_groups"])), 'uint8_t')
            ]]]*/
            //[[[end]]]
            struct table_row_t {
                Expansion expansion_;
                Type type_;
                uint8_t amount_;

                constexpr bool operator==(const table_row_t& other) const noexcept = default;
            };

            /*[[[cog
            generator.create_landscapes_supply_table()
            ]]]*/
            //[[[end]]]
            using TypeFilter = utils::table::EnumMask<SizedType>;

            struct TableQuery {
                TypeFilter type_{};
            };

            /*[[[cog
            generator.create_landscapes_supply_queries()
            generator.create_landscapes_supply_specials()
            generator.create_landscapes_amount_index()
            ]]]*/
            //[[[end]]]
        }

        namespace other {
            /*[[[cog
            create_enum_class('Type', generator_config["other_landscape_types"], 'uint8_t')
            ]]]*/
            //[[[end]]]
            using SizedType = utils::enums::SizedEnum<Type, 4uz>;
            using TypeFilter = utils::table::EnumMask<SizedType>;

            struct TableQuery {
                TypeFilter type_{};
            };

            struct table_row_t {
                Expansion expansion_;
                Type type_;
                uint8_t amount_;

                constexpr bool operator==(const table_row_t& other) const noexcept = default;
            };

            /*[[[cog
            generator.create_landscapes_other_table()
            ]]]*/
            //[[[end]]]
        }
    }
}