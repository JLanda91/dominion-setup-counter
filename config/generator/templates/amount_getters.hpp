#pragma once

#include "lib/typedefs.hpp"

namespace config {
    namespace kingdom {
        /*[[[cog
        from config_io import generator_config, card_list
        from generator_amount_getter import AmountGetterGenerator
        generator = AmountGetterGenerator(card_list, generator_config)
        AmountGetterGenerator.create_enum_sum_functions()
        generator.create_kingdom_amount_getter_declarations()
        generator.create_kingdom_special_getter_declarations()
        ]]]*/
        //[[[end]]]
    }

    namespace landscapes {
        /*[[[cog
        AmountGetterGenerator.create_enum_sum_functions()
        generator.create_landscapes_supply_amount_getter_declarations()
        generator.create_landscapes_supply_special_getter_declarations()
        ]]]*/
        //[[[end]]]
    }
}
