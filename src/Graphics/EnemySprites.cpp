#include "Graphics/EnemySprites.h"

const sf::Texture* EnemyTextures::patrol = nullptr;
const sf::Texture* EnemyTextures::shooterIdle = nullptr;
const sf::Texture* EnemyTextures::shooterAttack = nullptr;
const sf::Texture* EnemyTextures::shooterDie = nullptr;
const sf::Texture* EnemyTextures::flyingIdle = nullptr;
const sf::Texture* EnemyTextures::flyingHurt = nullptr;
const sf::Texture* EnemyTextures::flyingDie = nullptr;
const sf::Texture* EnemyTextures::bossIdle = nullptr;
const sf::Texture* EnemyTextures::bossWalk = nullptr;
const sf::Texture* EnemyTextures::bossHurt = nullptr;
const sf::Texture* EnemyTextures::bossDie = nullptr;
const sf::Texture* EnemyTextures::bossAttack = nullptr;
const sf::Texture* EnemyTextures::projectile = nullptr;

void EnemyTextures::setTextures(const sf::Texture& patrolTex,
                                const sf::Texture& shooterIdleTex,
                                const sf::Texture& shooterAttackTex,
                                const sf::Texture& shooterDieTex,
                                const sf::Texture& flyingIdleTex,
                                const sf::Texture& flyingHurtTex,
                                const sf::Texture& flyingDieTex,
                                const sf::Texture& bossIdleTex,
                                const sf::Texture& bossWalkTex,
                                const sf::Texture& bossHurtTex,
                                const sf::Texture& bossDieTex,
                                const sf::Texture& bossAttackTex,
                                const sf::Texture& projectileTex) {
    patrol = &patrolTex;
    shooterIdle = &shooterIdleTex;
    shooterAttack = &shooterAttackTex;
    shooterDie = &shooterDieTex;
    flyingIdle = &flyingIdleTex;
    flyingHurt = &flyingHurtTex;
    flyingDie = &flyingDieTex;
    bossIdle = &bossIdleTex;
    bossWalk = &bossWalkTex;
    bossHurt = &bossHurtTex;
    bossDie = &bossDieTex;
    bossAttack = &bossAttackTex;
    projectile = &projectileTex;
}
