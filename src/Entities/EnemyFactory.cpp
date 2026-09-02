#include "Entities/EnemyFactory.h"

#include "Entities/BossEnemy.h"
#include "Entities/FlyingEnemy.h"
#include "Entities/PatrolEnemy.h"
#include "Entities/ShooterEnemy.h"

// Implements the Simple Factory pattern for data-driven enemy construction.
std::unique_ptr<Enemy> EnemyFactory::create(char type, sf::Vector2f position) {
    switch (type) {
        case 'P':
            return std::make_unique<PatrolEnemy>(position);
        case 'R':
            return std::make_unique<ShooterEnemy>(position);
        case 'F':
            return std::make_unique<FlyingEnemy>(position);
        case 'B':
            return std::make_unique<BossEnemy>(position + sf::Vector2f(0.0f, -46.0f));
        default:
            return nullptr;
    }
}
