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
    : Enemy(position, {72.0f, 78.0f}, Constants::BOSS_HEALTH, {142, 65, 77}) {
    facingDirection = -1;
}

void BossEnemy::update(float dt, Level& level) {
    tick(dt);
    if (hitTimer <= 0.0f) velocity.x = static_cast<float>(facingDirection) * 42.0f;
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
    if (target) facingDirection = target->position.x > position.x ? 1 : -1;
    shootCooldown -= dt;
    if (target && shootCooldown <= 0.0f) {
        shootCooldown = 1.35f;
        level.addProjectile(std::make_unique<Projectile>(
            position + sf::Vector2f(size.x * 0.5f, 24.0f),
            sf::Vector2f(static_cast<float>(facingDirection) * 430.0f, -60.0f),
            false
        ));
        level.addProjectile(std::make_unique<Projectile>(
            position + sf::Vector2f(size.x * 0.5f, 48.0f),
            sf::Vector2f(static_cast<float>(facingDirection) * 360.0f, 80.0f),
            false
        ));
    }
}

void BossEnemy::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    if (EnemyTextures::boss) {
        drawSprite(window, camera, *EnemyTextures::boss);
    } else {
        sf::RectangleShape shell(size);
        shell.setPosition(position - camera);
        shell.setFillColor({91, 32, 43});
        window.draw(shell);

        sf::RectangleShape core(size - sf::Vector2f(16.0f, 10.0f));
        core.setPosition(position - camera + sf::Vector2f(8.0f, 8.0f));
        core.setFillColor(bodyColor);
        window.draw(core);
    }
    drawHealthBar(window, camera);
}
