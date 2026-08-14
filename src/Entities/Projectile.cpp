#include "Entities/Projectile.h"

#include "Utils/Constants.h"
#include "World/TileMap.h"

namespace {
const sf::Texture* projectileTex = nullptr;
}

void Projectile::setTexture(const sf::Texture& texture) {
    projectileTex = &texture;
}

Projectile::Projectile(sf::Vector2f position, sf::Vector2f velocity, bool friendly)
    : Entity(position, {static_cast<float>(Constants::ITEM_ICON_SIZE), static_cast<float>(Constants::ITEM_ICON_SIZE)}),
      friendly(friendly) {
    this->velocity = velocity;
}

void Projectile::update(float dt, const TileMap& map) {
    life -= dt;
    animTime += dt;
    position += velocity * dt;
    if (life <= 0.0f || map.isSolidAt(position + size * 0.5f)) alive = false;
}

void Projectile::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    if (projectileTex) {
        sf::Sprite sprite(*projectileTex);
        const int frame = static_cast<int>(animTime * 12.0f) % Constants::PROJECTILE_FRAME_COUNT;
        sprite.setTextureRect(sf::IntRect({frame * Constants::ITEM_ICON_SIZE, 0},
                                          {Constants::ITEM_ICON_SIZE, Constants::ITEM_ICON_SIZE}));
        sprite.setPosition(position - camera);
        window.draw(sprite);
        return;
    }

    sf::CircleShape shape(6.0f);
    shape.setPosition(position - camera);
    shape.setFillColor(friendly ? sf::Color(241, 209, 107) : sf::Color(216, 95, 159));
    window.draw(shape);
}

bool Projectile::isFriendly() const {
    return friendly;
}
