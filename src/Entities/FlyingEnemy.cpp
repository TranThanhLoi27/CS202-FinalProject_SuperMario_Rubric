#include "Entities/FlyingEnemy.h"
#include "Graphics/EnemySprites.h"
#include "World/Level.h"
#include "Entities/Player.h"

#include <cmath>

FlyingEnemy::FlyingEnemy(sf::Vector2f position)
    : Enemy(position + sf::Vector2f(0.0f, -32.0f), {64.0f, 64.0f}, 2, {96, 176, 88}),
      origin(position + sf::Vector2f(0.0f, -64.0f)) {
    this->position = origin;
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
        
        // Track target only if within a reasonable distance, otherwise patrol origin
        bool movingToTarget = false;
        if (target && std::abs(target->position.x - position.x) < 400.0f) {
            if (target->position.x > position.x && position.x < origin.x + 150.0f) {
                facingDirection = 1;
                movingToTarget = true;
            } else if (target->position.x < position.x && position.x > origin.x - 150.0f) {
                facingDirection = -1;
                movingToTarget = true;
            }
        }
        
        if (!movingToTarget) {
            if (position.x < origin.x - 128.0f) {
                facingDirection = 1;
            } else if (position.x > origin.x + 128.0f) {
                facingDirection = -1;
            }
        }

        velocity.x = static_cast<float>(facingDirection) * 72.0f;
        position.x += velocity.x * dt;
        position.y = origin.y + std::sin(waveTime * 2.2f) * 42.0f;
    }
}

void FlyingEnemy::takeDamage(int damage, Level& level, const Player& source) {
    if (isDying || hitTimer > 0.0f) return;
    health -= damage;
    hitTimer = 0.25f;
    animTime = 0.0f; // Reset anim time for hurt animation
    velocity.x += static_cast<float>(source.getFacingDirection()) * 150.0f; // Minor knockback for flying
    if (health <= 0) {
        isDying = true;
        deathTimer = 1.0f; // 12 frames at ~12 fps
        animTime = 0.0f; // Reset anim time for death animation
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
