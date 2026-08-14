#include <cmath>

#include <Utils/MathUtils.h>

namespace MathUtils {

bool intersects(const sf::FloatRect& rec1, const sf::FloatRect& rec2){
    return rec1.position.x < rec2.position.x + rec2.size.x && 
           rec1.position.x + rec1.size.x > rec2.position.x &&
           rec1.position.y < rec2.position.y + rec2.size.y &&
           rec1.position.y + rec1.size.y > rec2.position.y;
}

float distance(const sf::Vector2f v1, const sf::Vector2f v2){
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    return std::sqrt(dx * dx + dy * dy);
}

}
