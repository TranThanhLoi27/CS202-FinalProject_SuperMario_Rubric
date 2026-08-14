#include "Entities/Projectile.h"

#include "World/TileMap.h"

Projectile::Projectile(sf::Vector2f position, sf::Vector2f velocity, bool friendly)
    : Entity(position, {12.0f, 12.0f}), friendly(friendly) {
    this->velocity = velocity;
}

void Projectile::update(float dt, const TileMap& map) {
    life -= dt;
    position += velocity * dt;
    if (life <= 0.0f || map.isSolidAt(position + size * 0.5f)) alive = false;
}

void Projectile::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    sf::CircleShape shape(6.0f);
    shape.setPosition(position - camera);
    shape.setFillColor(friendly ? sf::Color(241, 209, 107) : sf::Color(216, 95, 159));
    window.draw(shape);
}

bool Projectile::isFriendly() const {
    return friendly;
}

