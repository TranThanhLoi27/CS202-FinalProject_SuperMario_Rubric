#include <Core/Camera.h>
#include <Utils/MathUtils.h>
Camera::Camera(sf::Vector2f viewsize, sf::Vector2f levelsize): 
    view(sf::Vector2f(viewsize.x / 2, viewsize.y / 2), viewsize),
    worldsize(levelsize){}

void Camera::Change(const sf::Vector2f& p1, const sf::Vector2f& p2){
    sf::Vector2f center({(p1.x + p2.x) / 2, (p1.y + p2.y) / 2});
    const sf::Vector2f viewsize = view.getSize();
    center.x = MathUtils::clamp(center.x, viewsize.x / 2.f, worldsize.x - viewsize.x / 2.f);
    center.y = MathUtils::clamp(center.y, viewsize.y / 2.f, worldsize.y - viewsize.y / 2.f);
    view.setCenter(center);
}