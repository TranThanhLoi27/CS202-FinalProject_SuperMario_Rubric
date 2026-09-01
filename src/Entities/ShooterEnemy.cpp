#include "Entities/ShooterEnemy.h"
#include "Entities/Projectile.h"
#include "Graphics/EnemySprites.h"
#include "World/Collision.h"
#include "World/Level.h"
#include "Utils/Constants.h"

ShooterEnemy::ShooterEnemy(sf::Vector2f pos) : Enemy(pos, {48.0f, 72.0f}, Constants::SHOOTER_ENEMY_HEALTH, {118, 99, 196}) {}

void ShooterEnemy::update(float dt, Level& level) {
    tick(dt);
    applyGravity(dt);
    velocity *= dt;
    Collision::resolveTileCollision(*this, level.getTileMap());
    velocity /= dt;
    Player* target = level.closestLivingPlayer(*this);
    shootCooldown -= dt;
    if (target && shootCooldown <= 0) {
        facingDirection = target->position.x > position.x ? 1 : -1;
        shootCooldown = 1.5f;
        level.addProjectile(std::make_unique<Projectile>(
            position + sf::Vector2f(24.0f, 29.0f),
            sf::Vector2f(facingDirection * 300.f, 0),
            false
        ));
    }
}

void ShooterEnemy::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    if (EnemyTextures::shooter) {
        sf::Sprite sprite(*EnemyTextures::shooter);
        const auto texSize = EnemyTextures::shooter->getSize();
        const float scaleX = size.x / static_cast<float>(texSize.x);
        const float scaleY = size.y / static_cast<float>(texSize.y);
        if (facingDirection < 0) {
            sprite.setOrigin({static_cast<float>(texSize.x), 0.0f});
            sprite.setScale({-scaleX, scaleY});
        } else {
            sprite.setOrigin({0.0f, 0.0f});
            sprite.setScale({scaleX, scaleY});
        }
        sprite.setPosition(position - camera);
        window.draw(sprite);
    } else {
        drawBody(window, camera);
    }
    drawHealthBar(window, camera);
}
