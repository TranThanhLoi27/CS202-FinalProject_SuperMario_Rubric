#include <World/SpikeTrap.h>

SpikeTrap::SpikeTrap(sf::FloatRect bound, const sf::Texture& texture, int damage): damage(damage), bound(bound), sprite(texture){}

void SpikeTrap::render(sf::RenderWindow& window, sf::Vector2f camera) const {
    sf::Sprite drawSprite = sprite;
    const sf::Vector2u texSize = sprite.getTexture().getSize();
    drawSprite.setPosition(bound.position - camera);
    drawSprite.setScale({
        bound.size.x / static_cast<float>(texSize.x),
        bound.size.y / static_cast<float>(texSize.y)
    });
    window.draw(drawSprite);
}

const sf::FloatRect& SpikeTrap::getBound() const {
    return bound;
}
