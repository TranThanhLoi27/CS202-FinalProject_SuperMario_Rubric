#include "UI/MenuScreen.h"
#include "UI/HUD.h"

namespace {
void row(sf::RenderWindow& window, const std::string& text, sf::Vector2f pos, bool active, sf::Color color = sf::Color::White) {
    sf::RectangleShape box({540.0f, 46.0f});
    box.setPosition(pos);
    box.setFillColor(active ? sf::Color(226, 180, 94) : sf::Color(34, 43, 48));
    box.setOutlineThickness(active ? 2.0f : 0.0f);
    box.setOutlineColor({255, 241, 190});
    window.draw(box);
    HUD::drawText(window, text, pos + sf::Vector2f(18.0f, 8.0f), 23, active ? sf::Color(20, 25, 27) : color);
}
}

void MenuScreen::draw(sf::RenderWindow& window, GameState state, int menuIndex, const int selectedProfiles[2], int activePlayer,
                      int playerCount, float volume, float gameSpeed, int selectedLevel, int unlockedLevelCount,
                      int selectedDifficulty, int walletCoins, int shopIndex, bool legendUnlocked,
                      const std::vector<int>& profileLevels) const {
    if (state == GameState::Playing) return;
    sf::RectangleShape shade(window.getView().getSize());
    shade.setFillColor({10, 13, 17, 225});
    window.draw(shade);
    const sf::Vector2f origin{90.0f, 60.0f};

    if (state == GameState::Menu) {
        HUD::drawText(window, "CO-OP PLATFORMER", origin, 46, {246, 233, 190});
        const char* entries[] = {"1 Player", "2 Players", "Choose Difficulty", "Shop", "Info", "Quit"};
        for (int i = 0; i < 6; ++i) row(window, entries[i], {origin.x, 150.0f + 55.0f * i}, i == menuIndex);
        HUD::drawText(window, "Up/Down: choose   Enter: confirm", {origin.x, 500.0f}, 20, {204, 213, 210});
        return;
    }
    if (state == GameState::LevelSelect) {
        HUD::drawText(window, "CHOOSE MAP", origin, 46, {246, 233, 190});
        const char* maps[] = {"Forest Trail", "Stone Bridge", "Spike Valley", "Boss Lair"};
        for (int i = 0; i < 4; ++i) row(window, std::to_string(i + 1) + ". " + maps[i] + (i >= unlockedLevelCount ? "  [LOCKED]" : ""), {origin.x, 150.0f + 55.0f * i}, i == selectedLevel, i < unlockedLevelCount ? sf::Color::White : sf::Color(140, 140, 140));
        HUD::drawText(window, "Up/Down: choose   Enter: confirm   Escape: back", {origin.x, 420.0f}, 20, {204, 213, 210});
        return;
    }
    if (state == GameState::DifficultySelect) {
        HUD::drawText(window, "CHOOSE DIFFICULTY", origin, 46, {246, 233, 190});
        const char* levels[] = {"Easy", "Medium (+1 enemy HP)", "Hard (+2 enemy HP)"};
        for (int i = 0; i < 3; ++i) row(window, levels[i], {origin.x, 150.0f + 55.0f * i}, i == selectedDifficulty);
        return;
    }
    if (state == GameState::CharacterSelect) {
        HUD::drawText(window, "CHOOSE CHARACTER", origin, 46, {246, 233, 190});
        const auto& profiles = Player::profiles();
        for (int i = 0; i < static_cast<int>(profiles.size()); ++i) {
            const bool locked = i == static_cast<int>(profiles.size()) - 1 && !legendUnlocked;
            std::string text = profiles[i].name + " — " + profiles[i].skill;
            if (locked) text += " [LOCKED]";
            const bool selected = i == selectedProfiles[0] || (playerCount == 2 && i == selectedProfiles[1]);
            row(window, text, {origin.x, 135.0f + 55.0f * i}, selected, locked ? sf::Color(145, 125, 125) : profiles[i].color);
        }
        const std::string selection = playerCount == 1 ? "P1" : "P1 / P2";
        HUD::drawText(window, selection + ": A/D, Left/Right choose   Enter: start", {origin.x, 490.0f}, 19, {204, 213, 210});
        HUD::drawText(window, "Active selector: Player " + std::to_string(activePlayer + 1), {origin.x, 520.0f}, 19, {246, 233, 190});
        return;
    }
    if (state == GameState::Shop) {
        HUD::drawText(window, "SHOP — Coins: " + std::to_string(walletCoins), origin, 42, {246, 233, 190});
        const auto& profiles = Player::profiles();
        for (int i = 0; i < static_cast<int>(profiles.size()); ++i) row(window, profiles[i].name + "  Lv " + std::to_string(profileLevels[i]) + (i == static_cast<int>(profiles.size()) - 1 && !legendUnlocked ? "  Buy Legend: 50" : "  Upgrade: 20"), {origin.x, 130.0f + 55.0f * i}, i == shopIndex);
        row(window, "Back", {origin.x, 130.0f + 55.0f * static_cast<float>(profiles.size())}, shopIndex == static_cast<int>(profiles.size()));
        return;
    }
    if (state == GameState::Paused) {
        HUD::drawText(window, "PAUSED", origin, 48, {246, 233, 190});
        HUD::drawText(window, "P: resume   Escape: menu", {origin.x, 150.0f}, 24, sf::Color::White);
        HUD::drawText(window, "Speed: x" + std::to_string(gameSpeed) + "   Volume: " + std::to_string(static_cast<int>(volume)) + "%", {origin.x, 195.0f}, 22, {204, 213, 210});
        return;
    }
    const char* message = state == GameState::Victory ? "VICTORY" : state == GameState::GameOver ? "GAME OVER" : "INFO";
    HUD::drawText(window, message, origin, 48, {246, 233, 190});
    HUD::drawText(window, "Enter: restart   Escape: menu", {origin.x, 150.0f}, 24, sf::Color::White);
}
