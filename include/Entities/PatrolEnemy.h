// Ground enemy that walks back and forth and turns at walls or ledges.
#pragma once

#include "Entities/Enemy.h"

class PatrolEnemy : public Enemy {
public:
    explicit PatrolEnemy(sf::Vector2f position);
    void update(float dt, Level& level) override;
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    void takeDamage(int damage, Level& level, const Player& source) override;
    int getDamage() const override;

private:
    float animTime = 0.0f;
    float deathTimer = 0.0f;
    bool isDying = false;
};
