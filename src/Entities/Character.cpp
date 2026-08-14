// Implements health and shared gravity/damage behavior for characters.
#include "Entities/Character.h"

#include "Utils/Constants.h"

#include <algorithm>

Character::Character(sf::Vector2f position, sf::Vector2f size, int maxHealth)
    : Entity(position, size), health(maxHealth), maxHealth(maxHealth) {}

void Character::applyGravity(float dt) {
    velocity.y = std::min(Constants::MAX_FALL_SPEED, velocity.y + Constants::GRAVITY * dt);
}

void Character::takeDamage(int damage, float knockback) {
    if (hurtTimer > 0.0f) return;
    health -= damage;
    hurtTimer = 0.65f;
    velocity.x += knockback;
    velocity.y = std::min(velocity.y, -220.0f);
}

bool Character::isDead() const {
    return health <= 0;
}

int Character::getHealth() const {
    return health;
}

int Character::getMaxHealth() const {
    return maxHealth;
}
