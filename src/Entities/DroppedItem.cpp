// Implements thrown/dropped item physics and procedural drawing.
#include "Entities/DroppedItem.h"

#include "Utils/Constants.h"
#include "World/Collision.h"
#include "World/TileMap.h"

#include <algorithm>
#include <cmath>
#include <random>

namespace {
float randomFloat(float low, float high) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(low, high);
    return dist(rng);
}
}

DroppedItem::DroppedItem(sf::Vector2f position, ItemType type, int quantity)
    : Entity(position, {20.0f, 20.0f}), type(type), quantity(quantity) {
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
    sf::RectangleShape shape(size);
    sf::Color fill = {241, 200, 76};
    if (type == ItemType::Food) fill = {114, 200, 106};
    if (type == ItemType::Heart) fill = {230, 93, 102};
    if (type == ItemType::Block) fill = {143, 164, 122};
    shape.setFillColor(fill);
    shape.setPosition(position - camera + sf::Vector2f(0.0f, std::sin(age * 7.0f) * 2.0f));
    window.draw(shape);
}

ItemType DroppedItem::getType() const { return type; }
int DroppedItem::getQuantity() const { return quantity; }
