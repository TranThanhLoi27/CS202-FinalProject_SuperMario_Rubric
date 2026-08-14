#include "Entities/PatrolEnemy.h"
#include "World/Collision.h"
#include "World/Level.h"
#include "Utils/Constants.h"
#include <cmath>
PatrolEnemy::PatrolEnemy(sf::Vector2f pos) : Enemy(pos,{30,38},Constants::PATROL_ENEMY_HEALTH,{185,104,78}) {}
void PatrolEnemy::update(float dt, Level& level) {
    tick(dt);
    velocity.x = static_cast<float>(facingDirection) * 65;
    applyGravity(dt);
    velocity *= dt;
    Collision::resolveTileCollision(*this, level.getTileMap());
    velocity /= dt;
    if (velocity.x == 0) facingDirection *= -1;
}
void PatrolEnemy::draw(sf::RenderWindow& window,sf::Vector2f camera) const { sf::RectangleShape body(size); body.setPosition(position-camera); body.setFillColor(bodyColor); window.draw(body); drawHealthBar(window,camera); }

