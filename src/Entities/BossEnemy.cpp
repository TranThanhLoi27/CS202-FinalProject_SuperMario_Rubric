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
    movementDirection = -1;
    shootCooldown = Constants::BOSS_PHASE_ONE_ATTACK_COOLDOWN;
}

void BossEnemy::updatePhase() {
    if (phase == Phase::PhaseOne &&
        static_cast<float>(health) <=
            static_cast<float>(maxHealth) * Constants::BOSS_ENRAGED_HEALTH_RATIO) {
        phase = Phase::Enraged;
        shootCooldown =
            std::min(shootCooldown, Constants::BOSS_ENRAGED_ATTACK_COOLDOWN);
    }
}

float BossEnemy::movementSpeed() const {
    const float multiplier = phase == Phase::Enraged
        ? Constants::BOSS_ENRAGED_SPEED_MULTIPLIER
        : 1.0f;
    return Constants::BOSS_MOVE_SPEED * multiplier;
}

float BossEnemy::attackCooldown() const {
    return phase == Phase::Enraged
        ? Constants::BOSS_ENRAGED_ATTACK_COOLDOWN
        : Constants::BOSS_PHASE_ONE_ATTACK_COOLDOWN;
}

void BossEnemy::fireAttack(Level& level) {
    if (phase == Phase::PhaseOne) {
        for (const float verticalSpeed :
             {-Constants::BOSS_PROJECTILE_SPREAD_SPEED,
              0.0f,
              Constants::BOSS_PROJECTILE_SPREAD_SPEED}) {
            level.addProjectile(std::make_unique<Projectile>(
                position + sf::Vector2f(size.x * 0.5f, 56.0f),
                sf::Vector2f(
                    static_cast<float>(facingDirection) * Constants::BOSS_PROJECTILE_SPEED,
                    verticalSpeed),
                false,
                true));
        }
        return;
    }

    // Enraged ground slam: send one floor-skimming shockwave in each direction.
    for (const int direction : {-1, 1}) {
        level.addProjectile(std::make_unique<Projectile>(
            position + sf::Vector2f(size.x * 0.5f, size.y - 16.0f),
            sf::Vector2f(
                static_cast<float>(direction) * Constants::BOSS_SHOCKWAVE_SPEED,
                0.0f),
            false,
            true));
    }
}

void BossEnemy::update(float dt, Level& level) {
    tick(dt);
    updatePhase();

    if (isDying) {
        velocity = {0.0f, 0.0f};
        deathTimer -= dt;
        if (deathTimer <= 0.0f) {
            alive = false;
        }
        animTime += dt;
        return;
    }

    Player* target = level.closestLivingPlayer(*this);
    if (target && std::abs(target->position.x - position.x) > 1.0f) {
        facingDirection = target->position.x > position.x ? 1 : -1;
    }

    if (hitTimer > 0.0f) {
        animTime += dt;
        velocity = {0.0f, 0.0f};
        applyGravity(dt);
        velocity *= dt;
        Collision::resolveTileCollision(*this, level.getTileMap());
        velocity /= dt;
        return;
    }

    if (isAttacking) {
        animTime += dt;
        velocity.x = 0.0f;
        applyGravity(dt);
        velocity *= dt;
        Collision::resolveTileCollision(*this, level.getTileMap());
        velocity /= dt;

        const int frameIndex = static_cast<int>(animTime * 12.0f);
        if (frameIndex >= 7 && !hasFired) {
            hasFired = true;
            fireAttack(level);
        }

        if (frameIndex >= 11) {
            isAttacking = false;
            shootCooldown = attackCooldown();
            animTime = 0.0f;
        }
        return;
    }

    animTime += dt;
    velocity.x = static_cast<float>(movementDirection) * movementSpeed();
    applyGravity(dt);

    const float requestedHorizontalVelocity = velocity.x;
    velocity *= dt;
    Collision::resolveTileCollision(*this, level.getTileMap());
    velocity /= dt;

    const bool hitWall =
        requestedHorizontalVelocity != 0.0f && velocity.x == 0.0f;
    const float frontX = movementDirection > 0
        ? position.x + size.x + 3.0f
        : position.x - 3.0f;
    if (hitWall ||
        !level.getTileMap().isSolidAt({frontX, position.y + size.y + 4.0f})) {
        movementDirection *= -1;
    }

    if (target &&
        std::abs(target->position.x - position.x) < Constants::BOSS_TRACK_RANGE) {
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

void BossEnemy::takeDamage(int damage, Level& level, const Player& /*source*/) {
    if (isDying || hitTimer > 0.0f) {
        return;
    }

    health -= damage;
    hitTimer = Constants::BOSS_HURT_TIME;
    animTime = 0.0f;
    velocity = {0.0f, 0.0f};
    updatePhase();

    if (health <= 0) {
        isDying = true;
        deathTimer = Constants::BOSS_DEATH_TIME;
        animTime = 0.0f;
        level.dropLoot(position + size * 0.5f);
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
