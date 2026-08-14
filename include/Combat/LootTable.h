// Random loot table used when enemies die.
#pragma once

#include "Entities/Inventory.h"

// Helper class containing static logic for random loot generation
class LootTable {
public:
    static ItemType roll();
};

