// Random loot table used when enemies die.
#pragma once

#include "Entities/Inventory.h"

class LootTable {
public:
    static ItemType roll();
};
