#pragma once

#include "Graphics/Animation.h"
#include <SFML/Graphics/Texture.hpp>

struct EnemyTextures {
    static void setTextures(const sf::Texture& patrol,
                            const sf::Texture& shooter,
                            const sf::Texture& flyingIdle,
                            const sf::Texture& flyingHurt,
                            const sf::Texture& flyingDie,
                            const sf::Texture& bossIdle,
                            const sf::Texture& bossWalk,
                            const sf::Texture& bossHurt,
                            const sf::Texture& bossDie,
                            const sf::Texture& bossAttack,
                            const sf::Texture& projectile);

    static const sf::Texture* patrol;
    static const sf::Texture* shooter;
    static const sf::Texture* flyingIdle;
    static const sf::Texture* flyingHurt;
    static const sf::Texture* flyingDie;
    static const sf::Texture* bossIdle;
    static const sf::Texture* bossWalk;
    static const sf::Texture* bossHurt;
    static const sf::Texture* bossDie;
    static const sf::Texture* bossAttack;
    static const sf::Texture* projectile;

    // Animations
    static Animation patrolWalkAnim;
    static Animation patrolDieAnim;

    static Animation flyingRunAnim;
    static Animation flyingHurtAnim;
    static Animation flyingDieAnim;

    static Animation bossWalkAnim;
    static Animation bossIdleAnim;
    static Animation bossAttackAnim;
    static Animation bossHurtAnim;
    static Animation bossDieAnim;
};

