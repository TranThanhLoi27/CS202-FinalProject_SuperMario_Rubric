#pragma once

#include <SFML/Graphics/Texture.hpp>

struct EnemyTextures {
    static void setTextures(const sf::Texture& patrol,
                            const sf::Texture& shooter,
                            const sf::Texture& flyingIdle,
                            const sf::Texture& flyingHurt,
                            const sf::Texture& flyingDie,
                            const sf::Texture& boss,
                            const sf::Texture& projectile);

    static const sf::Texture* patrol;
    static const sf::Texture* shooter;
    static const sf::Texture* flyingIdle;
    static const sf::Texture* flyingHurt;
    static const sf::Texture* flyingDie;
    static const sf::Texture* boss;
    static const sf::Texture* projectile;
};
