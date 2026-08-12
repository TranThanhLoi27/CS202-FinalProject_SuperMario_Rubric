#pragma once

#include "Entities/Character.h"
#include "Core/InputManager.h"
#include "Utils/Constants.h"
#include "Entities/Inventory.h"
#include "Entities/PlacedBlock.h"
#include <optional>
#include <vector>

// Player class representing Player 1 or Player 2 in game
class Player : public Character {
private:
    int playerId;
    float hunger = 100.0f;
    float maxHunger = 100.0f;
    sf::RectangleShape body;

    // Attack mechanics
    bool isAttacking = false;
    float attackTimer = 0.0f;
    float attackCooldownTimer = 0.0f;
    sf::RectangleShape attackHitboxVisual;

    // Phase 4: Inventory
    Inventory inventory;

    // Phase 4: Block Placing
    std::vector<PlacedBlock> pendingPlacedBlocks;

public:
    // Constructor specifying Player ID (1 or 2) and starting position
    Player(int id, const sf::Vector2f& startPosition = {0.0f, 0.0f});
    virtual ~Player() = default;

    // Default Entity update override (physics & hunger/health decay without input)
    void update(float dt) override;
    
    // Draw player body and attack hitbox if attacking
    void draw(sf::RenderWindow& window) override;

    // Main update method handling movement, jumping, attack input, and timers
    void update(float dt, const InputState& input);

    // Getters and Setters
    int getPlayerId() const { return playerId; }

    float getHunger() const { return hunger; }
    void setHunger(float newHunger) { hunger = newHunger; }

    float getMaxHunger() const { return maxHunger; }
    void setMaxHunger(float newMaxHunger) { maxHunger = newMaxHunger; }

    const sf::RectangleShape& getBody() const { return body; }

    // Phase 4: Access inventory
    Inventory& getInventory() { return inventory; }
    const Inventory& getInventory() const { return inventory; }

    // Health and Hunger restoration
    void takeDamage(int damage);
    void heal(int amount);
    void restoreHunger(int amount);

    // Attack methods
    void attack();
    sf::FloatRect getAttackHitbox() const;
    bool getIsAttacking() const { return isAttacking; }

    // Phase 4: Block placing logic
    std::optional<PlacedBlock> placeBlock();
    std::vector<PlacedBlock> getAndClearPendingBlocks();

    bool isDead() const;
    int getFacingDirection() const; // Returns -1 for LEFT, 1 for RIGHT
};
