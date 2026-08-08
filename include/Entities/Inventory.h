#pragma once

enum class ItemType {
    Coin,
    Food,
    Heart,
    Block
};

class Inventory {
public:
    int coin = 0;
    int food = 0;
    int heart = 0;
    int block = 0;

    int count(ItemType type) const;
    int& count(ItemType type);
    void add(ItemType type, int amount = 1);
    bool remove(ItemType type, int amount = 1);
    bool empty() const;
    Inventory takeAll();
    void merge(const Inventory& other);
};
