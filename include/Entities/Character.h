// Shared health, gravity, facing, and damage behavior for players and enemies.
#pragma once

#include "Entities/Entity.h"

class Character : public Entity {
public:
    /// Creates a character with default entity and health state.
    Character() = default;
    /// Creates a character with a transform and maximum health.
    Character(sf::Vector2f position, sf::Vector2f size, int maxHealth);
    /// Adds gravity acceleration while respecting the shared fall-speed limit.
    void applyGravity(float dt);
    /// Removes health and optionally applies horizontal knockback.
    virtual void takeDamage(int damage, float knockback = 0.0f);
    /// Reports whether health has reached zero.
    bool isDead() const;

    /// Returns current health.
    int getHealth() const;
    /// Returns maximum health.
    int getMaxHealth() const;

protected:
    int health = 1;
    int maxHealth = 1;
    float hurtTimer = 0.0f;
};

