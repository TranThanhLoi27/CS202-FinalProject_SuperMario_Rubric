#pragma once

#include "Core/InputManager.h"
#include "Entities/Character.h"
#include "Entities/Inventory.h"
#include "Graphics/Animator.h"
#include "Graphics/CharacterSprites.h"

#include <string>
#include <vector>

class Level;

class Player : public Character {
public:
    struct Profile {
        std::string name;
        std::string skill;
        std::string description;
        std::string spriteId;
        sf::Color color;
        float speedMultiplier = 1.0f;
        float jumpMultiplier = 1.0f;
        float gravityMultiplier = 1.0f;
        float maxFallMultiplier = 1.0f;
        int maxHealth = 5;
        bool blocksFirstHit = false;
    };

    Player(int id, sf::Vector2f spawnPosition, Profile profile);
    void update(float dt, const InputState& input, Level& level);
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    void takeDamage(int damage, float knockback = 0.0f) override;
    void heal(int amount);
    void restoreHunger(int amount);
    void die(Level& level, bool fell = false);
    bool isRespawning() const;
    bool isDodging() const;
    bool hasCollectedOwnTombstone() const;
    sf::FloatRect attackBox() const;
    bool isAttacking() const;
    int getId() const;
    int getFacingDirection() const;
    float getHunger() const;
    float getRespawnTimer() const;
    const Profile& getProfile() const;
    Inventory& getInventory() { return inventory; }
    const Inventory& getInventory() const { return inventory; }
    sf::Vector2f getSpawn() const;
    void setSpawn(sf::Vector2f spawnPosition);
    void markTombstoneRecovered();
    bool hasDiedBefore() const;
    int getSelectedSlot() const;
    float getActionTimer() const;
    void useSelectedItem(Level& level);
    static const std::vector<Profile>& profiles();

    sf::Color color;
    sf::Vector2f lastSafePosition;

private:
    int id = 0;
    Inventory inventory;
    sf::Vector2f spawn;

    float hunger = 100.0f;
    float attackTimer = 0.0f;
    float dodgeTimer = 0.0f;
    float dodgeCooldownTimer = 0.0f;
    float respawnTimer = 0.0f;
    float hungerDamageTimer = 0.0f;
    float coyoteTimer = 0.0f;
    float jumpBufferTimer = 0.0f;

    Animator animator;
    const CharacterSpriteSet* sprites = nullptr;

    bool diedBefore = false;
    bool recoveredTombstone = false;
    bool firstHitGuardAvailable = false;

    Profile profile;

    void updateAnimation(float dt);
    void drawSprite(sf::RenderWindow& window, sf::Vector2f camera) const;
};
