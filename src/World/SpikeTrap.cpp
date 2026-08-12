#include <World/SpikeTrap.h>

SpikeTrap::SpikeTrap(sf::FloatRect bound, const sf::Texture& texture, int damage): damage(damage), bound(bound), sprite(texture){}

void SpikeTrap::render(sf::RenderWindow& window) const{
    window.draw(sprite);
}

const sf::FloatRect& SpikeTrap::getBound() const {
    return bound;
}
