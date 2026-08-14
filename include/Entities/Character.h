// Shared health, gravity, facing, and damage behavior for players and enemies.
#pragma once

#include "Entities/Entity.h"

class Character : public Entity {
public:
    Character() = default;
    Character(sf::Vector2f position, sf::Vector2f size, int maxHealth);
    void applyGravity(float dt);
    virtual void takeDamage(int damage, float knockback = 0.0f);
    bool isDead() const;

    int getHealth() const;
    int getMaxHealth() const;

protected:
    int health = 1;
    int maxHealth = 1;
    float hurtTimer = 0.0f;
};

