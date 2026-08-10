#include "Entities/Player.h"
#include <algorithm>

// Initializes player stats, shape color based on Player ID, and default state
Player::Player(int id, const sf::Vector2f& startPosition)
    : playerId(id)
{
    position = startPosition;
    velocity = sf::Vector2f(0.0f, 0.0f);
    bounds = sf::FloatRect(position, sf::Vector2f(32.0f, 64.0f));
    isAlive = true;

    health = 100.0f;
    maxHealth = 100.0f;
    gravity = Constants::GRAVITY;
    isGrounded = false;
    facingDir = RIGHT;

    hunger = 100.0f;
    maxHunger = 100.0f;

    body.setSize(sf::Vector2f(32.0f, 64.0f));
    if (playerId == 1) {
        body.setFillColor(sf::Color::Red);
    } else {
        body.setFillColor(sf::Color::Blue);
    }
    body.setPosition(position);

    // Visual indicator for attack hitbox (transparent yellow)
    attackHitboxVisual.setFillColor(sf::Color(255, 255, 0, 128));
}

// Basic update when no player input is supplied
void Player::update(float dt) {
    velocity.y += gravity * dt;
    position += velocity * dt;
    bounds.position = position;
    body.setPosition(position);

    // Hunger decays by 1 point per second
    if (hunger > 0.0f) {
        hunger -= 1.0f * dt;
        if (hunger < 0.0f) hunger = 0.0f;
    }
    // Starvation causes health to decay by 2 points per second
    if (hunger <= 0.0f) {
        health -= 2.0f * dt;
        if (health <= 0.0f) {
            health = 0.0f;
            kill();
        }
    }

    // Update attack timers
    if (attackCooldownTimer > 0.0f) {
        attackCooldownTimer -= dt;
    }
    if (isAttacking) {
        attackTimer -= dt;
        if (attackTimer <= 0.0f) {
            isAttacking = false;
        } else {
            sf::FloatRect hitbox = getAttackHitbox();
            attackHitboxVisual.setPosition(hitbox.position);
            attackHitboxVisual.setSize(hitbox.size);
        }
    }
}

// Comprehensive update function handling inputs, movement physics, jumping, attacks, and status decay
void Player::update(float dt, const InputState& input) {
    // 1. Smooth horizontal movement with acceleration and friction
    float targetSpeed = 0.0f;
    if (input.moveLeft.isDown) {
        targetSpeed = -Constants::PLAYER_SPEED;
        facingDir = LEFT;
    } else if (input.moveRight.isDown) {
        targetSpeed = Constants::PLAYER_SPEED;
        facingDir = RIGHT;
    }

    const float acceleration = 1200.0f;
    const float friction = 1000.0f;

    if (targetSpeed != 0.0f) {
        if (velocity.x < targetSpeed) {
            velocity.x = std::min(velocity.x + acceleration * dt, targetSpeed);
        } else if (velocity.x > targetSpeed) {
            velocity.x = std::max(velocity.x - acceleration * dt, targetSpeed);
        }
    } else {
        if (velocity.x > 0.0f) {
            velocity.x = std::max(velocity.x - friction * dt, 0.0f);
        } else if (velocity.x < 0.0f) {
            velocity.x = std::min(velocity.x + friction * dt, 0.0f);
        }
    }

    // 2. Jump handling
    if ((input.jump.isDown || input.jump.isPressed) && isGrounded) {
        velocity.y = Constants::PLAYER_JUMP_SPEED;
        isGrounded = false;
    }

    // 3. Apply gravity
    velocity.y += gravity * dt;
    if (velocity.y > 1000.0f) {
        velocity.y = 1000.0f;
    }

    // 4. Update Position & Bounds
    position += velocity * dt;
    bounds.position = position;

    // 5. Update visual shape position
    body.setPosition(position);

    // 6. Update attack state and timers
    if (attackCooldownTimer > 0.0f) {
        attackCooldownTimer -= dt;
    }
    
    if (input.attack.isPressed) {
        attack();
    }

    if (isAttacking) {
        attackTimer -= dt;
        if (attackTimer <= 0.0f) {
            isAttacking = false;
        } else {
            sf::FloatRect hitbox = getAttackHitbox();
            attackHitboxVisual.setPosition(hitbox.position);
            attackHitboxVisual.setSize(hitbox.size);
        }
    }

    // 7. Hunger and starvation health decay
    if (hunger > 0.0f) {
        hunger -= 1.0f * dt;
        if (hunger < 0.0f) hunger = 0.0f;
    }

    if (hunger <= 0.0f) {
        health -= 2.0f * dt;
        if (health <= 0.0f) {
            health = 0.0f;
            kill();
        }
    }
}

// Renders the player shape and attack visual if currently attacking
void Player::draw(sf::RenderWindow& window) {
    if (getIsAlive()) {
        window.draw(body);
        if (isAttacking) {
            window.draw(attackHitboxVisual);
        }
    }
}

// Reduces player health when damaged
void Player::takeDamage(int damage) {
    health -= static_cast<float>(damage);
    if (health <= 0.0f) {
        health = 0.0f;
        kill();
    }
}

// Restores player health up to maxHealth
void Player::heal(int amount) {
    health = std::min(health + static_cast<float>(amount), maxHealth);
}

// Restores hunger points up to maxHunger
void Player::restoreHunger(int amount) {
    hunger = std::min(hunger + static_cast<float>(amount), maxHunger);
}

// Check if player is dead
bool Player::isDead() const {
    return !getIsAlive();
}

// Returns direction as an integer (-1 for Left, 1 for Right)
int Player::getFacingDirection() const {
    return (facingDir == LEFT) ? -1 : 1;
}

// Triggers an attack swing if not on cooldown
void Player::attack() {
    if (attackCooldownTimer <= 0.0f) {
        isAttacking = true;
        attackTimer = 0.2f;
        attackCooldownTimer = 0.5f;
    }
}

// Calculates attack hitbox rectangle in front of the player
sf::FloatRect Player::getAttackHitbox() const {
    float hitboxWidth = 40.0f;
    float hitboxHeight = 64.0f;
    
    float xPos = position.x;
    if (facingDir == RIGHT) {
        xPos += 32.0f; // Position right in front of player
    } else {
        xPos -= hitboxWidth; // Position to the left of player
    }
    
    return sf::FloatRect(sf::Vector2f(xPos, position.y), sf::Vector2f(hitboxWidth, hitboxHeight));
}
