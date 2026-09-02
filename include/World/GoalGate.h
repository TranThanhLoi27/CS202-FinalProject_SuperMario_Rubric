#pragma once

#include<SFML/Graphics.hpp>

class GoalGate{
public:
    /// Creates a goal trigger and its non-owning texture-backed sprite.
    GoalGate(sf::FloatRect bound, const sf::Texture& teture);
    /// Returns the world-space goal trigger.
    const sf::FloatRect& getBound() const;
    /// Draws the goal sprite relative to the camera.
    void render(sf::RenderWindow& window, sf::Vector2f camera) const;
protected:
    sf::Sprite sprite;
private:
    sf::FloatRect bound;
};
