#include "UI/MenuScreen.h"
#include "UI/HUD.h"
#include "Utils/Constants.h"
#include "Entities/Player.h"
#include <cmath>

namespace {
const sf::Texture* selector1Texture = nullptr;
const sf::Texture* selector2Texture = nullptr;

constexpr float kFrameWidth = 512.0f;
constexpr float kFrameHeight = 192.0f;
constexpr float kBaseRowHeight = 58.0f;
constexpr float kSelectorScaleX = (kBaseRowHeight / kFrameHeight) * 4.0f;
constexpr float kSelectorScaleY = (kBaseRowHeight / kFrameHeight) * 2.0f;
constexpr float kDisplayWidth = kFrameWidth * kSelectorScaleX;
constexpr float kDisplayHeight = kFrameHeight * kSelectorScaleY;
constexpr float kCenterX = (static_cast<float>(Constants::WINDOW_WIDTH) - kDisplayWidth) * 0.5f;
constexpr float kScreenCenterX = static_cast<float>(Constants::WINDOW_WIDTH) * 0.5f;
constexpr float kRowSpacing = 120.0f;

int viewportOffset = 0; // Which item is at the top of the viewport
constexpr int kMaxVisibleItems = 6; // Maximum items visible at once (for scrollable menus)

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
                      const InputManager& input) const {
    (void)volume;
    (void)gameSpeed;
    if (state == GameState::Playing) return;
    sf::RectangleShape shade(window.getView().getSize());
    shade.setFillColor({10, 13, 17, 225});
    window.draw(shade);

    if (state == GameState::Menu) {
        HUD::drawTextCentered(window, "CO-OP PLATFORMER", {kScreenCenterX, 40.0f}, 46, {246, 233, 190});
        const char* entries[] = {"1 Player", "2 Players", "Choose Difficulty", "Shop", "Info", "Quit"};
        const int maxVisible = getMaxVisibleItems(state);
        const int endItem = std::min(viewportOffset + maxVisible, 6);
        for (int i = viewportOffset; i < endItem; ++i) {
            row(window, entries[i], 78.0f + kRowSpacing * (i - viewportOffset), i == menuIndex);
        }
        HUD::drawTextCentered(window, "Up/Down: choose   Enter: confirm", {kScreenCenterX, 660.0f}, 20, {204, 213, 210});
        return;
    }
    if (state == GameState::LevelSelect) {
        HUD::drawTextCentered(window, "CHOOSE MAP", {kScreenCenterX, 40.0f}, 46, {246, 233, 190});
        const char* maps[] = {"Forest Trail", "Stone Bridge", "Spike Valley", "Boss Lair"};
        const int maxVisible = getMaxVisibleItems(state);
        const int endItem = std::min(viewportOffset + maxVisible, 4);
        for (int i = viewportOffset; i < endItem; ++i) {
            row(window, std::to_string(i + 1) + ". " + maps[i] + (i >= unlockedLevelCount ? "  [LOCKED]" : ""),
                100.0f + kRowSpacing * (i - viewportOffset), i == selectedLevel,
                i < unlockedLevelCount ? sf::Color::White : sf::Color(140, 140, 140));
        }
        HUD::drawTextCentered(window, "Up/Down: choose   Enter: confirm   Escape: back", {kScreenCenterX, 560.0f}, 20, {204, 213, 210});
        return;
    }
    if (state == GameState::DifficultySelect) {
        HUD::drawTextCentered(window, "CHOOSE DIFFICULTY", {kScreenCenterX, 40.0f}, 46, {246, 233, 190});
        const char* levels[] = {"Easy", "Medium (+1 enemy HP)", "Hard (+2 enemy HP)"};
        const int maxVisible = getMaxVisibleItems(state);
        const int endItem = std::min(viewportOffset + maxVisible, 3);
        for (int i = viewportOffset; i < endItem; ++i) {
            row(window, levels[i], 160.0f + kRowSpacing * (i - viewportOffset), i == selectedDifficulty);
        }
        return;
    }
    if (state == GameState::CharacterSelect) {
        HUD::drawTextCentered(window, "CHOOSE CHARACTER", {kScreenCenterX, 36.0f}, 46, {246, 233, 190});
        const auto& profiles = Player::profiles();
        const int maxVisible = getMaxVisibleItems(state);
        const int totalItems = static_cast<int>(profiles.size());
        const int endItem = std::min(viewportOffset + maxVisible, totalItems);
        for (int i = viewportOffset; i < endItem; ++i) {
            const bool locked = i == static_cast<int>(profiles.size()) - 1 && !legendUnlocked;
            std::string text = profiles[i].name + " - " + profiles[i].skill;
            if (locked) text += " [LOCKED]";
            const bool selected = i == selectedProfiles[0] || (playerCount == 2 && i == selectedProfiles[1]);
            row(window, text, 72.0f + kRowSpacing * (i - viewportOffset), selected, locked ? sf::Color(145, 125, 125) : profiles[i].color);
        }
        const std::string selection = playerCount == 1 ? "P1" : "P1 / P2";
        HUD::drawTextCentered(window, selection + ": A/D, Left/Right choose   Enter: start", {kScreenCenterX, 660.0f}, 19, {204, 213, 210});
        HUD::drawTextCentered(window, "Scroll: Mouse wheel   Active: Player " + std::to_string(activePlayer + 1), {kScreenCenterX, 690.0f}, 19, {246, 233, 190});
        drawScrollDots(window, totalItems, maxVisible, viewportOffset, 720.0f);
        return;
    }
    if (state == GameState::Shop) {
        HUD::drawTextCentered(window, "SHOP - Coins: " + std::to_string(walletCoins), {kScreenCenterX, 36.0f}, 42, {246, 233, 190});
        const auto& profiles = Player::profiles();
        const int maxVisible = getMaxVisibleItems(state);
        const int totalItems = static_cast<int>(profiles.size()) + 1; // +1 for "Back"
        const int endItem = std::min(viewportOffset + maxVisible, totalItems);
        for (int i = viewportOffset; i < endItem; ++i) {
            if (i < static_cast<int>(profiles.size())) {
                row(window, profiles[i].name + "  Lv " + std::to_string(profileLevels[i]) +
                        (i == static_cast<int>(profiles.size()) - 1 && !legendUnlocked ? "  Buy Legend: 50" : "  Upgrade: 20"),
                    68.0f + kRowSpacing * (i - viewportOffset), i == shopIndex);
            } else {
                row(window, "Back", 68.0f + kRowSpacing * (i - viewportOffset), shopIndex == i);
            }
        }
        drawScrollDots(window, totalItems, maxVisible, viewportOffset, 700.0f);
        return;
    }
    if (state == GameState::Paused) {
        drawPause(window, pauseMenuIndex);
        return;
    }
    if (state == GameState::Controls) {
        drawControls(window, controlsPlayerIndex, controlsActionIndex, rebinding, rebindWarning, input);
        // Show scroll indicator for controls menu
        const int totalItems = static_cast<int>(InputManager::ActionCount);
        const int maxVisible = getMaxVisibleItems(state);
        drawScrollDots(window, totalItems, maxVisible, viewportOffset, 700.0f);
        return;
    }
    const char* message = state == GameState::Victory ? "VICTORY" : state == GameState::GameOver ? "GAME OVER" : "INFO";
    HUD::drawTextCentered(window, message, {kScreenCenterX, 52.0f}, 48, {246, 233, 190});
    HUD::drawTextCentered(window, "Enter: restart   Escape: menu", {kScreenCenterX, 180.0f}, 24, sf::Color::White);
}

