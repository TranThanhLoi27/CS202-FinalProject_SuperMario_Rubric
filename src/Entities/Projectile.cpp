#include "Entities/Projectile.h"

#include "Utils/Constants.h"
#include "World/TileMap.h"

namespace {
const sf::Texture* mushroomTexture = nullptr;
const sf::Texture* bossTexture = nullptr;
}

void Projectile::setTextures(const sf::Texture& mushroomTex, const sf::Texture& bossTex) {
    mushroomTexture = &mushroomTex;
    bossTexture = &bossTex;
}

Projectile::Projectile(sf::Vector2f position, sf::Vector2f velocity, bool friendly, bool isBoss)
    : Entity(position, {16.0f, 16.0f}),
      friendly(friendly),
      textureType(isBoss ? TextureType::Boss : TextureType::Mushroom) {
    this->velocity = velocity;
}

void Projectile::update(float dt, const TileMap& map) {
    life -= dt;
    animTime += dt;
    position += velocity * dt;
    if (life <= 0.0f || map.isSolidAt(position + size * 0.5f)) alive = false;
}

void Projectile::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    const sf::Texture* tex = textureType == TextureType::Boss ? bossTexture : mushroomTexture;
    if (tex) {
        sf::Sprite sprite(*tex);
        const int frame = static_cast<int>(animTime * 12.0f) % Constants::PROJECTILE_FRAME_COUNT;
        const int frameWidth = static_cast<int>(tex->getSize().x) / Constants::PROJECTILE_FRAME_COUNT;
        const int frameHeight = static_cast<int>(tex->getSize().y);
        sprite.setTextureRect(sf::IntRect({frame * frameWidth, 0}, {frameWidth, frameHeight}));
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
