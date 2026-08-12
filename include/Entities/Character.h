#pragma once

#include "Entities/Entity.h"

// Base class for living entities that can move, jump, and take damage (Player, Enemy)
class Character : public Entity {
public:
    enum FacingDirection {
        LEFT,
        RIGHT
    };

protected:
    float health = 100.0f;
    float maxHealth = 100.0f;
    float gravity = 0.0f;
    bool isGrounded = false;
    FacingDirection facingDir = RIGHT;

public:
    virtual ~Character() = default;

    // Health management
    float getHealth() const { return health; }
    void setHealth(float newHealth) { health = newHealth; }

    float getMaxHealth() const { return maxHealth; }
    void setMaxHealth(float newMaxHealth) { maxHealth = newMaxHealth; }

    // Physics state
    float getGravity() const { return gravity; }
    void setGravity(float newGravity) { gravity = newGravity; }

    bool getIsGrounded() const { return isGrounded; }
    void setIsGrounded(bool grounded) { isGrounded = grounded; }

    // Direction state
    FacingDirection getFacingDirectionEnum() const { return facingDir; }
    void setFacingDirection(FacingDirection direction) { facingDir = direction; }
};