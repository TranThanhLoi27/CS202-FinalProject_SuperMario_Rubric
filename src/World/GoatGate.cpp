#include<World/GoalGate.h>

GoalGate::GoalGate(sf::FloatRect bound, const sf::Texture& texture): bound(bound), sprite(texture){}
const sf::FloatRect& GoalGate::getBound() const {
    return bound;
}

void GoalGate::render(sf::RenderWindow& window, sf::Vector2f camera) const {
    sf::Sprite drawSprite = sprite;
    const sf::Vector2u texSize = sprite.getTexture().getSize();
    drawSprite.setPosition(bound.position - camera);
    drawSprite.setScale({
        bound.size.x / static_cast<float>(texSize.x),
        bound.size.y / static_cast<float>(texSize.y)
    });
    window.draw(drawSprite);
}
