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
    /// Registers normal and highlighted selector textures.
    static void setSelectorTextures(const sf::Texture& selector1, const sf::Texture& selector2);
    /// Draws the overlay associated with the current application state.
    void draw(sf::RenderWindow& window, GameState state, int menuIndex, const int selectedProfiles[2], int activePlayer,
              int playerCount, float volume, float gameSpeed, int selectedLevel, int unlockedLevelCount,
              int selectedDifficulty, int walletCoins, int shopIndex, bool legendUnlocked,
              const std::vector<int>& profileLevels, int pauseMenuIndex, int controlsPlayerIndex,
              int controlsActionIndex, bool rebinding, const std::string& rebindWarning,
              const InputManager& input, int hoveredMenuItem) const;

    /// Returns the menu row or special player tab under the mouse.
    static int getMenuItemUnderMouse(GameState state, sf::Vector2f mousePos, int menuIndex, int selectedLevel,
                                     int unlockedLevelCount, int selectedDifficulty, int walletCoins, int shopIndex,
                                     bool legendUnlocked, const std::vector<int>& profileLevels, int pauseMenuIndex,
                                     int controlsPlayerIndex, int controlsActionIndex);

    /// Returns mutable access to the shared scroll offset.
    static int& getViewportOffset();
    /// Replaces the shared scroll offset.
    static void setViewportOffset(int offset);
    /// Scrolls far enough to keep a logical row visible.
    static void ensureVisible(int itemIndex, GameState state);
    /// Returns the maximum visible rows for a state.
    static int getMaxVisibleItems(GameState state);
    /// Returns the total logical rows for a state.
    static int getTotalItems(GameState state, bool legendUnlocked, const std::vector<int>& profileLevels);

private:
    /// Draws multiple lines of explanatory text.
    static void drawTextLines(sf::RenderWindow& window, const std::string& body, sf::Vector2f position, unsigned size, sf::Color color);
    /// Draws the legacy level-selection layout.
    static void drawLevelSelect(sf::RenderWindow& window, int selectedLevel, int unlockedLevelCount);
    /// Draws the legacy difficulty-selection layout.
    static void drawDifficultySelect(sf::RenderWindow& window, int selectedDifficulty);
    /// Draws the legacy shop layout.
    static void drawShop(sf::RenderWindow& window, int walletCoins, int shopIndex, bool legendUnlocked, const std::vector<int>& profileLevels);
    /// Draws the legacy character-selection layout.
    static void drawCharacterSelect(sf::RenderWindow& window, const int selectedProfiles[2], int activePlayer, int playerCount,
                                    bool legendUnlocked, const std::vector<int>& profileLevels);
    /// Draws pause-menu rows.
    static void drawPause(sf::RenderWindow& window, int pauseMenuIndex);
    /// Draws player tabs and the scrollable key-binding rows.
    static void drawControls(sf::RenderWindow& window, int controlsPlayerIndex, int controlsActionIndex,
                             bool rebinding, const std::string& rebindWarning, const InputManager& input,
                             int hoveredMenuItem);
    /// Draws a labeled legacy settings bar.
    static void drawSettingBar(sf::RenderWindow& window, const std::string& label, const std::string& value,
                               sf::Vector2f position, float ratio, sf::Color color);
};
