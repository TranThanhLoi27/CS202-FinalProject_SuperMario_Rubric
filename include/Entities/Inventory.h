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

    /// Creates an empty inventory with the configured starting block count.
    Inventory();

    /// Returns a slot count or zero when the index is invalid.
    int getSlot(int index) const;
    /// Returns mutable slot access or a safe dummy reference for an invalid index.
    int& getSlot(int index);
    /// Adds food to one of the four food-variant slots.
    void addFood(int variant, int amount = 1);
    /// Adds a positive amount to a specific slot.
    void addToSlot(int slot, int amount);
    /// Removes an amount when the slot contains enough items.
    bool removeFromSlot(int slot, int amount = 1);
    /// Moves the selected slot with wraparound.
    void cycleSlot(int delta);
    /// Selects a valid inventory slot directly.
    void selectSlot(int index);
    /// Reports whether the item-use cooldown has expired.
    bool canAct() const;
    /// Starts or extends the item-use cooldown.
    void startAction(float duration);
    /// Advances the item-use cooldown.
    void tick(float dt);

    /// Adds an item through its gameplay category.
    void add(ItemType type, int amount = 1, int foodVariant = 0);
    /// Removes an item through its gameplay category.
    bool remove(ItemType type, int amount = 1);
    /// Counts all items belonging to a gameplay category.
    int count(ItemType type) const;

    /// Reports whether every slot is empty.
    bool empty() const;
    /// Moves the full inventory into a value object and clears this inventory.
    Inventory takeAll();
    /// Adds all slot counts from another inventory.
    void merge(const Inventory& other);
};
