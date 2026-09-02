#include "UI/MenuScreen.h"
#include "UI/HUD.h"
#include "Utils/Constants.h"
#include "Entities/Player.h"
#include <cmath>

namespace {
const sf::Texture* selector1Texture = nullptr;
const sf::Texture* selector2Texture = nullptr;

constexpr float kFrameWidth = 500.0f;
constexpr float kFrameHeight = 200.0f;
constexpr float kSelectorScaleX = 1.0f;
constexpr float kSelectorScaleY = 0.5f;
constexpr float kDisplayWidth = kFrameWidth * kSelectorScaleX;
constexpr float kDisplayHeight = kFrameHeight * kSelectorScaleY;
constexpr float kCenterX = (static_cast<float>(Constants::WINDOW_WIDTH) - kDisplayWidth) * 0.5f;
constexpr float kScreenCenterX = static_cast<float>(Constants::WINDOW_WIDTH) * 0.5f;
constexpr float kRowSpacing = 115.0f;

int viewportOffset = 0; // Which item is at the top of the viewport
constexpr int kMaxVisibleItems = 4; // Maximum items visible at once (for scrollable menus)

void drawScrollDots(sf::RenderWindow& window, int totalItems, int maxVisible, int currentOffset, float y) {
    if (totalItems <= maxVisible) return;

    const int totalPages = (totalItems + maxVisible - 1) / maxVisible;
    const int currentPage = currentOffset / maxVisible;

    const float dotSize = 8.0f;
    const float dotSpacing = 12.0f;
    const float startX = kScreenCenterX - (totalPages - 1) * dotSpacing * 0.5f;

    for (int i = 0; i < totalPages; ++i) {
        sf::CircleShape dot(dotSize * 0.5f);
        dot.setPosition({startX + i * dotSpacing, y});
        dot.setFillColor(i == currentPage ? sf::Color(246, 233, 190) : sf::Color(100, 100, 100));
        window.draw(dot);
    }
}
}

void row(sf::RenderWindow& window, const std::string& text, float y, bool active, sf::Color color = sf::Color::White) {
    const sf::Texture* texture = active ? selector2Texture : selector1Texture;
    if (texture) {
        sf::Sprite selector(*texture);
        selector.setScale({kSelectorScaleX, kSelectorScaleY});
        selector.setPosition({kCenterX, y});
        window.draw(selector);
    } else {
        sf::RectangleShape box({kDisplayWidth, kDisplayHeight});
        box.setPosition({kCenterX, y});
        box.setFillColor(active ? sf::Color(246, 233, 190) : sf::Color(34, 43, 48));
        window.draw(box);
    }
    HUD::drawTextCentered(window, text, {kScreenCenterX, y + kDisplayHeight * 0.5f}, 23,
                          active ? sf::Color(20, 25, 27) : color);
}

void MenuScreen::setSelectorTextures(const sf::Texture& selector1, const sf::Texture& selector2) {
    selector1Texture = &selector1;
    selector2Texture = &selector2;
}

