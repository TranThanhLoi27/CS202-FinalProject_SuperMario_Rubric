#include "World/Level.h"

#include "Combat/DamageSystem.h"
#include "Combat/LootTable.h"
#include "Entities/BossEnemy.h"
#include "Entities/PatrolEnemy.h"
#include "Entities/ShooterEnemy.h"
#include "Utils/Constants.h"
#include "Utils/MathUtils.h"
#include "World/BlockPlacement.h"
#include "World/Collision.h"
#include "World/LevelLoader.h"

#include <algorithm>
#include <cmath>
#include <utility>

bool Level::loadFromFile(const std::string& path) {
    LevelData data;
    if (!LevelLoader().load(path, data)) return false;

    map.setTiles(std::move(data.solidTiles));
    spikes = std::move(data.spikes);
    checkpoints = std::move(data.checkpoints);
    goals = std::move(data.goals);
    playerSpawns = std::move(data.playerSpawns);
    enemySpawns = std::move(data.enemySpawns);
    spawnFromMap();
    return true;
}

bool Level::loadDefault(const Player::Profile& p1Profile, const Player::Profile& p2Profile, int playerCount, int enemyHealthBonus) {
    return loadLevel("assets/levels/level1.txt", p1Profile, p2Profile, playerCount, enemyHealthBonus) ||
           loadLevel("../assets/levels/level1.txt", p1Profile, p2Profile, playerCount, enemyHealthBonus);
}

bool Level::loadLevel(const std::string& path,
                      const Player::Profile& p1Profile,
                      const Player::Profile& p2Profile,
                      int playerCount,
                      int enemyHealthBonus) {
    pendingProfiles[0] = p1Profile;
    pendingProfiles[1] = p2Profile;
    activePlayerCount = std::clamp(playerCount, 1, 2);
    pendingEnemyHealthBonus = std::max(0, enemyHealthBonus);
    return loadFromFile(path);
}

void Level::spawnFromMap() {
    players.clear();
    enemies.clear();
    projectiles.clear();
    droppedItems.clear();
    tombstones.clear();

    players.push_back(std::make_unique<Player>(1, playerSpawns[0], pendingProfiles[0]));
    if (activePlayerCount >= 2) {
        players.push_back(std::make_unique<Player>(2, playerSpawns[1], pendingProfiles[1]));
    }

    for (const auto& spawn : enemySpawns) {
        if (spawn.first == 'P') enemies.push_back(std::make_unique<PatrolEnemy>(spawn.second));
        if (spawn.first == 'R') enemies.push_back(std::make_unique<ShooterEnemy>(spawn.second));
        if (spawn.first == 'B') enemies.push_back(std::make_unique<BossEnemy>(spawn.second + sf::Vector2f(0.0f, -46.0f)));
        if (!enemies.empty()) enemies.back()->addMaxHealth(pendingEnemyHealthBonus);
    }
}

void Level::update(float dt, const InputState& p1, const InputState& p2) {
    updateActors(dt, p1, p2);
    DamageSystem::handlePlayerAttacks(*this);
    DamageSystem::handleEnemyContact(*this);
    DamageSystem::handleProjectiles(*this);
    handlePickups();
    handleSpikes();
    handleCheckpoints();
    for (auto& player : players) {
        if (player->isDead() && !player->isRespawning()) player->die(*this);
    }
    eraseDeadObjects();
}

void Level::updateActors(float dt, const InputState& p1, const InputState& p2) {
    players[0]->update(dt, p1, *this);
    if (players.size() > 1) {
        constrainPlayerDistance(*players[0], *players[1]);
        players[1]->update(dt, p2, *this);
        constrainPlayerDistance(*players[1], *players[0]);
    }
    for (auto& enemy : enemies) enemy->update(dt, *this);
    for (auto& projectile : projectiles) projectile->update(dt, map);
    for (auto& item : droppedItems) item->update(dt, map);
    for (auto& tombstone : tombstones) tombstone->update(dt, map);
}

void Level::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    drawBackground(window, camera);
    map.draw(window, camera);
    drawMarkers(window, camera);
    for (const auto& item : droppedItems) item->draw(window, camera);
    for (const auto& tombstone : tombstones) tombstone->draw(window, camera);
    for (const auto& enemy : enemies) enemy->draw(window, camera);
    for (const auto& projectile : projectiles) projectile->draw(window, camera);
    for (const auto& player : players) player->draw(window, camera);
}

