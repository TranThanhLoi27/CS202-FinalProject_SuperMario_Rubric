#include "UI/MenuScreen.h"

#include "UI/HUD.h"
#include "Utils/Constants.h"

#include <array>

namespace {
const sf::Texture* selectorTexture = nullptr;

constexpr float kFrameWidth = 512.0f;
constexpr float kFrameHeight = 192.0f;
constexpr float kBaseRowHeight = 58.0f;
constexpr float kSelectorScaleX = (kBaseRowHeight / kFrameHeight) * 4.0f;
constexpr float kSelectorScaleY = (kBaseRowHeight / kFrameHeight) * 2.0f;
constexpr float kDisplayWidth = kFrameWidth * kSelectorScaleX;
constexpr float kDisplayHeight = kFrameHeight * kSelectorScaleY;
constexpr float kCenterX = (static_cast<float>(Constants::WINDOW_WIDTH) - kDisplayWidth) * 0.5f;
constexpr float kScreenCenterX = static_cast<float>(Constants::WINDOW_WIDTH) * 0.5f;

sf::FloatRect rowBounds(float y) {
    const float hitY = y + (kDisplayHeight - kBaseRowHeight) * 0.5f;
    return {{kCenterX, hitY}, {kDisplayWidth, kBaseRowHeight}};
}

void row(sf::RenderWindow& window, const std::string& text, float y, bool highlighted,
         sf::Color color = sf::Color::White) {
    if (selectorTexture) {
        sf::Sprite selector(*selectorTexture);
        selector.setTextureRect(sf::IntRect(
            {highlighted ? static_cast<int>(kFrameWidth) : 0, 0},
            {static_cast<int>(kFrameWidth), static_cast<int>(kFrameHeight)}
        ));
        const float hoverScale = highlighted ? 1.035f : 1.0f;
        selector.setScale({kSelectorScaleX * hoverScale, kSelectorScaleY * hoverScale});
        selector.setOrigin({kFrameWidth * 0.5f, kFrameHeight * 0.5f});
        selector.setPosition({kScreenCenterX, y + kDisplayHeight * 0.5f});
        window.draw(selector);
    } else {
        sf::RectangleShape box({kDisplayWidth, kBaseRowHeight});
        box.setPosition(rowBounds(y).position);
        box.setFillColor(highlighted ? sf::Color(246, 233, 190) : sf::Color(34, 43, 48));
        window.draw(box);
    }

    HUD::drawTextCentered(window, text, {kScreenCenterX, y + kDisplayHeight * 0.5f},
                          highlighted ? 25U : 23U,
                          highlighted ? sf::Color(20, 25, 27) : color);
}
}

void MenuScreen::setSelectorTexture(const sf::Texture& texture) {
    selectorTexture = &texture;
}

std::vector<sf::FloatRect> MenuScreen::buttonBounds(GameState state, int profileCount) {
    std::vector<sf::FloatRect> bounds;
    const auto addRows = [&](int count, float startY, float spacing) {
        for (int i = 0; i < count; ++i) bounds.push_back(rowBounds(startY + spacing * static_cast<float>(i)));
    };

    if (state == GameState::Menu) addRows(7, 68.0f, 78.0f);
    if (state == GameState::LevelSelect) {
        addRows(4, 96.0f, 88.0f);
        bounds.push_back(rowBounds(500.0f));
    }
    if (state == GameState::DifficultySelect) {
        addRows(3, 145.0f, 92.0f);
        bounds.push_back(rowBounds(470.0f));
    }
    if (state == GameState::CharacterSelect) {
        addRows(profileCount, 64.0f, 76.0f);
        bounds.push_back(rowBounds(535.0f));
        bounds.push_back(rowBounds(610.0f));
    }
    if (state == GameState::Shop) addRows(profileCount + 1, 60.0f, 78.0f);
    if (state == GameState::Info) bounds.push_back(rowBounds(245.0f));
    if (state == GameState::Paused) addRows(3, 155.0f, 90.0f);
    if (state == GameState::Victory || state == GameState::GameOver) addRows(2, 150.0f, 90.0f);
    return bounds;
}

void MenuScreen::updateHover(GameState state, sf::Vector2f mousePosition, int profileCount) {
    hoveredButton = buttonAt(state, mousePosition, profileCount);
}

int MenuScreen::buttonAt(GameState state, sf::Vector2f mousePosition, int profileCount) const {
    const auto bounds = buttonBounds(state, profileCount);
    for (std::size_t i = 0; i < bounds.size(); ++i) {
        if (bounds[i].contains(mousePosition)) return static_cast<int>(i);
    }
    return -1;
}

int MenuScreen::getHoveredButton() const {
    return hoveredButton;
}