void MenuScreen::draw(sf::RenderWindow& window, GameState state, int menuIndex, const int selectedProfiles[2], int activePlayer,
                      int playerCount, float volume, float gameSpeed, int selectedLevel, int unlockedLevelCount,
                      int selectedDifficulty, int walletCoins, int shopIndex, bool legendUnlocked,
                      const std::vector<int>& profileLevels, int pauseMenuIndex, int controlsPlayerIndex,
                      int controlsActionIndex, bool rebinding, const std::string& rebindWarning,
                      const InputManager& input, int hoveredMenuItem) const {
    (void)volume;
    (void)gameSpeed;
    if (state == GameState::Playing) return;
    sf::RectangleShape shade(window.getView().getSize());
    shade.setFillColor({10, 13, 17, 225});
    window.draw(shade);

    if (state == GameState::Menu) {
        HUD::drawTextCentered(window, "CO-OP PLATFORMER", {kScreenCenterX, 25.0f}, 46, {246, 233, 190});
        const char* entries[] = {"1 Player", "2 Players", "Choose Difficulty", "Shop", "Info", "Reset Game", "Quit"};
        const int maxVisible = getMaxVisibleItems(state);
        const int totalItems = 7;
        const int endItem = std::min(viewportOffset + maxVisible, totalItems);
        for (int i = viewportOffset; i < endItem; ++i) {
            row(window, entries[i], 75.0f + kRowSpacing * (i - viewportOffset), i == hoveredMenuItem);
        }
        HUD::drawTextCentered(window, "Up/Down / Scroll: choose   Enter/Click: confirm", {kScreenCenterX, 565.0f}, 20, {204, 213, 210});
        drawScrollDots(window, totalItems, maxVisible, viewportOffset, 595.0f);
        return;
    }
    if (state == GameState::LevelSelect) {
        HUD::drawTextCentered(window, "CHOOSE MAP", {kScreenCenterX, 30.0f}, 46, {246, 233, 190});
        const char* maps[] = {"Forest Trail", "Stone Bridge", "Spike Valley", "Boss Lair"};
        const int maxVisible = getMaxVisibleItems(state);
        const int totalItems = 5;
        const int endItem = std::min(viewportOffset + maxVisible, totalItems);
        for (int i = viewportOffset; i < endItem; ++i) {
            if (i < 4) {
                row(window, std::to_string(i + 1) + ". " + maps[i] + (i >= unlockedLevelCount ? "  [LOCKED]" : ""),
                    80.0f + kRowSpacing * (i - viewportOffset), i == hoveredMenuItem,
                    i < unlockedLevelCount ? sf::Color::White : sf::Color(140, 140, 140));
            } else {
                row(window, "Back", 80.0f + kRowSpacing * (i - viewportOffset), i == hoveredMenuItem);
            }
        }
        HUD::drawTextCentered(window, "Scroll and left-click an option", {kScreenCenterX, 570.0f}, 20, {204, 213, 210});
        drawScrollDots(window, totalItems, maxVisible, viewportOffset, 600.0f);
        return;
    }
    if (state == GameState::DifficultySelect) {
        HUD::drawTextCentered(window, "CHOOSE DIFFICULTY", {kScreenCenterX, 40.0f}, 46, {246, 233, 190});
        const char* levels[] = {"Easy", "Medium (+1 enemy HP)", "Hard (+2 enemy HP)"};
        const int maxVisible = getMaxVisibleItems(state);
        const int totalItems = 4;
        const int endItem = std::min(viewportOffset + maxVisible, totalItems);
        for (int i = viewportOffset; i < endItem; ++i) {
            if (i < 3) {
                row(window, levels[i], 140.0f + kRowSpacing * (i - viewportOffset), i == hoveredMenuItem);
            } else {
                row(window, "Back", 140.0f + kRowSpacing * (i - viewportOffset), i == hoveredMenuItem);
            }
        }
        drawScrollDots(window, totalItems, maxVisible, viewportOffset, 600.0f);
        return;
    }
    if (state == GameState::CharacterSelect) {
        HUD::drawTextCentered(window, "CHOOSE CHARACTER", {kScreenCenterX, 25.0f}, 46, {246, 233, 190});
        const auto& profiles = Player::profiles();
        const int maxVisible = getMaxVisibleItems(state);
        const int totalItems = static_cast<int>(profiles.size()) + 1;
        const int endItem = std::min(viewportOffset + maxVisible, totalItems);
        for (int i = viewportOffset; i < endItem; ++i) {
            if (i < static_cast<int>(profiles.size())) {
                const bool locked = i == static_cast<int>(profiles.size()) - 1 && !legendUnlocked;
                std::string text = profiles[i].name + " - " + profiles[i].skill;
                if (locked) text += " [LOCKED]";
                const bool selected = i == selectedProfiles[0] || (playerCount == 2 && i == selectedProfiles[1]);
                row(window, text, 70.0f + kRowSpacing * (i - viewportOffset),
                    selected || i == hoveredMenuItem,
                    locked ? sf::Color(145, 125, 125) : profiles[i].color);
            } else {
                row(window, "Back", 70.0f + kRowSpacing * (i - viewportOffset), i == hoveredMenuItem);
            }
        }
        const std::string selection = playerCount == 1 ? "P1" : "P1 / P2";
        HUD::drawTextCentered(window, selection + ": W/S, Up/Down, A/D choose   Enter/Click: start", {kScreenCenterX, 560.0f}, 19, {204, 213, 210});
        HUD::drawTextCentered(window, "Scroll: Mouse wheel   Active: Player " + std::to_string(activePlayer + 1), {kScreenCenterX, 590.0f}, 19, {246, 233, 190});
        drawScrollDots(window, totalItems, maxVisible, viewportOffset, 620.0f);
        return;
    }
    if (state == GameState::Shop) {
        HUD::drawTextCentered(window, "SHOP - Coins: " + std::to_string(walletCoins), {kScreenCenterX, 25.0f}, 42, {246, 233, 190});
        const auto& profiles = Player::profiles();
        const int maxVisible = getMaxVisibleItems(state);
        const int totalItems = static_cast<int>(profiles.size()) + 1; // +1 for "Back"
        const int endItem = std::min(viewportOffset + maxVisible, totalItems);
        for (int i = viewportOffset; i < endItem; ++i) {
            if (i < static_cast<int>(profiles.size())) {
                row(window, profiles[i].name + "  Lv " + std::to_string(profileLevels[i]) +
                        (i == static_cast<int>(profiles.size()) - 1 && !legendUnlocked ? "  Buy Legend: 80" : "  Upgrade: 20"),
                    68.0f + kRowSpacing * (i - viewportOffset), i == hoveredMenuItem);
            } else {
                row(window, "Back", 68.0f + kRowSpacing * (i - viewportOffset), i == hoveredMenuItem);
            }
        }
        drawScrollDots(window, totalItems, maxVisible, viewportOffset, 560.0f);
        return;
    }
    if (state == GameState::Paused) {
        drawPause(window, hoveredMenuItem);
        return;
    }
    if (state == GameState::Controls) {
        drawControls(window, controlsPlayerIndex, controlsActionIndex, rebinding, rebindWarning, input,
                     hoveredMenuItem);
        // Show scroll indicator for controls menu
        const int totalItems = static_cast<int>(InputManager::ActionCount) + 1;
        const int maxVisible = getMaxVisibleItems(state);
        drawScrollDots(window, totalItems, maxVisible, viewportOffset, 560.0f);
        return;
    }
    if (state == GameState::Info) {
        HUD::drawTextCentered(window, "INFO", {kScreenCenterX, 52.0f}, 48, {246, 233, 190});
        HUD::drawTextCentered(window, "Cooperate, defeat enemies, collect loot, and reach the goal.",
                              {kScreenCenterX, 135.0f}, 22, sf::Color::White);
        row(window, "Back", 220.0f, hoveredMenuItem == 0);
        return;
    }
    const char* message = state == GameState::Victory ? "VICTORY" : state == GameState::GameOver ? "GAME OVER" : "INFO";
    HUD::drawTextCentered(window, message, {kScreenCenterX, 52.0f}, 48, {246, 233, 190});
    row(window, "Restart", 150.0f, hoveredMenuItem == 0);
    row(window, "Main Menu", 265.0f, hoveredMenuItem == 1);
}

