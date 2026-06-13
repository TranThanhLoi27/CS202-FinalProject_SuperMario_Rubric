#pragma once

#include <SFML/Graphics.hpp>

class Camera{
public:
    Camera(sf:: View view, sf::Vector2f worldsize);
    void Change(const sf::Vector2f& p1, const sf::Vector2f& p2);
    void SetWorldSize(const sf::Vector2f size);
private:
    sf::View view;
    sf::Vector2f worldsize; 
};