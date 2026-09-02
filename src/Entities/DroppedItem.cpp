// Implements thrown/dropped item physics and textured drawing.
#include "Entities/DroppedItem.h"

#include "Utils/Constants.h"
#include "World/Collision.h"
#include "World/TileMap.h"

#include <algorithm>
#include <cmath>
#include <random>

namespace {
const sf::Texture* foodTex = nullptr;
const sf::Texture* coinTex = nullptr;
const sf::Texture* solidTex = nullptr;

// Produces a uniformly distributed value within the requested range.
float randomFloat(float low, float high) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(low, high);
    return dist(rng);
}
}

void DroppedItem::setTextures(const sf::Texture& food, const sf::Texture& coin, const sf::Texture& solid) {
    foodTex = &food;
    coinTex = &coin;
    solidTex = &solid;
}

DroppedItem::DroppedItem(sf::Vector2f position, ItemType type, int quantity, int foodVariant)
    : Entity(position, {static_cast<float>(Constants::ITEM_ICON_SIZE), static_cast<float>(Constants::ITEM_ICON_SIZE)}),
      type(type),
      quantity(quantity),
      foodVariant(foodVariant) {
    velocity = {randomFloat(-90.0f, 90.0f), -260.0f};
}

void DroppedItem::update(float dt, const TileMap& map) {
    age += dt;
    velocity.x *= 0.985f;
    velocity.y = std::min(Constants::MAX_FALL_SPEED, velocity.y + Constants::GRAVITY * dt);
    velocity *= dt;
    Collision::resolveTileCollision(*this, map);
    velocity /= dt;
}

void DroppedItem::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    const sf::Vector2f bob = {0.0f, std::sin(age * 7.0f) * 2.0f};
    const sf::Vector2f drawPos = position - camera + bob;

    if (type == ItemType::Food && foodTex) {
        sf::Sprite sprite(*foodTex);
        sprite.setTextureRect(sf::IntRect({foodVariant * Constants::ITEM_ICON_SIZE, 0},
                                          {Constants::ITEM_ICON_SIZE, Constants::ITEM_ICON_SIZE}));
        sprite.setPosition(drawPos);
        window.draw(sprite);
        return;
    }
    if (type == ItemType::Coin && coinTex) {
        sf::Sprite sprite(*coinTex);
        const int frameWidth = coinTex->getSize().x / Constants::COIN_FRAME_COUNT;
        const int frameHeight = coinTex->getSize().y;
        const int frame = static_cast<int>(age * 10.0f) % Constants::COIN_FRAME_COUNT;
        sprite.setTextureRect(sf::IntRect({frame * frameWidth, 0},
                                          {frameWidth, frameHeight}));
        sprite.setPosition(drawPos);
        window.draw(sprite);
        return;
    }
    if (type == ItemType::Block && solidTex) {
        sf::Sprite sprite(*solidTex);
        sprite.setScale({Constants::ITEM_ICON_SIZE / 32.0f, Constants::ITEM_ICON_SIZE / 32.0f});
        sprite.setPosition(drawPos);
        window.draw(sprite);
        return;
    }

    sf::RectangleShape shape(size);
    sf::Color fill = {241, 200, 76};
    if (type == ItemType::Food) fill = {114, 200, 106};
    if (type == ItemType::Heart) fill = {230, 93, 102};
    if (type == ItemType::Block) fill = {143, 164, 122};
    shape.setFillColor(fill);
    shape.setPosition(drawPos);
    window.draw(shape);
}

ItemType DroppedItem::getType() const { return type; }
int DroppedItem::getQuantity() const { return quantity; }
int DroppedItem::getFoodVariant() const { return foodVariant; }
