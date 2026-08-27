#include "Graphics/EnemySprites.h"

const sf::Texture* EnemyTextures::patrol = nullptr;
const sf::Texture* EnemyTextures::shooter = nullptr;
const sf::Texture* EnemyTextures::flyingIdle = nullptr;
const sf::Texture* EnemyTextures::flyingHurt = nullptr;
const sf::Texture* EnemyTextures::flyingDie = nullptr;
const sf::Texture* EnemyTextures::boss = nullptr;
const sf::Texture* EnemyTextures::projectile = nullptr;

void EnemyTextures::setTextures(const sf::Texture& patrolTex,
                                const sf::Texture& shooterTex,
                                const sf::Texture& flyingIdleTex,
                                const sf::Texture& flyingHurtTex,
                                const sf::Texture& flyingDieTex,
                                const sf::Texture& bossTex,
                                const sf::Texture& projectileTex) {
    patrol = &patrolTex;
    shooter = &shooterTex;
    flyingIdle = &flyingIdleTex;
    flyingHurt = &flyingHurtTex;
    flyingDie = &flyingDieTex;
    boss = &bossTex;
    projectile = &projectileTex;
}