Player* Level::closestLivingPlayer(const Entity& entity) {
    Player* best = nullptr;
    float bestDist = 999999999.0f;
    for (auto& player : players) {
        if (player->isRespawning()) continue;
        const sf::Vector2f delta = player->position - entity.position;
        const float dist = delta.x * delta.x + delta.y * delta.y;
        if (dist < bestDist) {
            bestDist = dist;
            best = player.get();
        }
    }
    return best;
}

void Level::addProjectile(std::unique_ptr<Projectile> projectile) {
    projectiles.push_back(std::move(projectile));
}

void Level::addDroppedItem(std::unique_ptr<DroppedItem> item) {
    droppedItems.push_back(std::move(item));
}

void Level::addTombstone(std::unique_ptr<Tombstone> tombstone) {
    tombstones.push_back(std::move(tombstone));
}

void Level::dropLoot(sf::Vector2f position) {
    const ItemType type = LootTable::roll();
    const int quantity = type == ItemType::Coin ? 2 : 1;
    addDroppedItem(std::make_unique<DroppedItem>(position, type, quantity));
}

void Level::tryPlaceBlock(Player& player) {
    BlockPlacement::tryPlaceBlock(player, map, players, enemies, droppedItems, tombstones);
}

void Level::throwItem(Player& player) {
    const ItemType order[] = {ItemType::Block, ItemType::Food, ItemType::Heart, ItemType::Coin};
    for (ItemType type : order) {
        if (player.getInventory().count(type) <= 0) continue;
        player.getInventory().remove(type);
        auto item = std::make_unique<DroppedItem>(
            player.position + sf::Vector2f(static_cast<float>(player.getFacingDirection()) * 28.0f, 10.0f),
            type,
            1
        );
        item->velocity = {static_cast<float>(player.getFacingDirection()) * 330.0f, -240.0f};
        addDroppedItem(std::move(item));
        return;
    }
}

void Level::limitPlayerDistance() {
    if (players.size() < 2) return;
    constrainPlayerDistance(*players[0], *players[1]);
    constrainPlayerDistance(*players[1], *players[0]);
}

void Level::constrainPlayerDistance(Player& moving, const Player& anchor) {
    if (moving.isRespawning() || anchor.isRespawning()) return;

    const sf::Vector2f movingCenter = moving.position + moving.size * 0.5f;
    const sf::Vector2f anchorCenter = anchor.position + anchor.size * 0.5f;
    const sf::Vector2f fromAnchor = movingCenter - anchorCenter;
    const float distance = std::sqrt(fromAnchor.x * fromAnchor.x + fromAnchor.y * fromAnchor.y);
    if (distance <= Constants::MAX_PLAYER_DISTANCE || distance <= 0.0f) return;

    const sf::Vector2f direction = fromAnchor / distance;
    const sf::Vector2f targetPosition = anchorCenter + direction * Constants::MAX_PLAYER_DISTANCE - moving.size * 0.5f;
    const sf::Vector2f correction = targetPosition - moving.position;
    const sf::Vector2f originalVelocity = moving.velocity;
    const bool wasOnGround = moving.onGround;

    moving.velocity = correction;
    Collision::resolveTileCollision(moving, map);
    moving.onGround = wasOnGround || moving.onGround;
    moving.velocity = originalVelocity;

    const float outwardSpeed = moving.velocity.x * direction.x + moving.velocity.y * direction.y;
    if (outwardSpeed > 0.0f) {
        moving.velocity -= direction * outwardSpeed;
    }
}

void Level::handlePickups() {
    for (auto& player : players) {
        if (player->isRespawning()) continue;
        for (auto& item : droppedItems) {
            if (!item->isAlive() || !MathUtils::intersects(player->getBounds(), item->getBounds())) continue;
            player->getInventory().add(item->getType(), item->getQuantity());
            if (item->getType() == ItemType::Food) player->restoreHunger(28 * item->getQuantity());
            if (item->getType() == ItemType::Heart) player->heal(item->getQuantity());
            item->kill();
        }
        for (auto& tombstone : tombstones) {
            if (!tombstone->isAlive() || !MathUtils::intersects(player->getBounds(), tombstone->getBounds())) continue;
            player->getInventory().merge(tombstone->getInventory());
            for (auto& owner : players) {
                if (owner->getId() == tombstone->getOwnerId()) owner->markTombstoneRecovered();
            }
            tombstone->kill();
        }
    }
}

void Level::handleSpikes() {
    for (auto& player : players) {
        if (player->isRespawning()) continue;
        for (const auto& spike : spikes) {
            if (!MathUtils::intersects(player->getBounds(), spike.bounds)) continue;
            player->takeDamage(spike.damage, player->position.x < spike.bounds.position.x ? -170.0f : 170.0f);
        }
    }
}

