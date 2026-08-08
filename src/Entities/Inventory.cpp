#include "Entities/Inventory.h"

int Inventory::count(ItemType type) const {
    if (type == ItemType::Coin) return coin;
    if (type == ItemType::Food) return food;
    if (type == ItemType::Heart) return heart;
    return block;
}

int& Inventory::count(ItemType type) {
    if (type == ItemType::Coin) return coin;
    if (type == ItemType::Food) return food;
    if (type == ItemType::Heart) return heart;
    return block;
}

void Inventory::add(ItemType type, int amount) {
    count(type) += amount;
}

bool Inventory::remove(ItemType type, int amount) {
    if (count(type) < amount) return false;
    count(type) -= amount;
    return true;
}

bool Inventory::empty() const {
    return coin + food + heart + block == 0;
}

Inventory Inventory::takeAll() {
    Inventory out;
    out.coin = coin;
    out.food = food;
    out.heart = heart;
    out.block = block;
    coin = 0;
    food = 0;
    heart = 0;
    block = 0;
    return out;
}

void Inventory::merge(const Inventory& other) {
    coin += other.coin;
    food += other.food;
    heart += other.heart;
    block += other.block;
}
