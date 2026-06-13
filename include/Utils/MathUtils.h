#pragma once
#include <SFML/Graphics.hpp>

namespace MathUtils{

    //tra ve gia tri khong vuot qua [low, high]
    template <typename T>
    T clamp(T value, T low, T high){
        return value < low ? low : value > high ? high : low;
    }

    //kiem tra 2 khoi co cat nhau khong
    bool intersect(const sf::FloatRect& rec1, sf::FloatRect& rec2);

    //khoang cach cua 2 vecto
    float distance(const sf::Vector2f u, const sf::Vector2f v);
}