void Level::handleCheckpoints() {
    for (auto& player : players) {
        if (player->isRespawning()) continue;
        for (const auto& checkpoint : checkpoints) {
            if (!MathUtils::intersects(player->getBounds(), checkpoint.bounds)) continue;
            player->setSpawn({checkpoint.bounds.position.x, checkpoint.bounds.position.y - player->size.y});
        }
    }
}

void Level::eraseDeadObjects() {
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const auto& e) { return !e->isAlive(); }), enemies.end());
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](const auto& p) { return !p->isAlive(); }), projectiles.end());
    droppedItems.erase(std::remove_if(droppedItems.begin(), droppedItems.end(), [](const auto& i) { return !i->isAlive(); }), droppedItems.end());
    tombstones.erase(std::remove_if(tombstones.begin(), tombstones.end(), [](const auto& t) { return !t->isAlive(); }), tombstones.end());
}

bool Level::hasWon() const {
    const auto inGoal = [&](const Player& player) {
        return std::any_of(goals.begin(), goals.end(), [&](const GoalGate& goal) {
            return MathUtils::intersects(player.getBounds(), goal.bounds);
        });
    };
    const bool allActivePlayersInGoal = std::all_of(players.begin(), players.end(), [&](const auto& player) {
        return !player->isRespawning() && inGoal(*player);
    });
    if (allActivePlayersInGoal) return true;
    if (players.size() < 2) return false;
    const bool p1Alive = !players[0]->isRespawning();
    const bool p2Alive = !players[1]->isRespawning();
    if (tombstones.empty() &&
        ((players[0]->hasDiedBefore() && players[0]->hasCollectedOwnTombstone()) ||
         (players[1]->hasDiedBefore() && players[1]->hasCollectedOwnTombstone()))) {
        return (p1Alive && inGoal(*players[0])) || (p2Alive && inGoal(*players[1]));
    }
    return false;
}

bool Level::allDead() const {
    return std::all_of(players.begin(), players.end(), [](const auto& p) { return p->isRespawning(); });
}

int Level::collectedCoins() const {
    int total = 0;
    for (const auto& player : players) total += player->getInventory().coin;
    return total;
}

TileMap& Level::getTileMap() { return map; }
const TileMap& Level::getTileMap() const { return map; }
std::vector<std::unique_ptr<Player>>& Level::getPlayers() { return players; }
const std::vector<std::unique_ptr<Player>>& Level::getPlayers() const { return players; }
std::vector<std::unique_ptr<Enemy>>& Level::getEnemies() { return enemies; }
std::vector<std::unique_ptr<Projectile>>& Level::getProjectiles() { return projectiles; }
std::vector<std::unique_ptr<DroppedItem>>& Level::getDroppedItems() { return droppedItems; }
std::vector<std::unique_ptr<Tombstone>>& Level::getTombstones() { return tombstones; }

void Level::drawMarkers(sf::RenderWindow& window, sf::Vector2f camera) const {
    Draw(spikes, window, camera);
    Draw(goals, window, camera);
    for (const auto& checkpoint : checkpoints) {
        sf::RectangleShape pole({5.0f, 60.0f});
        pole.setFillColor({217, 198, 111});
        pole.setPosition({checkpoint.bounds.position.x + 12.0f - camera.x, checkpoint.bounds.position.y - 28.0f - camera.y});
        window.draw(pole);

        sf::ConvexShape flag(3);
        const sf::Vector2f p(checkpoint.bounds.position.x + 17.0f - camera.x, checkpoint.bounds.position.y - 24.0f - camera.y);
        flag.setPoint(0, p);
        flag.setPoint(1, p + sf::Vector2f(28.0f, 9.0f));
        flag.setPoint(2, p + sf::Vector2f(0.0f, 22.0f));
        flag.setFillColor({114, 216, 168});
        window.draw(flag);
    }
}

void Level::drawBackground(sf::RenderWindow& window, sf::Vector2f camera) const {
    sf::RectangleShape sky(window.getView().getSize());
    sky.setFillColor({28, 46, 55});
    window.draw(sky);
    for (int i = 0; i < 26; ++i) {
        sf::ConvexShape tree(3);
        const float x = std::fmod(i * 230.0f - camera.x * 0.22f, window.getView().getSize().x + 260.0f) - 140.0f;
        const float base = window.getView().getSize().y - 120.0f - (i % 5) * 16.0f;
        tree.setPoint(0, {x, base});
        tree.setPoint(1, {x + 55.0f, base - 150.0f - (i % 3) * 22.0f});
        tree.setPoint(2, {x + 110.0f, base});
        tree.setFillColor({88, 122, 99, 82});
        window.draw(tree);
    }
}
