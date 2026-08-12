#pragma once
#include "Entities/Enemy.h"

// Flying enemy that moves in a sine wave pattern in the air
class FlyingEnemy : public Enemy {
private:
    float speed;
    float time;

public:
    // Constructor setting start position
    FlyingEnemy(const sf::Vector2f& startPosition);
    virtual ~FlyingEnemy() = default;

    // Updates vertical sine wave position per frame
    void update(float dt) override;
};
