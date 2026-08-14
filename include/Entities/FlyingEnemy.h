#pragma once

#include "Entities/Enemy.h"

class FlyingEnemy : public Enemy {
public:
    explicit FlyingEnemy(sf::Vector2f position);
    void update(float dt, Level& level) override;
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;

private:
    float waveTime = 0.0f;
    sf::Vector2f origin;
};
