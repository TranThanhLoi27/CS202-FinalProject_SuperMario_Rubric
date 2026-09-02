#pragma once

#include <SFML/Graphics/Texture.hpp>

struct EnemyTextures {
    /// Registers all shared enemy textures while AssetManager retains ownership.
    static void setTextures(const sf::Texture& patrol,
                            const sf::Texture& shooterIdle,
                            const sf::Texture& shooterAttack,
                            const sf::Texture& shooterDie,
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
    static const sf::Texture* shooterIdle;
    static const sf::Texture* shooterAttack;
    static const sf::Texture* shooterDie;
    static const sf::Texture* flyingIdle;
    static const sf::Texture* flyingHurt;
    static const sf::Texture* flyingDie;
    static const sf::Texture* bossIdle;
    static const sf::Texture* bossWalk;
    static const sf::Texture* bossHurt;
    static const sf::Texture* bossDie;
    static const sf::Texture* bossAttack;
    static const sf::Texture* projectile;
};
