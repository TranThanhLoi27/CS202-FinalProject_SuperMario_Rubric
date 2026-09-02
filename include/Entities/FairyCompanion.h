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
    static void setTexture(const sf::Texture& texture);

    void initForPlayers(std::size_t playerCount);
    void update(float dt, const std::vector<std::unique_ptr<Player>>& players);
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const;
    void createFireworks(sf::Vector2f position, int count = 10);

private:
    static const sf::Texture* fairyTexture;
    std::vector<std::vector<Fairy>> playerFairies;
};
