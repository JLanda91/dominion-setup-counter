#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "lib/generator_bounds.hpp"
#include "lib/default_expansion_editions.hpp"

namespace {
    using config::coefficient_t;
    using config::GeneratorBounds;
    using config::Expansion;
    using config::kAllExpansionEditions;
    using config::kingdom::ExpansionEditionFilter;
    using config::kingdom::EditionModifier;

    using testing::FieldsAre;

    /*[[[cog
    from config_io import generator_config, card_list
    from generator_component_config_generator_bounds import ComponentConfigGeneratorBoundsGenerator
    generator = ComponentConfigGeneratorBoundsGenerator(card_list, generator_config)
    generator.create_tests()
    ]]]*/
    //[[[end]]]

}