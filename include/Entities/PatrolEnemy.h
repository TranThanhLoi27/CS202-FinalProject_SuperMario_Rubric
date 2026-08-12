#pragma once
#include "Entities/Enemy.h"

// Ground enemy that patrols back and forth horizontally
class PatrolEnemy : public Enemy {
private:
    float patrolDistance;
    float startX;
    float speed;
    int direction; // 1 for right, -1 for left

public:
    // Constructor setting start position and patrol range
    PatrolEnemy(const sf::Vector2f& startPosition, float distance = 100.0f);
    virtual ~PatrolEnemy() = default;

    // Updates horizontal movement and reverses direction upon reaching patrol boundaries
    void update(float dt) override;
};
