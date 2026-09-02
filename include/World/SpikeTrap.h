#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class SpikeTrap{
public:
    /// Creates a spike trigger, sprite, and damage value.
    SpikeTrap(sf::FloatRect bound, const sf::Texture& texture, int damage = 1);
    /// Draws the spike sprite relative to the camera.
    void render(sf::RenderWindow& window, sf::Vector2f camera) const;
    /// Returns the world-space damage trigger.
    const sf::FloatRect& getBound() const;
    /// Returns damage applied per spike hit.
    int getDamage() const { return damage; }
protected:
sf::Sprite sprite;
private:
    sf::FloatRect bound;
    int damage = 1;
};
