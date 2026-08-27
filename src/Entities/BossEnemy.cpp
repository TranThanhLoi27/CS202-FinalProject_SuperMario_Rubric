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
}

void BossEnemy::update(float dt, Level& level) {
    tick(dt);
    
    if (isDying) {
        deathTimer -= dt;
        if (deathTimer <= 0.0f) {
            alive = false;
        }
        animTime += dt;
        return;
    }

    if (hitTimer > 0.0f) {
        animTime += dt;
        applyGravity(dt);
        const float beforeX = position.x;
        velocity *= dt;
        Collision::resolveTileCollision(*this, level.getTileMap());
        velocity /= dt;
        return;
    }

    if (isAttacking) {
        animTime += dt;
        applyGravity(dt);
        const float beforeX = position.x;
        velocity.x = 0; // stop moving
        velocity *= dt;
        Collision::resolveTileCollision(*this, level.getTileMap());
        velocity /= dt;

        int frameIndex = static_cast<int>(animTime * 12.0f);
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

        if (frameIndex >= 11) {
            isAttacking = false;
            shootCooldown = 1.35f;
            animTime = 0.0f;
        }
        return;
    }

    animTime += dt;
    velocity.x = static_cast<float>(facingDirection) * 42.0f;
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
            animTime = 0.0f;
        }
    } else {
        shootCooldown = std::max(0.0f, shootCooldown - dt);
    }
}

void BossEnemy::takeDamage(int damage, Level& level, const Player& source) {
    if (isDying || hitTimer > 0.0f) return;
    health -= damage;
    hitTimer = 0.3f;
    animTime = 0.0f; // Reset anim time for hurt animation
    if (health <= 0) {
        isDying = true;
        deathTimer = 1.0f; // 13 frames at ~13 fps
        animTime = 0.0f; // Reset anim time for death animation
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
    int frameCount = 1;
    float fps = 12.0f; // Default fps

    if (isDying) {
        currentTexture = EnemyTextures::bossDie;
        frameCount = 13; // Die is 13 frames
        fps = 13.0f;
    } else if (hitTimer > 0.0f) {
        currentTexture = EnemyTextures::bossHurt;
        frameCount = 4;
        fps = 13.0f;
    } else if (isAttacking) {
        currentTexture = EnemyTextures::bossAttack;
        frameCount = 11;
        fps = 12.0f;
    } else {
        // Walk or Idle
        if (std::abs(velocity.x) > 0.1f) {
            currentTexture = EnemyTextures::bossWalk;
            frameCount = 10;
        } else {
            currentTexture = EnemyTextures::bossIdle;
            frameCount = 8;
        }
    }

    if (currentTexture) {
        sf::Sprite sprite(*currentTexture);
        
        int frameIndex = static_cast<int>(animTime * fps);
        if ((isDying || isAttacking) && frameIndex >= frameCount) {
            frameIndex = frameCount - 1;
        } else {
            frameIndex %= frameCount;
        }

        sprite.setTextureRect(sf::IntRect({frameIndex * 90, 0}, {90, 64}));
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
