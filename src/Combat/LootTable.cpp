// Implements the random enemy loot distribution.
#include "Combat/LootTable.h"

#include <random>

ItemType LootTable::roll() {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    const float roll = dist(rng);
    if (roll < 0.50f) return ItemType::Coin;
    if (roll < 0.75f) return ItemType::Food;
    if (roll < 0.90f) return ItemType::Heart;
    return ItemType::Block;
}

