// Implements tombstone physics and drawing.
#include "Entities/Tombstone.h"

#include "Utils/Constants.h"
#include "World/Collision.h"
#include "World/TileMap.h"

#include <algorithm>

namespace { const sf::Texture* tombstoneTexture = nullptr; }

Tombstone::Tombstone(sf::Vector2f position, Inventory inventory, int ownerId)
    : Entity(position, {28.0f, 34.0f}), inventory(inventory), ownerId(ownerId) {}

void Tombstone::setTexture(const sf::Texture& texture) { tombstoneTexture = &texture; }

void Tombstone::update(float dt, const TileMap& map) {
    velocity.y = std::min(Constants::MAX_FALL_SPEED, velocity.y + Constants::GRAVITY * dt);
    velocity *= dt;
    Collision::resolveTileCollision(*this, map);
    velocity /= dt;
}

void Tombstone::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    if (tombstoneTexture) {
        sf::Sprite sprite(*tombstoneTexture);
        const auto dimensions = tombstoneTexture->getSize();
        sprite.setScale({size.x / static_cast<float>(dimensions.x), size.y / static_cast<float>(dimensions.y)});
        sprite.setPosition(position - camera);
        window.draw(sprite);
        return;
    }
    sf::RectangleShape stone(size);
    stone.setPosition(position - camera);
    stone.setFillColor({166, 174, 181});
    window.draw(stone);

    sf::RectangleShape crossH({14.0f, 3.0f});
    crossH.setPosition(position - camera + sf::Vector2f(7.0f, 12.0f));
    crossH.setFillColor({80, 88, 96});
    window.draw(crossH);

    sf::RectangleShape crossV({4.0f, 14.0f});
    crossV.setPosition(position - camera + sf::Vector2f(12.0f, 7.0f));
    crossV.setFillColor({80, 88, 96});
    window.draw(crossV);

    const sf::Vector2f origin = position - camera + sf::Vector2f(8.0f, 22.0f);
    const sf::Color digitColor{39, 45, 52};
    if (ownerId == 1) {
        sf::RectangleShape digit({4.0f, 10.0f});
        digit.setPosition(origin + sf::Vector2f(5.0f, 0.0f));
        digit.setFillColor(digitColor);
        window.draw(digit);
    } else {
        sf::RectangleShape top({12.0f, 3.0f});
        top.setPosition(origin);
        top.setFillColor(digitColor);
        window.draw(top);

        sf::RectangleShape middle({12.0f, 3.0f});
        middle.setPosition(origin + sf::Vector2f(0.0f, 5.0f));
        middle.setFillColor(digitColor);
        window.draw(middle);

        sf::RectangleShape bottom({12.0f, 3.0f});
        bottom.setPosition(origin + sf::Vector2f(0.0f, 10.0f));
        bottom.setFillColor(digitColor);
        window.draw(bottom);

        sf::RectangleShape upperRight({3.0f, 5.0f});
        upperRight.setPosition(origin + sf::Vector2f(9.0f, 0.0f));
        upperRight.setFillColor(digitColor);
        window.draw(upperRight);

        sf::RectangleShape lowerLeft({3.0f, 5.0f});
        lowerLeft.setPosition(origin + sf::Vector2f(0.0f, 5.0f));
        lowerLeft.setFillColor(digitColor);
        window.draw(lowerLeft);
    }
}

const Inventory& Tombstone::getInventory() const { return inventory; }
int Tombstone::getOwnerId() const { return ownerId; }
