#include "Entities/FairyCompanion.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>

const sf::Texture* FairyCompanionManager::fairyTexture = nullptr;

void FairyCompanionManager::setTexture(const sf::Texture& texture) {
    fairyTexture = &texture;
}

void FairyCompanionManager::initForPlayers(std::size_t playerCount) {
    playerFairies.clear();
    playerFairies.resize(playerCount);

    for (std::size_t p = 0; p < playerCount; ++p) {
        // Create 3 fairies per player
        constexpr int count = 3;
        for (int i = 0; i < count; ++i) {
            Fairy f;
            f.angleOffset = (2.0f * 3.14159265f / count) * i;
            f.phase = static_cast<float>(i) * 1.5f;
            f.flutterSpeed = 4.0f + static_cast<float>(i) * 0.5f;
            f.position = {0.0f, 0.0f};
            f.velocity = {0.0f, 0.0f};
            playerFairies[p].push_back(f);
        }
    }
}

void FairyCompanionManager::update(float dt, const std::vector<std::unique_ptr<Player>>& players) {
    if (playerFairies.size() != players.size()) {
        initForPlayers(players.size());
    }

    for (std::size_t p = 0; p < players.size(); ++p) {
        if (!players[p] || players[p]->isRespawning()) continue;

        const sf::Vector2f playerCenter = players[p]->position + players[p]->size * 0.5f;
        const sf::Vector2f playerVel = players[p]->velocity;
        const float speed = std::sqrt(playerVel.x * playerVel.x + playerVel.y * playerVel.y);
        const bool fastMoving = speed > 150.0f;

        for (auto& fairy : playerFairies[p]) {
            fairy.phase += dt * (fastMoving ? 12.0f : fairy.flutterSpeed);

            sf::Vector2f targetPos;
            if (fastMoving) {
                // Trail formation behind movement direction
                const sf::Vector2f moveDir = playerVel / speed;
                const float trailDist = 35.0f + std::sin(fairy.phase) * 12.0f;
                const float perpOffset = std::cos(fairy.phase * 1.5f) * 18.0f;
                const sf::Vector2f perpDir(-moveDir.y, moveDir.x);

                targetPos = playerCenter - moveDir * trailDist + perpDir * perpOffset;

                // Spawn sparkle particle during fast movement
                if (fairy.particles.size() < 12) {
                    FairyParticle part;
                    part.position = fairy.position + sf::Vector2f((std::rand() % 10 - 5) * 0.8f, (std::rand() % 10 - 5) * 0.8f);
                    part.velocity = {0.0f, 0.0f};
                    part.scale = 0.6f;
                    part.alpha = 240.0f;
                    part.lifetime = 0.22f;
                    part.maxLifetime = 0.22f;
                    part.color = sf::Color::White;
                    fairy.particles.push_back(part);
                }
            } else {
                // Orbit floating around player
                const float orbitRadius = 38.0f + std::sin(fairy.phase * 0.8f) * 8.0f;
                const float currentAngle = fairy.angleOffset + fairy.phase * 0.6f;
                const float floatY = std::sin(fairy.phase * 2.0f) * 6.0f;

                targetPos = playerCenter + sf::Vector2f(
                    std::cos(currentAngle) * orbitRadius,
                    std::sin(currentAngle) * orbitRadius + floatY - 10.0f
                );
            }

            // Smooth spring easing toward target position
            const sf::Vector2f delta = targetPos - fairy.position;
            fairy.position += delta * std::min(1.0f, dt * (fastMoving ? 18.0f : 8.0f));

            // Scale and alpha pulse
            fairy.currentScale = 0.75f + std::sin(fairy.phase * 3.0f) * 0.15f;
            if (fastMoving) fairy.currentScale *= 1.15f;

            // Update particles
            for (auto& part : fairy.particles) {
                part.lifetime -= dt;
                part.scale *= 0.94f;
                part.alpha = (part.lifetime / part.maxLifetime) * 240.0f;

                // Move particles if they have velocity (fireworks)
                if (part.velocity.x != 0.0f || part.velocity.y != 0.0f) {
                    part.position += part.velocity * dt;
                    part.velocity *= 0.98f; // Friction
                }
            }

            fairy.particles.erase(
                std::remove_if(fairy.particles.begin(), fairy.particles.end(),
                               [](const FairyParticle& pt) { return pt.lifetime <= 0.0f; }),
                fairy.particles.end()
            );
        }
    }
}

void FairyCompanionManager::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    if (!fairyTexture) return;

    sf::Sprite sprite(*fairyTexture);
    const sf::Vector2u texSize = fairyTexture->getSize();
    sprite.setOrigin({texSize.x * 0.5f, texSize.y * 0.5f});

    for (const auto& fairies : playerFairies) {
        for (const auto& fairy : fairies) {
            // Draw particles first
            for (const auto& part : fairy.particles) {
                sprite.setPosition(part.position - camera);
                sprite.setScale({part.scale, part.scale});
                sprite.setColor(sf::Color(part.color.r, part.color.g, part.color.b, static_cast<std::uint8_t>(std::clamp(part.alpha, 0.0f, 255.0f))));
                window.draw(sprite);
            }

            // Draw main fairy
            sprite.setPosition(fairy.position - camera);
            sprite.setScale({fairy.currentScale, fairy.currentScale});
            sprite.setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(std::clamp(fairy.currentAlpha, 0.0f, 255.0f))));
            window.draw(sprite);
        }
    }
}

void FairyCompanionManager::createFireworks(sf::Vector2f position, int count) {
    if (playerFairies.empty()) {
        playerFairies.resize(1);
    }

    // Create temporary fireworks particles
    for (auto& fairies : playerFairies) {
        for (auto& fairy : fairies) {
            // Clear existing particles and add fireworks
            fairy.particles.clear();

            for (int i = 0; i < count; ++i) {
                FairyParticle part;
                const float angle = (2.0f * 3.14159265f / count) * i;
                const float speed = 150.0f + (std::rand() % 100);
                part.position = position;
                part.velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
                part.scale = 1.5f;
                part.alpha = 255.0f;
                part.lifetime = 1.0f;
                part.maxLifetime = 1.0f;

                // Random colors for fireworks
                const int colorChoice = std::rand() % 5;
                switch (colorChoice) {
                    case 0: part.color = sf::Color(255, 100, 100); break; // Red
                    case 1: part.color = sf::Color(100, 255, 100); break; // Green
                    case 2: part.color = sf::Color(100, 100, 255); break; // Blue
                    case 3: part.color = sf::Color(255, 255, 100); break; // Yellow
                    case 4: part.color = sf::Color(255, 100, 255); break; // Purple
                }

                fairy.particles.push_back(part);
            }
        }
    }
}
