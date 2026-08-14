#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class SpikeTrap{
public:
    SpikeTrap(sf::FloatRect bound, const sf::Texture& texture, int damage = 1);
    void render(sf::RenderWindow& window, sf::Vector2f camera) const;
    const sf::FloatRect& getBound() const;
    int getDamage() const { return damage; }
protected:
sf::Sprite sprite;
private:
    sf::FloatRect bound;
    int damage = 1;
};
