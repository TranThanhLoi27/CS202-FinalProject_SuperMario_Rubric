#include "Entities/ShooterEnemy.h"
#include "Entities/Player.h"
#include "Entities/Projectile.h"
#include "Graphics/EnemySprites.h"
#include "World/Level.h"
#include "Utils/Constants.h"

#include <algorithm>
#include <memory>

namespace {
constexpr int kFrameSize = 32;
constexpr float kSpriteScale = 2.0f;
constexpr float kRenderedSize = static_cast<float>(kFrameSize) * kSpriteScale;
constexpr int kIdleFrameCount = 2;
constexpr int kAttackFrameCount = 2;
constexpr int kDeathFrameCount = 5;
constexpr float kAnimationFps = 10.0f;
constexpr float kDeathDuration = static_cast<float>(kDeathFrameCount) / kAnimationFps;
}

ShooterEnemy::ShooterEnemy(sf::Vector2f pos)
    : Enemy(pos + sf::Vector2f(0.0f, -static_cast<float>(kFrameSize)),
            {kRenderedSize, kRenderedSize},
            Constants::SHOOTER_ENEMY_HEALTH, {118, 99, 196}),
      rootPosition(position) {
    facingDirection = -1;
}

void ShooterEnemy::update(float dt, Level& level) {
    // The mushroom is rooted: it never integrates velocity or receives gravity.
    position = rootPosition;
    velocity = {};
    tick(dt);
    animationTime += dt;

    if (isDying) {
        deathTimer -= dt;
        if (deathTimer <= 0.0f) alive = false;
        return;
    }

    attackTimer = std::max(0.0f, attackTimer - dt);
    if (hitTimer > 0.0f) return;

    Player* target = level.closestLivingPlayer(*this);
    shootCooldown -= dt;
    if (target && shootCooldown <= 0.0f) {
        facingDirection = target->position.x > position.x ? 1 : -1;
        shootCooldown = 1.5f;
        attackTimer = static_cast<float>(kAttackFrameCount) / kAnimationFps;
        animationTime = 0.0f;
        level.addProjectile(std::make_unique<Projectile>(
            position + sf::Vector2f(
                facingDirection > 0 ? size.x : -16.0f,
                (size.y - 16.0f) * 0.83f
            ),
            sf::Vector2f(static_cast<float>(facingDirection) * 300.0f, 0.0f),
            false
        ));
    }
}

void ShooterEnemy::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    const sf::Texture* texture = EnemyTextures::shooterIdle;
    int frameCount = kIdleFrameCount;
    int frameIndex = 0;

    if (isDying) {
        texture = EnemyTextures::shooterDie;
        frameCount = kDeathFrameCount;
        frameIndex = std::min(static_cast<int>(animationTime * kAnimationFps), frameCount - 1);
    } else if (hitTimer > 0.0f) {
        texture = EnemyTextures::shooterDie;
        frameCount = kDeathFrameCount;
        frameIndex = 1;
    } else if (attackTimer > 0.0f) {
        texture = EnemyTextures::shooterAttack;
        frameCount = kAttackFrameCount;
        frameIndex = std::min(static_cast<int>(animationTime * kAnimationFps), frameCount - 1);
    } else {
        frameIndex = static_cast<int>(animationTime * kAnimationFps) % frameCount;
    }

    if (texture) {
        sf::Sprite sprite(*texture);
        sprite.setTextureRect(sf::IntRect({frameIndex * kFrameSize, 0}, {kFrameSize, kFrameSize}));
        sprite.setOrigin({kFrameSize * 0.5f, 0.0f});
        sprite.setScale({facingDirection > 0 ? -kSpriteScale : kSpriteScale, kSpriteScale});
        sprite.setPosition({position.x - camera.x + size.x * 0.5f, position.y - camera.y});
        window.draw(sprite);
    } else {
        drawBody(window, camera);
    }

    if (!isDying) drawHealthBar(window, camera);
}

void ShooterEnemy::takeDamage(int damage, Level& level, const Player&) {
    if (isDying || hitTimer > 0.0f) return;

    health -= damage;
    hitTimer = 0.22f;
    animationTime = 0.0f;
    velocity = {};

    if (health <= 0) {
        isDying = true;
        deathTimer = kDeathDuration;
        level.dropLoot(position + size * 0.5f);
    }
}

int ShooterEnemy::getDamage() const {
    return isDying ? 0 : damage;
}
