#pragma once

#include "Entities/Entity.h"

class Projectile : public Entity {
public:
    /// Registers the shared mushroom and boss projectile textures.
    static void setTextures(const sf::Texture& mushroomTexture, const sf::Texture& bossTexture);

    /// Creates a projectile with ownership, velocity, and texture-category state.
    Projectile(sf::Vector2f position, sf::Vector2f velocity, bool friendly, bool isBoss = false);
    /// Integrates movement and kills the projectile on terrain impact or timeout.
    void update(float dt, const class TileMap& map);
    /// Draws the animated projectile using its selected texture.
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    /// Reports whether the projectile can damage enemies rather than players.
    bool isFriendly() const;

private:
    enum class TextureType {
        Mushroom,
        Boss
    };

    bool friendly = false;
    TextureType textureType = TextureType::Mushroom;
    float life = 4.0f;
    float animTime = 0.0f;
};
