#include "Entities/Enemy.h"

// Constructor setting initial health, size, and bounds
Enemy::Enemy(const sf::Vector2f& startPosition) {
    position = startPosition;
    health = 100.0f;
    maxHealth = 100.0f;
    isAlive = true;
    
    body.setSize(sf::Vector2f(32.0f, 32.0f));
    body.setPosition(position);
    bounds = sf::FloatRect(sf::Vector2f(position.x, position.y), sf::Vector2f(32.0f, 32.0f));
}

// Subtracts damage from health and sets isAlive to false if health drops to 0
void Enemy::takeDamage(float amount) {
    health -= amount;
    if (health <= 0.0f) {
        health = 0.0f;
        isAlive = false;
    }
}

// Renders enemy shape to the window if currently alive
void Enemy::draw(sf::RenderWindow& window) {
    if (isAlive) {
        window.draw(body);
    }
}
