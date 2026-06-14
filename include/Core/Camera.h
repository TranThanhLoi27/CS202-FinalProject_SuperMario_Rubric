#pragma once

#include <SFML/Graphics.hpp>

class Camera{
public:
    //set view, worldsize
    Camera(sf::Vector2f viewsize, sf::Vector2f levelsize);
    //view follows 2 players.
    void Change(const sf::Vector2f& p1, const sf::Vector2f& p2);
private:
    sf::View view;
    sf::Vector2f worldsize; 
};