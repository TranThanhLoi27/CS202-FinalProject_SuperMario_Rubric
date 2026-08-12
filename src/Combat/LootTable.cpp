#include "Combat/LootTable.h"
#include <random>

// Roll a random number between 1 and 100 to determine drop item type:
// 50% COIN, 25% FOOD, 15% HEART, 10% BLOCK
ItemType LootTable::getRandomLoot() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(1, 100);

    int roll = distrib(gen);

    if (roll <= 50) {
        return ItemType::COIN;       // 50% chance
    } else if (roll <= 75) {
        return ItemType::FOOD;       // 25% chance
    } else if (roll <= 90) {
        return ItemType::HEART;      // 15% chance
    } else {
        return ItemType::BLOCK;      // 10% chance
    }
}
