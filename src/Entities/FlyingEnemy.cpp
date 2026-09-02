#include "Entities/FlyingEnemy.h"
#include "Entities/Player.h"
#include "Graphics/EnemySprites.h"
#include "Utils/Constants.h"
#include "World/Collision.h"
#include "World/Level.h"

#include <cmath>

FlyingEnemy::FlyingEnemy(sf::Vector2f position)
    : Enemy(position + sf::Vector2f(0.0f, -64.0f),
            {64.0f, 64.0f},
            Constants::FLYING_ENEMY_HEALTH,
            {96, 176, 88}),
      origin(this->position) {
    facingDirection = 1;
}

void FlyingEnemy::update(float dt, Level& level) {
    tick(dt);

    if (isDying) {
        deathTimer -= dt;
        if (deathTimer <= 0.0f) {
            alive = false;
        }
        return;
    }

    animTime += dt;
    waveTime += dt;

    if (hitTimer <= 0.0f) {
        Player* target = level.closestLivingPlayer(*this);
        bool movingToTarget = false;

        if (target &&
            std::abs(target->position.x - position.x) <
                Constants::FLYING_ENEMY_AGGRO_RANGE) {
            if (target->position.x > position.x &&
                position.x < origin.x + Constants::FLYING_ENEMY_TRACK_RANGE) {
                facingDirection = 1;
                movingToTarget = true;
            } else if (target->position.x < position.x &&
                       position.x > origin.x - Constants::FLYING_ENEMY_TRACK_RANGE) {
                facingDirection = -1;
                movingToTarget = true;
            }
        }

        if (!movingToTarget) {
            if (position.x < origin.x - Constants::FLYING_ENEMY_PATROL_RANGE) {
                facingDirection = 1;
            } else if (position.x > origin.x + Constants::FLYING_ENEMY_PATROL_RANGE) {
                facingDirection = -1;
            }
        }

        velocity.x = static_cast<float>(facingDirection) * Constants::FLYING_ENEMY_SPEED;
    }

    // Flying motion deliberately replaces gravity with a sinusoidal target.
    const float targetY =
        origin.y +
        std::sin(waveTime * Constants::FLYING_ENEMY_WAVE_FREQUENCY) *
            Constants::FLYING_ENEMY_WAVE_AMPLITUDE;
    velocity.y = (targetY - position.y) / dt;

    const float requestedHorizontalVelocity = velocity.x;
    velocity *= dt;
    Collision::resolveTileCollision(*this, level.getTileMap());
    velocity /= dt;

    const bool hitWall =
        requestedHorizontalVelocity != 0.0f && velocity.x == 0.0f;
    if (hitWall) {
        facingDirection *= -1;
        velocity.x = -requestedHorizontalVelocity;
    }
}

void FlyingEnemy::takeDamage(int damage, Level& level, const Player& source) {
    if (isDying || hitTimer > 0.0f) {
        return;
    }

    health -= damage;
    hitTimer = Constants::FLYING_ENEMY_HURT_TIME;
    animTime = 0.0f;
    velocity.x = static_cast<float>(source.getFacingDirection()) *
                 Constants::FLYING_ENEMY_KNOCKBACK;

    if (health <= 0) {
        isDying = true;
        deathTimer = Constants::FLYING_ENEMY_DEATH_TIME;
        velocity = {0.0f, 0.0f};
        animTime = 0.0f;
        level.dropLoot(position + size * 0.5f);
    }
}

int FlyingEnemy::getDamage() const {
    return isDying ? 0 : damage;
}

void FlyingEnemy::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    const sf::Texture* currentTexture = nullptr;
    int frameCount = 1;
    float fps = 12.0f; // Default fps

    if (isDying) {
        currentTexture = EnemyTextures::flyingDie;
        frameCount = 12;
    } else if (hitTimer > 0.0f) {
        currentTexture = EnemyTextures::flyingHurt;
        frameCount = 5;
        fps = 20.0f; // Faster hurt animation
    } else {
        currentTexture = EnemyTextures::flyingIdle;
        frameCount = 9;
    }

    if (currentTexture) {
        sf::Sprite sprite(*currentTexture);
        
        // Clamp frameIndex to last frame if dying so it doesn't loop
        int frameIndex = static_cast<int>(animTime * fps);
        if (isDying && frameIndex >= frameCount) {
            frameIndex = frameCount - 1;
        } else {
            frameIndex %= frameCount;
        }

        sprite.setTextureRect(sf::IntRect({frameIndex * 64, 0}, {64, 64}));
        sprite.setOrigin({32.0f, 0.0f});
        
        // Fix scaling: texture faces left by default.
        sprite.setScale({facingDirection > 0 ? -1.0f : 1.0f, 1.0f});
        
        sprite.setPosition({position.x - camera.x + 32.0f, position.y - camera.y});
        window.draw(sprite);
    } else {
        drawBody(window, camera);
    }

    if (!isDying) {
        drawHealthBar(window, camera);
    }
}
