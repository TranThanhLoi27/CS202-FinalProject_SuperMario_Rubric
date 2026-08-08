#pragma once

#include<SFML/Graphics.hpp>

class GoalGate{
public:
    GoalGate(sf::FloatRect bound, const sf::Texture& teture);
    sf::FloatRect getBound();
    void render(sf::RenderWindow& window) const;
protected:
    sf::Sprite sprite;
private:
    sf::FloatRect bound;
};