void MenuScreen::drawPause(sf::RenderWindow& window, int pauseMenuIndex) {
    HUD::drawTextCentered(window, "PAUSED", {kScreenCenterX, 52.0f}, 48, {246, 233, 190});
    const char* entries[] = {"Resume", "Change Controls", "Main Menu"};
    const int maxVisible = getMaxVisibleItems(GameState::Paused);
    const int endItem = std::min(viewportOffset + maxVisible, 3);
    for (int i = viewportOffset; i < endItem; ++i) {
        row(window, entries[i], 140.0f + kRowSpacing * (i - viewportOffset), i == pauseMenuIndex);
    }
    HUD::drawTextCentered(window, "Up/Down: choose   Enter/Click: confirm   Escape: resume", {kScreenCenterX, 550.0f}, 20, {204, 213, 210});
}

void MenuScreen::drawControls(sf::RenderWindow& window, int controlsPlayerIndex, int controlsActionIndex,
                              bool rebinding, const std::string& rebindWarning, const InputManager& input,
                              int hoveredMenuItem) {
    const float tabWidth = 260.0f;
    const float tabHeight = 44.0f;
    const float p1TabX = kScreenCenterX - tabWidth - 15.0f;
    const float p2TabX = kScreenCenterX + 15.0f;
    const float tabY = 18.0f;

    // Draw Player 1 Tab Selector
    sf::RectangleShape p1Box({tabWidth, tabHeight});
    p1Box.setPosition({p1TabX, tabY});
    p1Box.setFillColor(controlsPlayerIndex == 0 ? sf::Color(246, 233, 190) : sf::Color(34, 43, 48));
    p1Box.setOutlineThickness(2.0f);
    p1Box.setOutlineColor(controlsPlayerIndex == 0 ? sf::Color(255, 255, 255) : sf::Color(80, 80, 80));
    window.draw(p1Box);
    HUD::drawTextCentered(window, "Player 1 Controls", {p1TabX + tabWidth * 0.5f, tabY + tabHeight * 0.5f}, 21,
                          controlsPlayerIndex == 0 ? sf::Color(20, 25, 27) : sf::Color(204, 213, 210));

    // Draw Player 2 Tab Selector
    sf::RectangleShape p2Box({tabWidth, tabHeight});
    p2Box.setPosition({p2TabX, tabY});
    p2Box.setFillColor(controlsPlayerIndex == 1 ? sf::Color(246, 233, 190) : sf::Color(34, 43, 48));
    p2Box.setOutlineThickness(2.0f);
    p2Box.setOutlineColor(controlsPlayerIndex == 1 ? sf::Color(255, 255, 255) : sf::Color(80, 80, 80));
    window.draw(p2Box);
    HUD::drawTextCentered(window, "Player 2 Controls", {p2TabX + tabWidth * 0.5f, tabY + tabHeight * 0.5f}, 21,
                          controlsPlayerIndex == 1 ? sf::Color(20, 25, 27) : sf::Color(204, 213, 210));

    const int maxVisible = getMaxVisibleItems(GameState::Controls);
    const int totalItems = static_cast<int>(InputManager::ActionCount) + 1;
    const int endItem = std::min(viewportOffset + maxVisible, totalItems);
    for (int i = viewportOffset; i < endItem; ++i) {
        if (i < InputManager::ActionCount) {
            const std::string actionName = InputManager::getActionName(i);
            const std::string keyName = InputManager::getKeyName(input.getKey(controlsPlayerIndex, i));
            const std::string label = actionName + "  -  " + keyName;
            row(window, label, 72.0f + kRowSpacing * (i - viewportOffset),
                i == hoveredMenuItem || (rebinding && i == controlsActionIndex));
        } else {
            row(window, "Back", 72.0f + kRowSpacing * (i - viewportOffset), i == hoveredMenuItem);
        }
    }

    if (rebinding) {
        HUD::drawTextCentered(window, "Press an unassigned key...", {kScreenCenterX, 560.0f}, 24, {246, 233, 190});
        HUD::drawTextCentered(window, "Escape: cancel", {kScreenCenterX, 595.0f}, 20, {204, 213, 210});
    } else {
        HUD::drawTextCentered(window, "Click tab to switch player   Click key row to rebind   Escape: back", {kScreenCenterX, 560.0f}, 20, {204, 213, 210});
    }

    if (!rebindWarning.empty()) {
        HUD::drawTextCentered(window, rebindWarning, {kScreenCenterX, 620.0f}, 22, {255, 120, 120});
    }
}

