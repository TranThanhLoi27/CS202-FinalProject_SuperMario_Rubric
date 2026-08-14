#pragma once

#include<SFML/Graphics.hpp>

class GoalGate{
public:
    GoalGate(sf::FloatRect bound, const sf::Texture& teture);
    const sf::FloatRect& getBound() const;
    void render(sf::RenderWindow& window, sf::Vector2f camera) const;
protected:
    sf::Sprite sprite;
private:
    sf::FloatRect bound;
};
