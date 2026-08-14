#include "Entities/Inventory.h"

#include "Utils/Constants.h"

#include <algorithm>

Inventory::Inventory() {
    slots[BLOCK_SLOT_INDEX] = Constants::START_BLOCK_COUNT;
}

int Inventory::getSlot(int index) const {
    if (index < 0 || index >= INVENTORY_SLOTS) return 0;
    return slots[index];
}

int& Inventory::getSlot(int index) {
    static int dummy = 0;
    if (index < 0 || index >= INVENTORY_SLOTS) return dummy;
    return slots[index];
}

void Inventory::addFood(int variant, int amount) {
    if (variant < 0 || variant >= FOOD_TYPE_COUNT || amount <= 0) return;
    slots[variant] += amount;
}

void Inventory::addToSlot(int slot, int amount) {
    if (slot < 0 || slot >= INVENTORY_SLOTS || amount <= 0) return;
    slots[slot] += amount;
}

bool Inventory::removeFromSlot(int slot, int amount) {
    if (slot < 0 || slot >= INVENTORY_SLOTS || slots[slot] < amount) return false;
    slots[slot] -= amount;
    return true;
}

void Inventory::cycleSlot(int delta) {
    selectedSlot = (selectedSlot + delta + INVENTORY_SLOTS) % INVENTORY_SLOTS;
}

void Inventory::selectSlot(int index) {
    if (index >= 0 && index < INVENTORY_SLOTS) selectedSlot = index;
}

bool Inventory::canAct() const {
    return actionTimer <= 0.0f;
}

void Inventory::startAction(float duration) {
    actionTimer = std::max(actionTimer, duration);
}

void Inventory::tick(float dt) {
    actionTimer = std::max(0.0f, actionTimer - dt);
}

void Inventory::add(ItemType type, int amount, int foodVariant) {
    if (amount <= 0) return;
    if (type == ItemType::Food) {
        addFood(foodVariant, amount);
        return;
    }
    if (type == ItemType::Coin) addToSlot(COIN_SLOT_INDEX, amount);
    if (type == ItemType::Heart) addFood(0, amount);
    if (type == ItemType::Block) addToSlot(BLOCK_SLOT_INDEX, amount);
}

bool Inventory::remove(ItemType type, int amount) {
    if (type == ItemType::Coin) return removeFromSlot(COIN_SLOT_INDEX, amount);
    if (type == ItemType::Block) return removeFromSlot(BLOCK_SLOT_INDEX, amount);
    if (type == ItemType::Food) {
        for (int i = 0; i < FOOD_TYPE_COUNT; ++i) {
            if (slots[i] >= amount) return removeFromSlot(i, amount);
        }
        return false;
    }
    return false;
}

int Inventory::count(ItemType type) const {
    if (type == ItemType::Coin) return slots[COIN_SLOT_INDEX];
    if (type == ItemType::Block) return slots[BLOCK_SLOT_INDEX];
    if (type == ItemType::Food) {
        int total = 0;
        for (int i = 0; i < FOOD_TYPE_COUNT; ++i) total += slots[i];
        return total;
    }
    return 0;
}

bool Inventory::empty() const {
    for (int count : slots) {
        if (count != 0) return false;
    }
    return true;
}

Inventory Inventory::takeAll() {
    Inventory out = *this;
    for (int& slot : slots) slot = 0;
    selectedSlot = 0;
    actionTimer = 0.0f;
    return out;
}

void Inventory::merge(const Inventory& other) {
    for (int i = 0; i < INVENTORY_SLOTS; ++i) {
        slots[i] += other.slots[i];
    }
}