int MenuScreen::getMenuItemUnderMouse(GameState state, sf::Vector2f mousePos, int menuIndex, int selectedLevel,
                                      int unlockedLevelCount, int selectedDifficulty, int walletCoins, int shopIndex,
                                      bool legendUnlocked, const std::vector<int>& profileLevels, int pauseMenuIndex,
                                      int controlsPlayerIndex, int controlsActionIndex) {
    (void)menuIndex;
    (void)selectedLevel;
    (void)unlockedLevelCount;
    (void)selectedDifficulty;
    (void)walletCoins;
    (void)shopIndex;
    (void)legendUnlocked;
    (void)profileLevels;
    (void)pauseMenuIndex;
    (void)controlsPlayerIndex;
    (void)controlsActionIndex;

    const float mouseX = mousePos.x;
    const float mouseY = mousePos.y;

    if (state == GameState::Controls) {
        const float tabWidth = 260.0f;
        const float tabHeight = 44.0f;
        const float p1TabX = kScreenCenterX - tabWidth - 15.0f;
        const float p2TabX = kScreenCenterX + 15.0f;
        const float tabY = 18.0f;

        if (mouseY >= tabY && mouseY <= tabY + tabHeight) {
            if (mouseX >= p1TabX && mouseX <= p1TabX + tabWidth) return -10; // Player 1 tab
            if (mouseX >= p2TabX && mouseX <= p2TabX + tabWidth) return -11; // Player 2 tab
        }
    }

    // Check if mouse is within the horizontal bounds of menu items
    if (mouseX < kCenterX || mouseX > kCenterX + kDisplayWidth) {
        return -1;
    }

    float startY = 0.0f;
    int totalItems = getTotalItems(state, legendUnlocked, profileLevels);

    // Get starting Y position based on state
    if (state == GameState::Menu) startY = 75.0f;
    else if (state == GameState::LevelSelect) startY = 80.0f;
    else if (state == GameState::DifficultySelect) startY = 140.0f;
    else if (state == GameState::CharacterSelect) startY = 70.0f;
    else if (state == GameState::Shop) startY = 68.0f;
    else if (state == GameState::Paused) startY = 140.0f;
    else if (state == GameState::Controls) startY = 72.0f;
    else if (state == GameState::Info) startY = 220.0f;
    else if (state == GameState::Victory || state == GameState::GameOver) startY = 150.0f;

    // Check items within viewport
    const int maxVisible = getMaxVisibleItems(state);
    const int endItem = std::min(viewportOffset + maxVisible, totalItems);

    for (int i = viewportOffset; i < endItem; ++i) {
        const float itemY = startY + kRowSpacing * (i - viewportOffset);
        if (mouseY >= itemY && mouseY <= itemY + kDisplayHeight) {
            return i; // Return the actual item index, not the viewport-relative index
        }
    }

    return -1;
}

