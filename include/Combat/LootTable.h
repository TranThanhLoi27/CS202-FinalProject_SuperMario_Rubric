#pragma once

#include "Entities/DroppedItem.h"

// Helper class containing static logic for random loot generation
class LootTable {
public:
    // Returns a random ItemType based on defined percentage probabilities
    static ItemType getRandomLoot();
};
