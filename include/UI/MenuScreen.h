// Draws menu, pause, victory, and game-over overlays.
#pragma once

#include "Core/GameState.h"
#include "Entities/Player.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class MenuScreen {
public:
    void draw(sf::RenderWindow& window, GameState state, int menuIndex, const int selectedProfiles[2], int activePlayer,
              int playerCount, float volume, float gameSpeed, int selectedLevel, int unlockedLevelCount,
              int selectedDifficulty, int walletCoins, int shopIndex, bool legendUnlocked,
              const std::vector<int>& profileLevels) const;

private:
    static void drawTextLines(sf::RenderWindow& window, const std::string& body, sf::Vector2f position, unsigned size, sf::Color color);
    static void drawLevelSelect(sf::RenderWindow& window, int selectedLevel, int unlockedLevelCount);
    static void drawDifficultySelect(sf::RenderWindow& window, int selectedDifficulty);
    static void drawShop(sf::RenderWindow& window, int walletCoins, int shopIndex, bool legendUnlocked, const std::vector<int>& profileLevels);
    static void drawCharacterSelect(sf::RenderWindow& window, const int selectedProfiles[2], int activePlayer, int playerCount,
                                    bool legendUnlocked, const std::vector<int>& profileLevels);
    static void drawPause(sf::RenderWindow& window, float volume, float gameSpeed);
    static void drawSettingBar(sf::RenderWindow& window, const std::string& label, const std::string& value,
                               sf::Vector2f position, float ratio, sf::Color color);
};
