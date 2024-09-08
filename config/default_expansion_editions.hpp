#pragma once

#include "card_amount_tables.save.hpp"
#include "enum_mask.hpp"

namespace config {
    inline constexpr auto kAllExpansionEditions = kingdom::ExpansionEditionFilter{}.set();
}