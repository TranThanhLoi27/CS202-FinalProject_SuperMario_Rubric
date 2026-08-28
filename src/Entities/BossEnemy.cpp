// Implements simple boss patrol and multi-projectile attack loop.
#include "Entities/BossEnemy.h"

#include "Entities/Player.h"
#include "Entities/Projectile.h"
#include "Graphics/EnemySprites.h"
#include "Utils/Constants.h"
#include "World/Collision.h"
#include "World/Level.h"

#include <algorithm>
#include <cmath>
#include <memory>

BossEnemy::BossEnemy(sf::Vector2f position)
    : Enemy(position, {180.0f, 128.0f}, Constants::BOSS_HEALTH, {142, 65, 77}) {
    facingDirection = -1;
    animator.play(EnemyTextures::bossIdleAnim);
}

void BossEnemy::update(float dt, Level& level) {
    tick(dt);
    
    if (isDying) {
        animator.play(EnemyTextures::bossDieAnim);
        animator.update(dt);
        deathTimer -= dt;
        if (deathTimer <= 0.0f) {
            alive = false;
        }
        return;
    }

    if (hitTimer > 0.0f) {
        animator.play(EnemyTextures::bossHurtAnim);
        animator.update(dt);
        applyGravity(dt);
        const float beforeX = position.x;
        velocity *= dt;
        Collision::resolveTileCollision(*this, level.getTileMap());
        velocity /= dt;
        return;
    }

    if (isAttacking) {
        animator.play(EnemyTextures::bossAttackAnim);
        animator.update(dt);
        applyGravity(dt);
        const float beforeX = position.x;
        velocity.x = 0; // stop moving
        velocity *= dt;
        Collision::resolveTileCollision(*this, level.getTileMap());
        velocity /= dt;

        int frameIndex = animator.getFrame();
        if (frameIndex == 7 && !hasFired) {
            hasFired = true;
            level.addProjectile(std::make_unique<Projectile>(
                position + sf::Vector2f(size.x * 0.5f, 48.0f),
                sf::Vector2f(static_cast<float>(facingDirection) * 430.0f, -60.0f),
                false, true
            ));
            level.addProjectile(std::make_unique<Projectile>(
                position + sf::Vector2f(size.x * 0.5f, 96.0f),
                sf::Vector2f(static_cast<float>(facingDirection) * 360.0f, 80.0f),
                false, true
            ));
        }

        if (frameIndex >= 10) {
            isAttacking = false;
            shootCooldown = 1.35f;
        }
        return;
    }

    velocity.x = static_cast<float>(facingDirection) * 42.0f;
    if (std::abs(velocity.x) > 0.1f) {
        animator.play(EnemyTextures::bossWalkAnim);
    } else {
        animator.play(EnemyTextures::bossIdleAnim);
    }
    animator.update(dt);

    applyGravity(dt);

    const float beforeX = position.x;
    velocity *= dt;
    Collision::resolveTileCollision(*this, level.getTileMap());
    velocity /= dt;

    const float frontX = facingDirection > 0 ? position.x + size.x + 3.0f : position.x - 3.0f;
    if (std::abs(position.x - beforeX) < 1.0f ||
        !level.getTileMap().isSolidAt({frontX, position.y + size.y + 4.0f})) {
        facingDirection *= -1;
    }

    Player* target = level.closestLivingPlayer(*this);
    if (target && std::abs(target->position.x - position.x) < 500.0f) {
        facingDirection = target->position.x > position.x ? 1 : -1;
        shootCooldown -= dt;
        if (shootCooldown <= 0.0f) {
            isAttacking = true;
            hasFired = false;
            animator.play(EnemyTextures::bossAttackAnim);
        }
    } else {
        shootCooldown = std::max(0.0f, shootCooldown - dt);
    }
}

void BossEnemy::takeDamage(int damage, Level& level, const Player& source) {
    if (isDying || hitTimer > 0.0f) return;
    health -= damage;
    hitTimer = 0.3f;
    animator.play(EnemyTextures::bossHurtAnim);
    if (health <= 0) {
        isDying = true;
        deathTimer = 1.0f; // 13 frames at ~13 fps
        animator.play(EnemyTextures::bossDieAnim);
        level.dropLoot(position + size * 0.5f);
        // extra loot for boss
        level.dropLoot(position + size * 0.5f + sf::Vector2f(30, -30));
        level.dropLoot(position + size * 0.5f + sf::Vector2f(-30, -30));
    }
}

int BossEnemy::getDamage() const {
    return isDying ? 0 : damage;
}

void BossEnemy::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    const sf::Texture* currentTexture = nullptr;

    if (isDying) {
        currentTexture = EnemyTextures::bossDie;
    } else if (hitTimer > 0.0f) {
        currentTexture = EnemyTextures::bossHurt;
    } else if (isAttacking) {
        currentTexture = EnemyTextures::bossAttack;
    } else {
        if (std::abs(velocity.x) > 0.1f) {
            currentTexture = EnemyTextures::bossWalk;
        } else {
            currentTexture = EnemyTextures::bossIdle;
        }
    }

    if (currentTexture) {
        sf::Sprite sprite(*currentTexture);
        sprite.setTextureRect(animator.getFrameRect());
        sprite.setOrigin({45.0f, 0.0f});
        
        sprite.setScale({facingDirection > 0 ? 2.0f : -2.0f, 2.0f}); 
        
        sprite.setPosition({position.x - camera.x + 90.0f, position.y - camera.y});
        window.draw(sprite);
    } else {
        // fallback
        sf::RectangleShape shell(size);
        shell.setPosition(position - camera);
        shell.setFillColor({91, 32, 43});
        window.draw(shell);

        sf::RectangleShape core(size - sf::Vector2f(32.0f, 20.0f));
        core.setPosition(position - camera + sf::Vector2f(16.0f, 16.0f));
        core.setFillColor(bodyColor);
        window.draw(core);
    }

    if (!isDying) {
        drawHealthBar(window, camera);
    }
}
