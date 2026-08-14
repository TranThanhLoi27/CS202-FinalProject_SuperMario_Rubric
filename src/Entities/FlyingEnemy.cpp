#include "Entities/FlyingEnemy.h"
#include "Graphics/EnemySprites.h"
#include "World/Level.h"

#include <cmath>

FlyingEnemy::FlyingEnemy(sf::Vector2f position)
    : Enemy(position, {32.0f, 48.0f}, 2, {96, 176, 88}),
      origin(position + sf::Vector2f(0.0f, -64.0f)) {
    position = origin;
    facingDirection = 1;
}

void FlyingEnemy::update(float dt, Level& level) {
    tick(dt);
    waveTime += dt;

    Player* target = level.closestLivingPlayer(*this);
    if (target) {
        facingDirection = target->position.x > position.x ? 1 : -1;
    }

    velocity.x = static_cast<float>(facingDirection) * 72.0f;
    position.x += velocity.x * dt;
    position.y = origin.y + std::sin(waveTime * 2.2f) * 42.0f;

    if (position.x < origin.x - 128.0f || position.x > origin.x + 128.0f) {
        facingDirection *= -1;
    }
}

void FlyingEnemy::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    if (EnemyTextures::flying) {
        drawSprite(window, camera, *EnemyTextures::flying);
    } else {
        drawBody(window, camera);
    }
    drawHealthBar(window, camera);
}
