#include "Graphics/EnemySprites.h"

const sf::Texture* EnemyTextures::patrol = nullptr;
const sf::Texture* EnemyTextures::shooter = nullptr;
const sf::Texture* EnemyTextures::flying = nullptr;
const sf::Texture* EnemyTextures::boss = nullptr;
const sf::Texture* EnemyTextures::projectile = nullptr;

void EnemyTextures::setTextures(const sf::Texture& patrolTex,
                                const sf::Texture& shooterTex,
                                const sf::Texture& flyingTex,
                                const sf::Texture& bossTex,
                                const sf::Texture& projectileTex) {
    patrol = &patrolTex;
    shooter = &shooterTex;
    flying = &flyingTex;
    boss = &bossTex;
    projectile = &projectileTex;
}
