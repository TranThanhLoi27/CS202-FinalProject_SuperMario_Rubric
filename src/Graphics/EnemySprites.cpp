#include "Graphics/EnemySprites.h"

const sf::Texture* EnemyTextures::patrol = nullptr;
const sf::Texture* EnemyTextures::shooter = nullptr;
const sf::Texture* EnemyTextures::flyingIdle = nullptr;
const sf::Texture* EnemyTextures::flyingHurt = nullptr;
const sf::Texture* EnemyTextures::flyingDie = nullptr;
const sf::Texture* EnemyTextures::bossIdle = nullptr;
const sf::Texture* EnemyTextures::bossWalk = nullptr;
const sf::Texture* EnemyTextures::bossHurt = nullptr;
const sf::Texture* EnemyTextures::bossDie = nullptr;
const sf::Texture* EnemyTextures::bossAttack = nullptr;
const sf::Texture* EnemyTextures::projectile = nullptr;

Animation EnemyTextures::patrolWalkAnim(sf::IntRect({0, 0}, {32, 32}), 2, 0.15f);
Animation EnemyTextures::patrolDieAnim(sf::IntRect({64, 0}, {32, 32}), 2, 0.15f);

Animation EnemyTextures::flyingRunAnim(sf::IntRect({0, 0}, {64, 64}), 8, 0.10f);
Animation EnemyTextures::flyingHurtAnim(sf::IntRect({0, 0}, {64, 64}), 5, 0.05f);
Animation EnemyTextures::flyingDieAnim(sf::IntRect({0, 0}, {64, 64}), 12, 0.08f);

Animation EnemyTextures::bossWalkAnim(sf::IntRect({0, 0}, {90, 64}), 10, 0.10f);
Animation EnemyTextures::bossIdleAnim(sf::IntRect({0, 0}, {90, 64}), 8, 0.10f);
Animation EnemyTextures::bossAttackAnim(sf::IntRect({0, 0}, {90, 64}), 11, 0.08f);
Animation EnemyTextures::bossHurtAnim(sf::IntRect({0, 0}, {90, 64}), 4, 0.08f);
Animation EnemyTextures::bossDieAnim(sf::IntRect({0, 0}, {90, 64}), 13, 0.08f);

void EnemyTextures::setTextures(const sf::Texture& patrolTex,
                                const sf::Texture& shooterTex,
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
    shooter = &shooterTex;
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

