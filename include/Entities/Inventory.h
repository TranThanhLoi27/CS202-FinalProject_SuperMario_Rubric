#pragma once

enum class ItemType {
    Coin,
    Food,
    Heart,
    Block
};

constexpr int INVENTORY_SLOTS = 6;
constexpr int FOOD_TYPE_COUNT = 4;
constexpr int COIN_SLOT_INDEX = 4;
constexpr int BLOCK_SLOT_INDEX = 5;

class Inventory {
public:
    int slots[INVENTORY_SLOTS] = {};
    int selectedSlot = 0;
    float actionTimer = 0.0f;

    Inventory();

    int getSlot(int index) const;
    int& getSlot(int index);
    void addFood(int variant, int amount = 1);
    void addToSlot(int slot, int amount);
    bool removeFromSlot(int slot, int amount = 1);
    void cycleSlot(int delta);
    void selectSlot(int index);
    bool canAct() const;
    void startAction(float duration);
    void tick(float dt);

    void add(ItemType type, int amount = 1, int foodVariant = 0);
    bool remove(ItemType type, int amount = 1);
    int count(ItemType type) const;

    bool empty() const;
    Inventory takeAll();
    void merge(const Inventory& other);
};
