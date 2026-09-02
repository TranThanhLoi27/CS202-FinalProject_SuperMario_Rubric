// Physics item on the ground that players can collect or throw to each other.
#pragma once

#include "Entities/Entity.h"
#include "Entities/Inventory.h"

class DroppedItem : public Entity {
public:
    /// Registers shared textures for every dropped-item category.
    static void setTextures(const sf::Texture& food, const sf::Texture& coin, const sf::Texture& solid);

    /// Creates a collectible or throwable item stack.
    DroppedItem(sf::Vector2f position, ItemType type, int quantity = 1, int foodVariant = 0);
    /// Integrates item physics and resolves terrain collision.
    void update(float dt, const class TileMap& map);
    /// Draws the correct item texture and animation frame.
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;

    /// Returns the inventory category represented by the item.
    ItemType getType() const;
    /// Returns the number of items in the stack.
    int getQuantity() const;
    /// Returns the food spritesheet variant.
    int getFoodVariant() const;

private:
    ItemType type;
    int quantity = 1;
    int foodVariant = 0;
    float age = 0.0f;
};

