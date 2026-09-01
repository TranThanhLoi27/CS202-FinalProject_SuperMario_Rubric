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

class Level {
public:
    bool loadFromFile(const std::string& path);
    bool loadDefault(const Player::Profile& p1Profile = Player::profiles()[0],
                     const Player::Profile& p2Profile = Player::profiles()[1],
                     int playerCount = 2,
                     int enemyHealthBonus = 0);
    bool loadLevel(const std::string& path,
                   const Player::Profile& p1Profile,
                   const Player::Profile& p2Profile,
                   int playerCount,
                   int enemyHealthBonus);
    void update(float dt, const InputState& p1, const InputState& p2);
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const;
    static void setTextures(const sf::Texture& solid,
                            const sf::Texture& goal,
                            const sf::Texture& spike,
                            const sf::Texture& fairy);

    Player* closestLivingPlayer(const Entity& entity);
    void addProjectile(std::unique_ptr<Projectile> projectile);
    void addDroppedItem(std::unique_ptr<DroppedItem> item);
    void addTombstone(std::unique_ptr<Tombstone> tombstone);
    void dropLoot(sf::Vector2f position);
    bool tryPlaceBlock(Player& player);
    void limitPlayerDistance();
    void eraseDeadObjects();
    bool hasWon() const;
    bool allDead() const;
    int collectedCoins() const;

    FairyCompanionManager& getFairies();
    const FairyCompanionManager& getFairies() const;

    TileMap& getTileMap();
    const TileMap& getTileMap() const;
    std::vector<std::unique_ptr<Player>>& getPlayers();
    const std::vector<std::unique_ptr<Player>>& getPlayers() const;
    std::vector<std::unique_ptr<Enemy>>& getEnemies();
    std::vector<std::unique_ptr<Projectile>>& getProjectiles();
    std::vector<std::unique_ptr<DroppedItem>>& getDroppedItems();
    std::vector<std::unique_ptr<Tombstone>>& getTombstones();

    std::vector<bool> getAchievements() const;
    void setAchievements(const std::vector<bool>& achievements);
    void resetAchievements();
    void checkAchievements();

private:
    void spawnFromMap();
    void updateActors(float dt, const InputState& p1, const InputState& p2);
    void handlePickups();
    void handleSpikes();
    void handleCheckpoints();
    void drawMarkers(sf::RenderWindow& window, sf::Vector2f camera) const;
    void constrainPlayerDistance(Player& moving, const Player& anchor);
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
};
