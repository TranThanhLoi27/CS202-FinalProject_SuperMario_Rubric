#pragma once

#include <SFML/Graphics/Texture.hpp>

struct EnemyTextures {
    static void setTextures(const sf::Texture& patrol,
                            const sf::Texture& shooter,
                            const sf::Texture& flying,
                            const sf::Texture& boss,
                            const sf::Texture& projectile);

    static const sf::Texture* patrol;
    static const sf::Texture* shooter;
    static const sf::Texture* flying;
    static const sf::Texture* boss;
    static const sf::Texture* projectile;
};