int& MenuScreen::getViewportOffset() {
    return viewportOffset;
}

void MenuScreen::setViewportOffset(int offset) {
    viewportOffset = offset;
}

void MenuScreen::ensureVisible(int itemIndex, GameState state) {
    const int maxVisible = getMaxVisibleItems(state);
    if (itemIndex < viewportOffset) {
        viewportOffset = itemIndex;
    } else if (itemIndex >= viewportOffset + maxVisible) {
        viewportOffset = itemIndex - maxVisible + 1;
    }
}

int MenuScreen::getMaxVisibleItems(GameState state) {
    if (state == GameState::Menu) return 4; // Scrollable (7 items total)
    if (state == GameState::LevelSelect) return 4;
    if (state == GameState::DifficultySelect) return 3;
    if (state == GameState::Paused) return 3;
    if (state == GameState::Controls) return 4;
    if (state == GameState::CharacterSelect) return 4;
    if (state == GameState::Shop) return 4;
    if (state == GameState::Info) return 1;
    if (state == GameState::Victory || state == GameState::GameOver) return 2;
    return kMaxVisibleItems;
}

int MenuScreen::getTotalItems(GameState state, bool legendUnlocked, const std::vector<int>& profileLevels) {
    (void)legendUnlocked;
    (void)profileLevels;

    if (state == GameState::Menu) return 7;
    if (state == GameState::LevelSelect) return 5;
    if (state == GameState::DifficultySelect) return 4;
    if (state == GameState::Paused) return 3;
    if (state == GameState::Controls) return static_cast<int>(InputManager::ActionCount) + 1;
    if (state == GameState::CharacterSelect) return static_cast<int>(Player::profiles().size()) + 1;
    if (state == GameState::Shop) return static_cast<int>(Player::profiles().size()) + 1; // +1 for "Back"
    if (state == GameState::Info) return 1;
    if (state == GameState::Victory || state == GameState::GameOver) return 2;
    return 0;
}