void MenuScreen::drawPause(sf::RenderWindow& window, int pauseMenuIndex) {
    HUD::drawTextCentered(window, "PAUSED", {kScreenCenterX, 52.0f}, 48, {246, 233, 190});
    const char* entries[] = {"Resume", "Change Controls", "Main Menu"};
    const int maxVisible = getMaxVisibleItems(GameState::Paused);
    const int endItem = std::min(viewportOffset + maxVisible, 3);
    for (int i = viewportOffset; i < endItem; ++i) {
        row(window, entries[i], 160.0f + kRowSpacing * (i - viewportOffset), i == pauseMenuIndex);
    }
    HUD::drawTextCentered(window, "Up/Down: choose   Enter: confirm   Escape: resume", {kScreenCenterX, 560.0f}, 20, {204, 213, 210});
}

void MenuScreen::drawControls(sf::RenderWindow& window, int controlsPlayerIndex, int controlsActionIndex,
                              bool rebinding, const std::string& rebindWarning, const InputManager& input) {
    const std::string playerLabel = "Player " + std::to_string(controlsPlayerIndex + 1) + " Controls";
    HUD::drawTextCentered(window, playerLabel, {kScreenCenterX, 36.0f}, 42, {246, 233, 190});

    const int maxVisible = getMaxVisibleItems(GameState::Controls);
    const int endItem = std::min(viewportOffset + maxVisible, static_cast<int>(InputManager::ActionCount));
    for (int i = viewportOffset; i < endItem; ++i) {
        const std::string actionName = InputManager::getActionName(i);
        const std::string keyName = InputManager::getKeyName(input.getKey(controlsPlayerIndex, i));
        const std::string label = actionName + "  -  " + keyName;
        row(window, label, 80.0f + kRowSpacing * (i - viewportOffset), i == controlsActionIndex);
    }

    if (rebinding) {
        HUD::drawTextCentered(window, "Press an unassigned key...", {kScreenCenterX, 620.0f}, 24, {246, 233, 190});
        HUD::drawTextCentered(window, "Escape: cancel", {kScreenCenterX, 655.0f}, 20, {204, 213, 210});
    } else {
        HUD::drawTextCentered(window, "Left/Right: switch player   Enter: rebind   Escape: back", {kScreenCenterX, 620.0f}, 20, {204, 213, 210});
    }

    if (!rebindWarning.empty()) {
        HUD::drawTextCentered(window, rebindWarning, {kScreenCenterX, 690.0f}, 22, {255, 120, 120});
    }
}

