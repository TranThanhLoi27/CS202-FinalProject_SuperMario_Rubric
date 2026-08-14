#include "Entities/ShooterEnemy.h"
#include "Entities/Projectile.h"
#include "Graphics/EnemySprites.h"
#include "World/Collision.h"
#include "World/Level.h"
#include "Utils/Constants.h"

ShooterEnemy::ShooterEnemy(sf::Vector2f pos) : Enemy(pos, {32.0f, 48.0f}, Constants::SHOOTER_ENEMY_HEALTH, {118, 99, 196}) {}

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
            position + sf::Vector2f(15, 15),
            sf::Vector2f(facingDirection * 300.f, 0),
            false
        ));
    }
}

void ShooterEnemy::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    if (EnemyTextures::shooter) {
        drawSprite(window, camera, *EnemyTextures::shooter);
    } else {
        drawBody(window, camera);
    }
    drawHealthBar(window, camera);
}
