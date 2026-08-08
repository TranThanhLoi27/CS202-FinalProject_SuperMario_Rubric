#include<World/GoalGate.h>

GoalGate::GoalGate(sf::FloatRect bound, const sf::Texture& texture): bound(bound), sprite(texture){}
sf::FloatRect GoalGate::getBound(){
    return bound;
}

void GoalGate::render(sf::RenderWindow& window) const{
    window.draw(sprite);
}