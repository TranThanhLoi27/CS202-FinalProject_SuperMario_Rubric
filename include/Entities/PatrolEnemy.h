// Ground enemy that walks back and forth and turns at walls or ledges.
#pragma once

#include "Entities/Enemy.h"

class PatrolEnemy : public Enemy {
public:
    explicit PatrolEnemy(sf::Vector2f position);
    void update(float dt, Level& level) override;
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
};
