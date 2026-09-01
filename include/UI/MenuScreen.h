// Draws menu, pause, victory, and game-over overlays.
#pragma once

#include "Core/GameState.h"
#include "Core/InputManager.h"
#include "Entities/Player.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class MenuScreen {
public:
    static void setSelectorTextures(const sf::Texture& selector1, const sf::Texture& selector2);
    void draw(sf::RenderWindow& window, GameState state, int menuIndex, const int selectedProfiles[2], int activePlayer,
              int playerCount, float volume, float gameSpeed, int selectedLevel, int unlockedLevelCount,
              int selectedDifficulty, int walletCoins, int shopIndex, bool legendUnlocked,
              const std::vector<int>& profileLevels, int pauseMenuIndex, int controlsPlayerIndex,
              int controlsActionIndex, bool rebinding, const std::string& rebindWarning,
              const InputManager& input) const;

    // Mouse interaction methods
    static int getMenuItemUnderMouse(GameState state, sf::Vector2i mousePos, int menuIndex, int selectedLevel,
                                     int unlockedLevelCount, int selectedDifficulty, int walletCoins, int shopIndex,
                                     bool legendUnlocked, const std::vector<int>& profileLevels, int pauseMenuIndex,
                                     int controlsPlayerIndex, int controlsActionIndex);

    // Viewport and scrolling support
    static int& getViewportOffset();
    static void setViewportOffset(int offset);
    static void ensureVisible(int itemIndex, GameState state);
    static int getMaxVisibleItems(GameState state);
    static int getTotalItems(GameState state, bool legendUnlocked, const std::vector<int>& profileLevels);

private:
    static void drawTextLines(sf::RenderWindow& window, const std::string& body, sf::Vector2f position, unsigned size, sf::Color color);
    static void drawLevelSelect(sf::RenderWindow& window, int selectedLevel, int unlockedLevelCount);
    static void drawDifficultySelect(sf::RenderWindow& window, int selectedDifficulty);
    static void drawShop(sf::RenderWindow& window, int walletCoins, int shopIndex, bool legendUnlocked, const std::vector<int>& profileLevels);
    static void drawCharacterSelect(sf::RenderWindow& window, const int selectedProfiles[2], int activePlayer, int playerCount,
                                    bool legendUnlocked, const std::vector<int>& profileLevels);
    static void drawPause(sf::RenderWindow& window, int pauseMenuIndex);
    static void drawControls(sf::RenderWindow& window, int controlsPlayerIndex, int controlsActionIndex,
                             bool rebinding, const std::string& rebindWarning, const InputManager& input);
    static void drawSettingBar(sf::RenderWindow& window, const std::string& label, const std::string& value,
                               sf::Vector2f position, float ratio, sf::Color color);
};
