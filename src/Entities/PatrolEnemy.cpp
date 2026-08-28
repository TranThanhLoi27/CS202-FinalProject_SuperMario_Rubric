#include "Entities/PatrolEnemy.h"
#include "Graphics/EnemySprites.h"
#include "World/Collision.h"
#include "World/Level.h"
#include "Entities/Player.h"
#include "Utils/Constants.h"
#include <cmath>

PatrolEnemy::PatrolEnemy(sf::Vector2f pos)
    : Enemy(pos, {64.0f, 64.0f}, Constants::PATROL_ENEMY_HEALTH, {185, 104, 78}) {
    animator.play(EnemyTextures::patrolWalkAnim);
}

void PatrolEnemy::update(float dt, Level& level) {
    tick(dt);

    if (isDying) {
        animator.play(EnemyTextures::patrolDieAnim);
        animator.update(dt);
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

    animator.play(EnemyTextures::patrolWalkAnim);
    animator.update(dt);

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
        animator.play(EnemyTextures::patrolDieAnim);
        level.dropLoot(position + size * 0.5f);
    }
}

int PatrolEnemy::getDamage() const {
    return isDying ? 0 : damage;
}

void PatrolEnemy::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    if (EnemyTextures::patrol) {
        sf::Sprite sprite(*EnemyTextures::patrol);
        sprite.setTextureRect(animator.getFrameRect());
        sprite.setOrigin({16.0f, 32.0f});
        const float scale = size.y / 32.0f; // Scale frame (32x32) to match size (64x64, 2x scale)
        sprite.setScale({facingDirection > 0 ? scale : -scale, scale});
        sprite.setPosition({position.x - camera.x + size.x * 0.5f, position.y - camera.y + size.y});
        window.draw(sprite);
    } else {
        drawBody(window, camera);
    }
    if (!isDying) {
        drawHealthBar(window, camera);
    }
}
