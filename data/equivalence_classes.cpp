#include "equivalence_classes.hpp"


namespace data {
    const kingdom_equivalence_classes::host_type kECData = {{
        // Loot 0 - 5
        { 2 },
        { 1 },
        { 1 },
        { 1 },
        { 1 },

        // Omen 5 - 9
        { 1 },
        { 3 },
        { 1 },
        { 1 },

        // Knights 9 - 10
        { 1 },

        // Druid 10 - 11
        { 1 },

        // Looter 11 - 14
        { 1 },
        { 1 },
        { 1 },

        // Fate 14 - 19
        { 1 },
        { 2 },
        { 2 },
        { 1 },
        { 1 },

        // Doom false 19 - 20
        { 1 },

        // Doom true 20 - 24
        { 1 },
        { 1 },
        { 1 },
        { 2 },

        // Liaison 24 - 30
        { 1 },
        { 1 },
        { 1 },
        { 1 },
        { 3 },
        { 2 },

        // Other false 30 - 35
        { 8 },
        { 2 },
        { 1 },
        { 6 },
        { 5 },

        // Other true 35 - 53
        { 26 },
        { 5 },
        { 2 },
        { 9 },
        { 5 },
        { 43 },
        { 12 },
        { 37 },
        { 2 },
        { 93 },
        { 13 },
        { 65 },
        { 8 },
        { 88 },
        { 26 },
        { 1 },
        { 1 },
        { 1 },
    }};

    const kingdom_equivalence_class_segments::host_type kECSegments = {{
        { data_definitions::kingdom::MultiplierType::Loot, true, 0, 5 },
        { data_definitions::kingdom::MultiplierType::Omen, true, 5, 9 },
        { data_definitions::kingdom::MultiplierType::Knights, true, 9, 10 },
        { data_definitions::kingdom::MultiplierType::Druid, true, 10, 11 },
        { data_definitions::kingdom::MultiplierType::Looter, true, 11, 14 },
        { data_definitions::kingdom::MultiplierType::Fate, true, 14, 19 },
        { data_definitions::kingdom::MultiplierType::Doom, false, 19, 20 },
        { data_definitions::kingdom::MultiplierType::Doom, true, 20, 24 },
        { data_definitions::kingdom::MultiplierType::Liaison, true, 24, 30 },
        { data_definitions::kingdom::MultiplierType::Other, false, 30, 35 },
        { data_definitions::kingdom::MultiplierType::Other, true, 35, 53 },
    }};

}