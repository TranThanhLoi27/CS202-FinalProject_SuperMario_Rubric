// Draws menu, pause, victory, and game-over overlays.
#pragma once

#include "Core/GameState.h"
#include "Entities/Player.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class MenuScreen {
public:
    static void setSelectorTexture(const sf::Texture& texture);
    void updateHover(GameState state, sf::Vector2f mousePosition, int profileCount);
    int buttonAt(GameState state, sf::Vector2f mousePosition, int profileCount) const;
    int getHoveredButton() const;
    void draw(sf::RenderWindow& window, GameState state, int menuIndex, const int selectedProfiles[2], int activePlayer,
              int playerCount, float volume, float gameSpeed, int selectedLevel, int unlockedLevelCount,
              int selectedDifficulty, int walletCoins, int shopIndex, bool legendUnlocked,
              const std::vector<int>& profileLevels) const;

private:
    static std::vector<sf::FloatRect> buttonBounds(GameState state, int profileCount);
    int hoveredButton = -1;
};
