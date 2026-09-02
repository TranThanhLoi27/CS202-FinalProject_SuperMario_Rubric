#pragma once

#include "Entities/Player.h"
#include <SFML/Graphics.hpp>
#include <vector>

struct FairyParticle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float scale = 1.0f;
    float alpha = 255.0f;
    float lifetime = 0.25f;
    float maxLifetime = 0.25f;
    sf::Color color = sf::Color::White;
};

struct Fairy {
    sf::Vector2f position = {0.0f, 0.0f};
    sf::Vector2f velocity = {0.0f, 0.0f};
    float angleOffset = 0.0f;
    float phase = 0.0f;
    float flutterSpeed = 3.0f;
    float currentScale = 0.8f;
    float currentAlpha = 230.0f;
    std::vector<FairyParticle> particles;
};

class FairyCompanionManager {
public:
    /// Registers the shared fairy texture owned by AssetManager.
    static void setTexture(const sf::Texture& texture);

    /// Creates the companion groups required for the active player count.
    void initForPlayers(std::size_t playerCount);
    /// Updates orbiting fairies and expires trail particles.
    void update(float dt, const std::vector<std::unique_ptr<Player>>& players);
    /// Draws fairies and particles relative to the active camera.
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const;
    /// Spawns a radial burst of decorative fairy particles.
    void createFireworks(sf::Vector2f position, int count = 10);

private:
    static const sf::Texture* fairyTexture;
    std::vector<std::vector<Fairy>> playerFairies;
};
