// Implements base enemy damage, loot drop trigger, and common drawing helpers.
#include "Entities/Enemy.h"

#include "Entities/Player.h"
#include "World/Level.h"

Enemy::Enemy(sf::Vector2f position, sf::Vector2f size, int health, sf::Color color)
    : Character(position, size, health), bodyColor(color) {}

void Enemy::tick(float dt) {
    hitTimer = std::max(0.0f, hitTimer - dt);
}

void Enemy::takeDamage(int damage, Level& level, const Player& source) {
    if (hitTimer > 0.0f) return;
    health -= damage;
    hitTimer = 0.22f;
    velocity.x += static_cast<float>(source.getFacingDirection()) * 210.0f;
    velocity.y = -180.0f;
    if (health <= 0) {
        alive = false;
        level.dropLoot(position + size * 0.5f);
    }
}

void Enemy::addMaxHealth(int amount) {
    if (amount <= 0) return;
    maxHealth += amount;
    health += amount;
}

int Enemy::getDamage() const {
    return damage;
}

void Enemy::drawBody(sf::RenderWindow& window, sf::Vector2f camera) const {
    sf::RectangleShape body(size);
    body.setPosition(position - camera);
    body.setFillColor(bodyColor);
    window.draw(body);
}

void Enemy::drawHealthBar(sf::RenderWindow& window, sf::Vector2f camera) const {
    if (health >= maxHealth) return;
    sf::RectangleShape back({size.x, 4.0f});
    back.setPosition(position - camera + sf::Vector2f(0.0f, -8.0f));
    back.setFillColor({42, 29, 29});
    window.draw(back);

    sf::RectangleShape hp({size.x * static_cast<float>(health) / static_cast<float>(maxHealth), 4.0f});
    hp.setPosition(back.getPosition());
    hp.setFillColor({233, 106, 95});
    window.draw(hp);
}

