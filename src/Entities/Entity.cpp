// Implements base transform and bounds shared by all gameplay objects.
#include "Entities/Entity.h"

Entity::Entity(sf::Vector2f position, sf::Vector2f size) : position(position), size(size) {}

sf::FloatRect Entity::getBounds() const {
    return {position, size};
}