void MenuScreen::draw(sf::RenderWindow& window, GameState state, int, const int selectedProfiles[2], int activePlayer,
                      int playerCount, float volume, float gameSpeed, int selectedLevel, int unlockedLevelCount,
                      int selectedDifficulty, int walletCoins, int, bool legendUnlocked,
                      const std::vector<int>& profileLevels) const {
    if (state == GameState::Playing || state == GameState::Controls) return;

    sf::RectangleShape shade(window.getView().getSize());
    shade.setFillColor({10, 13, 17, 225});
    window.draw(shade);

    if (state == GameState::Menu) {
        HUD::drawTextCentered(window, "CO-OP PLATFORMER", {kScreenCenterX, 30.0f}, 42, {246, 233, 190});
        constexpr std::array<const char*, 7> entries = {
            "1 Player", "2 Players", "Choose Difficulty", "Shop", "Info", "Settings / Keybinds", "Quit"
        };
        for (std::size_t i = 0; i < entries.size(); ++i) {
            row(window, entries[i], 68.0f + 78.0f * static_cast<float>(i), hoveredButton == static_cast<int>(i));
        }
        HUD::drawTextCentered(window, "Move the mouse and left-click an option", {kScreenCenterX, 682.0f}, 18, {204, 213, 210});
        return;
    }

    if (state == GameState::LevelSelect) {
        HUD::drawTextCentered(window, "CHOOSE MAP", {kScreenCenterX, 36.0f}, 44, {246, 233, 190});
        constexpr std::array<const char*, 4> maps = {"Forest Trail", "Stone Bridge", "Spike Valley", "Boss Lair"};
        for (std::size_t i = 0; i < maps.size(); ++i) {
            const bool locked = static_cast<int>(i) >= unlockedLevelCount;
            std::string label = std::to_string(i + 1) + ". " + maps[i];
            if (static_cast<int>(i) == selectedLevel) label += "  [SELECTED]";
            if (locked) label += "  [LOCKED]";
            row(window, label, 96.0f + 88.0f * static_cast<float>(i), hoveredButton == static_cast<int>(i),
                locked ? sf::Color(140, 140, 140) : sf::Color::White);
        }
        row(window, "Back", 500.0f, hoveredButton == 4);
        return;
    }

    if (state == GameState::DifficultySelect) {
        HUD::drawTextCentered(window, "CHOOSE DIFFICULTY", {kScreenCenterX, 42.0f}, 44, {246, 233, 190});
        constexpr std::array<const char*, 3> levels = {"Easy", "Medium (+1 enemy HP)", "Hard (+2 enemy HP)"};
        for (std::size_t i = 0; i < levels.size(); ++i) {
            std::string label = levels[i];
            if (static_cast<int>(i) == selectedDifficulty) label += "  [SELECTED]";
            row(window, label, 145.0f + 92.0f * static_cast<float>(i), hoveredButton == static_cast<int>(i));
        }
        row(window, "Back", 470.0f, hoveredButton == 3);
        return;
    }

    if (state == GameState::CharacterSelect) {
        HUD::drawTextCentered(window, "CHOOSE CHARACTER FOR PLAYER " + std::to_string(activePlayer + 1),
                              {kScreenCenterX, 28.0f}, 38, {246, 233, 190});
        const auto& profiles = Player::profiles();
        for (int i = 0; i < static_cast<int>(profiles.size()); ++i) {
            const bool locked = i == static_cast<int>(profiles.size()) - 1 && !legendUnlocked;
            const bool selected = i == selectedProfiles[0] || (playerCount == 2 && i == selectedProfiles[1]);
            std::string label = profiles[i].name + " - " + profiles[i].skill;
            if (selected) label += "  [SELECTED]";
            if (locked) label += "  [LOCKED]";
            row(window, label, 64.0f + 76.0f * static_cast<float>(i), hoveredButton == i,
                locked ? sf::Color(145, 125, 125) : profiles[i].color);
        }
        const int startIndex = static_cast<int>(profiles.size());
        row(window, "Start Game", 535.0f, hoveredButton == startIndex);
        row(window, "Back", 610.0f, hoveredButton == startIndex + 1);
        return;
    }

    if (state == GameState::Shop) {
        HUD::drawTextCentered(window, "SHOP - Coins: " + std::to_string(walletCoins), {kScreenCenterX, 28.0f}, 38, {246, 233, 190});
        const auto& profiles = Player::profiles();
        for (int i = 0; i < static_cast<int>(profiles.size()); ++i) {
            row(window, profiles[i].name + "  Lv " + std::to_string(profileLevels[static_cast<std::size_t>(i)]) +
                    (i == static_cast<int>(profiles.size()) - 1 && !legendUnlocked ? "  Buy Legend: 50" : "  Upgrade: 20"),
                60.0f + 78.0f * static_cast<float>(i), hoveredButton == i);
        }
        row(window, "Back", 60.0f + 78.0f * static_cast<float>(profiles.size()),
            hoveredButton == static_cast<int>(profiles.size()));
        return;
    }

    if (state == GameState::Info) {
        HUD::drawTextCentered(window, "INFO", {kScreenCenterX, 52.0f}, 46, {246, 233, 190});
        HUD::drawTextCentered(window, "Cooperate, defeat enemies, collect loot, and reach the goal.",
                              {kScreenCenterX, 150.0f}, 22, sf::Color::White);
        row(window, "Back", 245.0f, hoveredButton == 0);
        return;
    }

    if (state == GameState::Paused) {
        HUD::drawTextCentered(window, "PAUSED", {kScreenCenterX, 52.0f}, 48, {246, 233, 190});
        row(window, "Resume", 155.0f, hoveredButton == 0);
        row(window, "Settings / Keybinds", 245.0f, hoveredButton == 1);
        row(window, "Main Menu", 335.0f, hoveredButton == 2);
        HUD::drawTextCentered(window,
                              "Speed: x" + std::to_string(gameSpeed) + "   Volume: " +
                                  std::to_string(static_cast<int>(volume)) + "%",
                              {kScreenCenterX, 520.0f}, 20, {204, 213, 210});
        return;
    }

    const bool victory = state == GameState::Victory;
    HUD::drawTextCentered(window, victory ? "VICTORY" : "GAME OVER", {kScreenCenterX, 52.0f}, 48, {246, 233, 190});
    row(window, "Restart", 150.0f, hoveredButton == 0);
    row(window, "Main Menu", 240.0f, hoveredButton == 1);
}