int MenuScreen::getMenuItemUnderMouse(GameState state, sf::Vector2i mousePos, int menuIndex, int selectedLevel,
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

    const float mouseX = static_cast<float>(mousePos.x);
    const float mouseY = static_cast<float>(mousePos.y);

    // Check if mouse is within the horizontal bounds of menu items
    if (mouseX < kCenterX || mouseX > kCenterX + kDisplayWidth) {
        return -1;
    }

    float startY = 0.0f;
    int totalItems = getTotalItems(state, legendUnlocked, profileLevels);

    // Get starting Y position based on state
    if (state == GameState::Menu) startY = 78.0f;
    else if (state == GameState::LevelSelect) startY = 100.0f;
    else if (state == GameState::DifficultySelect) startY = 160.0f;
    else if (state == GameState::CharacterSelect) startY = 72.0f;
    else if (state == GameState::Shop) startY = 68.0f;
    else if (state == GameState::Paused) startY = 160.0f;
    else if (state == GameState::Controls) startY = 80.0f;

    // Check items within viewport
    const int maxVisible = getMaxVisibleItems(state);
    const int endItem = std::min(viewportOffset + maxVisible, totalItems);

    for (int i = viewportOffset; i < endItem; ++i) {
        const float itemY = startY + kRowSpacing * (i - viewportOffset);
        if (mouseY >= itemY && mouseY <= itemY + kDisplayHeight) {
            return i;
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

int MenuScreen::getMaxVisibleItems(GameState state) {
    // Return different limits based on menu state
    if (state == GameState::Menu) return 6; // All items fit
    if (state == GameState::LevelSelect) return 4; // All items fit
    if (state == GameState::DifficultySelect) return 3; // All items fit
    if (state == GameState::Paused) return 3; // All items fit
    if (state == GameState::Controls) return 4; // Scrollable (16 actions total) - chỉ hiển thị 4 một lần
    if (state == GameState::CharacterSelect) return 4; // Scrollable
    if (state == GameState::Shop) return 4; // Scrollable
    return kMaxVisibleItems;
}

int MenuScreen::getTotalItems(GameState state, bool legendUnlocked, const std::vector<int>& profileLevels) {
    (void)legendUnlocked;
    (void)profileLevels;

    if (state == GameState::Menu) return 6;
    if (state == GameState::LevelSelect) return 4;
    if (state == GameState::DifficultySelect) return 3;
    if (state == GameState::Paused) return 3;
    if (state == GameState::Controls) return static_cast<int>(InputManager::ActionCount);
    if (state == GameState::CharacterSelect) return static_cast<int>(Player::profiles().size());
    if (state == GameState::Shop) return static_cast<int>(Player::profiles().size()) + 1; // +1 for "Back"
    return 0;
}
