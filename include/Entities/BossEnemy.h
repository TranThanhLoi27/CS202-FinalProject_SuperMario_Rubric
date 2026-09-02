#pragma once

#include "Entities/Enemy.h"

/// Larger enemy whose State-pattern phase changes movement and attack behavior.
class BossEnemy : public Enemy {
public:
    /// Creates the boss in its anchored phase-one state.
    explicit BossEnemy(sf::Vector2f position);
    /// Updates phase transitions, tracking, movement, attacks, collision, and death.
    void update(float dt, Level& level) override;
    /// Draws the phase-appropriate boss animation and health bar.
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    /// Applies damage and hurt feedback while enforcing knockback immunity.
    void takeDamage(int damage, Level& level, const Player& source) override;
    /// Returns zero while dying and normal boss contact damage otherwise.
    int getDamage() const override;

private:
    enum class Phase {
        PhaseOne,
        Enraged
    };

    /// Transitions from phase one to Enraged at the health threshold.
    void updatePhase();
    /// Returns movement speed for the current State-pattern phase.
    float movementSpeed() const;
    /// Returns attack delay for the current State-pattern phase.
    float attackCooldown() const;
    /// Emits the projectile pattern associated with the current phase.
    void fireAttack(Level& level);

    Phase phase = Phase::PhaseOne;
    int movementDirection = -1;
    float shootCooldown = 0.0f;
    float animTime = 0.0f;
    float deathTimer = 0.0f;
    bool isDying = false;
    bool isAttacking = false;
    bool hasFired = false;
};
