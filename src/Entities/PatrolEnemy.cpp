#include "Entities/PatrolEnemy.h"
#include "Graphics/EnemySprites.h"
#include "World/Collision.h"
#include "World/Level.h"
#include "Entities/Player.h"
#include "Utils/Constants.h"
#include <cmath>

PatrolEnemy::PatrolEnemy(sf::Vector2f pos)
    : Enemy(pos, {48.0f, 48.0f}, Constants::PATROL_ENEMY_HEALTH, {185, 104, 78}) {
    facingDirection = -1;
}

void PatrolEnemy::update(float dt, Level& level) {
    tick(dt);

    if (isDying) {
        deathTimer -= dt;
        velocity.x = 0.0f;
        applyGravity(dt);
        velocity *= dt;
        Collision::resolveTileCollision(*this, level.getTileMap());
        velocity /= dt;
        if (deathTimer <= 0.0f) {
            alive = false;
        }
        return;
    }

    animTime += dt;
    if (hitTimer <= 0.0f) {
        velocity.x = static_cast<float>(facingDirection) * 35.0f;
    }
    applyGravity(dt);
    velocity *= dt;
    Collision::resolveTileCollision(*this, level.getTileMap());
    velocity /= dt;
    if (velocity.x == 0.0f) facingDirection *= -1;
}

void PatrolEnemy::takeDamage(int damage, Level& level, const Player& source) {
    if (isDying || hitTimer > 0.0f) return;
    health -= damage;
    hitTimer = 0.25f;
    velocity.x += static_cast<float>(source.getFacingDirection()) * 160.0f;
    velocity.y = -140.0f;
    if (health <= 0) {
        isDying = true;
        deathTimer = 0.35f;
        level.dropLoot(position + size * 0.5f);
    }
}

int PatrolEnemy::getDamage() const {
    return isDying ? 0 : damage;
}

void PatrolEnemy::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    if (EnemyTextures::patrol) {
        sf::Sprite sprite(*EnemyTextures::patrol);
        int frameIndex = 0;
        if (isDying) {
            frameIndex = 3; // Frame 4: Death
        } else if (hitTimer > 0.0f) {
            frameIndex = 2; // Frame 3: Hurt / Hit
        } else {
            frameIndex = static_cast<int>(animTime * 6.0f) % 2; // Frame 1 & 2: Idle / Move
        }

        sprite.setTextureRect(sf::IntRect({frameIndex * 32, 0}, {32, 32}));
        sprite.setOrigin({16.0f, 0.0f});
        sprite.setScale({facingDirection > 0 ? -1.5f : 1.5f, 1.5f}); // Scale 1.5x
        sprite.setPosition({position.x - camera.x + 24.0f, position.y - camera.y});
        window.draw(sprite);
    } else {
        drawBody(window, camera);
    }
    if (!isDying) {
        drawHealthBar(window, camera);
    }
}
