#include "amount_getters.hpp"

namespace config {
    namespace kingdom {
        /*[[[cog
        from config_io import generator_config, card_list
        from generator_amount_getter import AmountGetterGenerator
        generator = AmountGetterGenerator(card_list, generator_config)
        generator.create_kingdom_amount_getter_definitions()
        generator.create_kingdom_special_getter_definitions()
        ]]]*/
        //[[[end]]]
    }

    namespace landscapes {
        /*[[[cog
        generator.create_landscapes_supply_amount_getter_definitions()
        generator.create_landscapes_supply_special_getter_definitions()
        ]]]*/
        //[[[end]]]
    }
}
