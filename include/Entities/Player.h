#pragma once

#include "Core/InputManager.h"
#include "Entities/Character.h"
#include "Entities/Inventory.h"
#include "Graphics/Animator.h"
#include "Graphics/CharacterSprites.h"

#include <functional>
#include <string>
#include <vector>

class Level;
class SoundObserver;

class Player : public Character {
public:
    using SoundCallback = std::function<void(const std::string&)>;

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

    /// Creates a player with an identity, spawn point, and character strategy profile.
    Player(int id, sf::Vector2f spawnPosition, Profile profile);
    /// Updates movement, survival, combat, inventory, and animation state.
    void update(float dt, const InputState& input, Level& level);
    /// Draws the player sprite and temporary visual effects.
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    /// Applies damage unless respawning, dodging, or protected by the first-hit guard.
    void takeDamage(int damage, float knockback = 0.0f) override;
    /// Restores health without exceeding the profile maximum.
    void heal(int amount);
    /// Restores hunger without exceeding the shared maximum.
    void restoreHunger(float amount);
    /// Records a tile as player-created so it can be reclaimed safely.
    void recordPlacedBlock(int tileX, int tileY);
    /// Publishes the item-pickup sound event.
    void playPickupSound() const;
    /// Stores inventory in a tombstone and begins the respawn delay.
    void die(Level& level, bool fell = false);
    /// Reports whether the player is waiting to respawn.
    bool isRespawning() const;
    /// Reports whether the dodge invulnerability window is active.
    bool isDodging() const;
    /// Reports whether this player recovered the inventory from their tombstone.
    bool hasCollectedOwnTombstone() const;
    /// Returns the current melee attack area in world coordinates.
    sf::FloatRect attackBox() const;
    /// Reports whether the melee attack window is active.
    bool isAttacking() const;
    /// Returns the stable player identifier.
    int getId() const;
    /// Returns minus one for left or one for right.
    int getFacingDirection() const;
    /// Returns current hunger.
    float getHunger() const;
    /// Returns seconds remaining before respawn.
    float getRespawnTimer() const;
    /// Returns the selected character profile.
    const Profile& getProfile() const;
    /// Returns mutable access to the player's inventory.
    Inventory& getInventory() { return inventory; }
    /// Returns read-only access to the player's inventory.
    const Inventory& getInventory() const { return inventory; }
    /// Returns the active spawn or checkpoint position.
    sf::Vector2f getSpawn() const;
    /// Updates both the respawn point and the safe fallback position.
    void setSpawn(sf::Vector2f spawnPosition);
    /// Marks this player's tombstone inventory as recovered.
    void markTombstoneRecovered();
    /// Reports whether this player has died during the current level.
    bool hasDiedBefore() const;
    /// Returns the currently selected inventory slot.
    int getSelectedSlot() const;
    /// Returns the remaining inventory action cooldown.
    float getActionTimer() const;
    /// Consumes, throws, or places the item in the selected slot.
    void useSelectedItem(Level& level);
    /// Registers the preferred gameplay-sound observer.
    static void setSoundObserver(SoundObserver* observer);
    /// Registers a legacy callback when no observer is connected.
    static void setSoundCallback(SoundCallback callback);
    /// Returns the immutable catalog of selectable character strategies.
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
    float starvationDamageAccumulator = 0.0f;
    float coyoteTimer = 0.0f;
    float jumpBufferTimer = 0.0f;

    std::vector<sf::Vector2i> placedBlockTiles;

    Animator animator;
    const CharacterSpriteSet* sprites = nullptr;

    bool diedBefore = false;
    bool recoveredTombstone = false;
    bool firstHitGuardAvailable = false;

    Profile profile;

    /// Publishes a logical sound event without depending on AudioManager.
    void playSound(const std::string& soundId) const;
    /// Reclaims a tracked player block directly in front of the player.
    bool tryReclaimPlacedBlock(Level& level);
    /// Chooses and advances the animation matching current player state.
    void updateAnimation(float dt);
    /// Draws the current animation frame with correct orientation.
    void drawSprite(sf::RenderWindow& window, sf::Vector2f camera) const;
};
