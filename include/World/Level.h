#pragma once

#include "Entities/DroppedItem.h"
#include "Entities/Enemy.h"
#include "Entities/Player.h"
#include "Entities/Projectile.h"
#include "Entities/Tombstone.h"
#include "Entities/FairyCompanion.h"
#include "World/Checkpoint.h"
#include "World/GoalGate.h"
#include "World/SpikeTrap.h"
#include "World/TileMap.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

/// Gameplay facade that owns world actors and coordinates subsystem interactions.
class Level {
public:
    /// Parses a level file and replaces all current world data.
    bool loadFromFile(const std::string& path);
    /// Loads the default level with optional profiles, player count, and difficulty bonus.
    bool loadDefault(const Player::Profile& p1Profile = Player::profiles()[0],
                     const Player::Profile& p2Profile = Player::profiles()[1],
                     int playerCount = 2,
                     int enemyHealthBonus = 0);
    /// Stores the requested runtime settings and loads a specific level file.
    bool loadLevel(const std::string& path,
                   const Player::Profile& p1Profile,
                   const Player::Profile& p2Profile,
                   int playerCount,
                   int enemyHealthBonus);
    /// Updates actors, combat, pickups, hazards, checkpoints, and cleanup.
    void update(float dt, const InputState& p1, const InputState& p2);
    /// Draws the background, map, actors, effects, and level markers.
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const;
    /// Registers all shared world textures owned by AssetManager.
    static void setTextures(const sf::Texture& solid,
                            const sf::Texture& goal,
                            const sf::Texture& spike,
                            const sf::Texture& checkpoint,
                            const sf::Texture& fairy,
                            const sf::Texture& background);

    /// Finds the closest player that is not currently respawning.
    Player* closestLivingPlayer(const Entity& entity);
    /// Transfers ownership of a projectile into the level.
    void addProjectile(std::unique_ptr<Projectile> projectile);
    /// Transfers ownership of a dropped item into the level.
    void addDroppedItem(std::unique_ptr<DroppedItem> item);
    /// Transfers ownership of a tombstone into the level.
    void addTombstone(std::unique_ptr<Tombstone> tombstone);
    /// Creates randomized enemy loot at a world position.
    void dropLoot(sf::Vector2f position);
    /// Delegates validated block placement to BlockPlacement.
    bool tryPlaceBlock(Player& player);
    /// Keeps two active players within the shared-camera distance.
    void limitPlayerDistance();
    /// Removes dead enemies and expired transient objects.
    void eraseDeadObjects();
    /// Reports whether the active players satisfy the cooperative goal rule.
    bool hasWon() const;
    /// Reports whether the loss condition is active.
    bool allDead() const;
    /// Returns the total coins currently carried by active players.
    int collectedCoins() const;

    /// Returns mutable access to fairy visual effects.
    FairyCompanionManager& getFairies();
    /// Returns read-only access to fairy visual effects.
    const FairyCompanionManager& getFairies() const;

    /// Returns mutable access to the tile map.
    TileMap& getTileMap();
    /// Returns read-only access to the tile map.
    const TileMap& getTileMap() const;
    /// Returns mutable access to level-owned players.
    std::vector<std::unique_ptr<Player>>& getPlayers();
    /// Returns read-only access to level-owned players.
    const std::vector<std::unique_ptr<Player>>& getPlayers() const;
    /// Returns mutable access to level-owned enemies.
    std::vector<std::unique_ptr<Enemy>>& getEnemies();
    /// Returns mutable access to level-owned projectiles.
    std::vector<std::unique_ptr<Projectile>>& getProjectiles();
    /// Returns mutable access to level-owned dropped items.
    std::vector<std::unique_ptr<DroppedItem>>& getDroppedItems();
    /// Returns mutable access to level-owned tombstones.
    std::vector<std::unique_ptr<Tombstone>>& getTombstones();

    /// Returns a copy of achievement completion flags.
    std::vector<bool> getAchievements() const;
    /// Restores achievement flags from persistent data.
    void setAchievements(const std::vector<bool>& achievements);
    /// Clears achievement progress.
    void resetAchievements();
    /// Evaluates achievement conditions against the current level state.
    void checkAchievements();

private:
    /// Recreates players and Factory-produced enemies from parsed spawn data.
    void spawnFromMap();
    /// Updates each owned player, enemy, projectile, item, tombstone, and fairy.
    void updateActors(float dt, const InputState& p1, const InputState& p2);
    /// Moves overlapping dropped or tombstone items into player inventories.
    void handlePickups();
    /// Applies spike damage to vulnerable overlapping players.
    void handleSpikes();
    /// Activates checkpoints and updates player spawn positions.
    void handleCheckpoints();
    /// Draws goals, checkpoints, and their visual indicators.
    void drawMarkers(sf::RenderWindow& window, sf::Vector2f camera) const;
    /// Corrects one player's position relative to the other player.
    void constrainPlayerDistance(Player& moving, const Player& anchor);
    /// Draws the parallax background behind the level.
    void drawBackground(sf::RenderWindow& window, sf::Vector2f camera) const;

    TileMap map;
    std::vector<SpikeTrap> spikes;
    std::vector<Checkpoint> checkpoints;
    std::vector<GoalGate> goals;
    std::vector<sf::Vector2f> playerSpawns;
    std::vector<std::pair<char, sf::Vector2f>> enemySpawns;
    Player::Profile pendingProfiles[2] = {Player::profiles()[0], Player::profiles()[1]};
    int activePlayerCount = 2;
    int pendingEnemyHealthBonus = 0;
    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::vector<std::unique_ptr<DroppedItem>> droppedItems;
    std::vector<std::unique_ptr<Tombstone>> tombstones;
    FairyCompanionManager fairies;

    std::vector<bool> achievements; // [0] first kill, [1] 5 kills, [2] no damage run, [3] boss defeated
    int totalKills = 0;
    bool levelStartedWithDamage = false;
    bool bossDefeated = false;

    static const sf::Texture* solidTexture;
    static const sf::Texture* goalTexture;
    static const sf::Texture* spikeTexture;
    static const sf::Texture* checkpointTexture;
    static const sf::Texture* backgroundTexture;
};
