#include "UI/MenuScreen.h"
#include "UI/HUD.h"
#include "Utils/Constants.h"

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
constexpr float kRowSpacing = 88.0f;

void row(sf::RenderWindow& window, const std::string& text, float y, bool active, sf::Color color = sf::Color::White) {
    if (selectorTexture) {
        sf::Sprite selector(*selectorTexture);
        const int frameIndex = active ? 1 : 0;
        selector.setTextureRect(sf::IntRect({static_cast<int>(frameIndex * kFrameWidth), 0},
                                            {static_cast<int>(kFrameWidth), static_cast<int>(kFrameHeight)}));
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
}

void MenuScreen::setSelectorTexture(const sf::Texture& texture) {
    selectorTexture = &texture;
}

void MenuScreen::draw(sf::RenderWindow& window, GameState state, int menuIndex, const int selectedProfiles[2], int activePlayer,
                      int playerCount, float volume, float gameSpeed, int selectedLevel, int unlockedLevelCount,
                      int selectedDifficulty, int walletCoins, int shopIndex, bool legendUnlocked,
                      const std::vector<int>& profileLevels) const {
    if (state == GameState::Playing) return;
    sf::RectangleShape shade(window.getView().getSize());
    shade.setFillColor({10, 13, 17, 225});
    window.draw(shade);

    if (state == GameState::Menu) {
        HUD::drawTextCentered(window, "CO-OP PLATFORMER", {kScreenCenterX, 40.0f}, 46, {246, 233, 190});
        const char* entries[] = {"1 Player", "2 Players", "Choose Difficulty", "Shop", "Info", "Quit"};
        for (int i = 0; i < 6; ++i) row(window, entries[i], 78.0f + kRowSpacing * i, i == menuIndex);
        HUD::drawTextCentered(window, "Up/Down: choose   Enter: confirm", {kScreenCenterX, 660.0f}, 20, {204, 213, 210});
        return;
    }
    if (state == GameState::LevelSelect) {
        HUD::drawTextCentered(window, "CHOOSE MAP", {kScreenCenterX, 40.0f}, 46, {246, 233, 190});
        const char* maps[] = {"Forest Trail", "Stone Bridge", "Spike Valley", "Boss Lair"};
        for (int i = 0; i < 4; ++i) {
            row(window, std::to_string(i + 1) + ". " + maps[i] + (i >= unlockedLevelCount ? "  [LOCKED]" : ""),
                100.0f + kRowSpacing * i, i == selectedLevel,
                i < unlockedLevelCount ? sf::Color::White : sf::Color(140, 140, 140));
        }
        HUD::drawTextCentered(window, "Up/Down: choose   Enter: confirm   Escape: back", {kScreenCenterX, 560.0f}, 20, {204, 213, 210});
        return;
    }
    if (state == GameState::DifficultySelect) {
        HUD::drawTextCentered(window, "CHOOSE DIFFICULTY", {kScreenCenterX, 40.0f}, 46, {246, 233, 190});
        const char* levels[] = {"Easy", "Medium (+1 enemy HP)", "Hard (+2 enemy HP)"};
        for (int i = 0; i < 3; ++i) row(window, levels[i], 160.0f + kRowSpacing * i, i == selectedDifficulty);
        return;
    }
    if (state == GameState::CharacterSelect) {
        HUD::drawTextCentered(window, "CHOOSE CHARACTER", {kScreenCenterX, 36.0f}, 46, {246, 233, 190});
        const auto& profiles = Player::profiles();
        for (int i = 0; i < static_cast<int>(profiles.size()); ++i) {
            const bool locked = i == static_cast<int>(profiles.size()) - 1 && !legendUnlocked;
            std::string text = profiles[i].name + " - " + profiles[i].skill;
            if (locked) text += " [LOCKED]";
            const bool selected = i == selectedProfiles[0] || (playerCount == 2 && i == selectedProfiles[1]);
            row(window, text, 72.0f + kRowSpacing * i, selected, locked ? sf::Color(145, 125, 125) : profiles[i].color);
        }
        const std::string selection = playerCount == 1 ? "P1" : "P1 / P2";
        HUD::drawTextCentered(window, selection + ": A/D, Left/Right choose   Enter: start", {kScreenCenterX, 660.0f}, 19, {204, 213, 210});
        HUD::drawTextCentered(window, "Active selector: Player " + std::to_string(activePlayer + 1), {kScreenCenterX, 690.0f}, 19, {246, 233, 190});
        return;
    }
    if (state == GameState::Shop) {
        HUD::drawTextCentered(window, "SHOP - Coins: " + std::to_string(walletCoins), {kScreenCenterX, 36.0f}, 42, {246, 233, 190});
        const auto& profiles = Player::profiles();
        for (int i = 0; i < static_cast<int>(profiles.size()); ++i) {
            row(window, profiles[i].name + "  Lv " + std::to_string(profileLevels[i]) +
                    (i == static_cast<int>(profiles.size()) - 1 && !legendUnlocked ? "  Buy Legend: 50" : "  Upgrade: 20"),
                68.0f + kRowSpacing * i, i == shopIndex);
        }
        row(window, "Back", 68.0f + kRowSpacing * static_cast<float>(profiles.size()), shopIndex == static_cast<int>(profiles.size()));
        return;
    }
    if (state == GameState::Paused) {
        HUD::drawTextCentered(window, "PAUSED", {kScreenCenterX, 52.0f}, 48, {246, 233, 190});
        HUD::drawTextCentered(window, "P: resume   Escape: menu", {kScreenCenterX, 180.0f}, 24, sf::Color::White);
        HUD::drawTextCentered(window, "Speed: x" + std::to_string(gameSpeed) + "   Volume: " + std::to_string(static_cast<int>(volume)) + "%",
                              {kScreenCenterX, 225.0f}, 22, {204, 213, 210});
        return;
    }
    const char* message = state == GameState::Victory ? "VICTORY" : state == GameState::GameOver ? "GAME OVER" : "INFO";
    HUD::drawTextCentered(window, message, {kScreenCenterX, 52.0f}, 48, {246, 233, 190});
    HUD::drawTextCentered(window, "Enter: restart   Escape: menu", {kScreenCenterX, 180.0f}, 24, sf::Color::White);
}
