#include "UI/SettingsScreen.h"

#include "UI/HUD.h"
#include "Utils/Constants.h"

#include <algorithm>

namespace {
constexpr float kScreenCenterX = static_cast<float>(Constants::WINDOW_WIDTH) * 0.5f;
constexpr float kButtonWidth = 500.0f;
constexpr float kButtonHeight = 44.0f;
constexpr float kColumnX[2] = {90.0f, 690.0f};
constexpr float kFirstRowY = 135.0f;
constexpr float kRowSpacing = 54.0f;
constexpr std::size_t kRowsPerColumn = 8;
}

sf::FloatRect SettingsScreen::playerTabBounds(std::size_t playerIndex) {
    return {{430.0f + static_cast<float>(playerIndex) * 220.0f, 72.0f}, {200.0f, 44.0f}};
}

sf::FloatRect SettingsScreen::actionBounds(std::size_t actionIndex) {
    const std::size_t column = actionIndex / kRowsPerColumn;
    const std::size_t row = actionIndex % kRowsPerColumn;
    return {{kColumnX[column], kFirstRowY + kRowSpacing * static_cast<float>(row)},
            {kButtonWidth, kButtonHeight}};
}

sf::FloatRect SettingsScreen::backButtonBounds() {
    return {{540.0f, 620.0f}, {200.0f, 48.0f}};
}

void SettingsScreen::updateHover(sf::Vector2f mousePosition) {
    hoveredPlayerTab = -1;
    for (std::size_t i = 0; i < 2; ++i) {
        if (playerTabBounds(i).contains(mousePosition)) hoveredPlayerTab = static_cast<int>(i);
    }

    hoveredAction = -1;
    const auto& actionList = InputManager::actions();
    for (std::size_t i = 0; i < actionList.size(); ++i) {
        if (actionBounds(i).contains(mousePosition)) hoveredAction = static_cast<int>(i);
    }
    backHovered = backButtonBounds().contains(mousePosition);
}

bool SettingsScreen::handleMouseClick(sf::Vector2f mousePosition) {
    for (std::size_t i = 0; i < 2; ++i) {
        if (!playerTabBounds(i).contains(mousePosition)) continue;
        selectedPlayer = i;
        waitingAction.reset();
        statusMessage.clear();
        return false;
    }

    const auto& actionList = InputManager::actions();
    for (std::size_t i = 0; i < actionList.size(); ++i) {
        if (!actionBounds(i).contains(mousePosition)) continue;
        waitingAction = actionList[i];
        statusMessage = "Press any key... (Escape cancels)";
        return false;
    }

    if (backButtonBounds().contains(mousePosition)) {
        waitingAction.reset();
        statusMessage.clear();
        return true;
    }
    return false;
}

void SettingsScreen::handleKeyPressed(sf::Keyboard::Key key, InputManager& input) {
    if (!waitingAction.has_value()) return;
    if (key == sf::Keyboard::Key::Escape) {
        waitingAction.reset();
        statusMessage = "Rebinding cancelled.";
        return;
    }
    if (input.rebindKey(*waitingAction, key, selectedPlayer)) {
        statusMessage = InputManager::actionToString(*waitingAction) + " is now bound to " +
                        InputManager::keyToString(key) + ".";
        waitingAction.reset();
    } else {
        statusMessage = InputManager::keyToString(key) + " is already used. Choose another key.";
    }
}

void SettingsScreen::drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds,
                                const std::string& label, bool hovered, bool selected) {
    const float scale = hovered ? 1.025f : 1.0f;
    sf::RectangleShape button(bounds.size * scale);
    button.setOrigin(button.getSize() * 0.5f);
    button.setPosition(bounds.position + bounds.size * 0.5f);
    button.setFillColor(selected ? sf::Color(103, 80, 164, 245)
                                 : hovered ? sf::Color(246, 233, 190, 245)
                                           : sf::Color(34, 43, 48, 235));
    button.setOutlineThickness(selected || hovered ? 2.0f : 1.0f);
    button.setOutlineColor(selected ? sf::Color(210, 185, 255) : sf::Color(95, 108, 115));
    window.draw(button);
    HUD::drawTextCentered(window, label, bounds.position + bounds.size * 0.5f, 18,
                          hovered && !selected ? sf::Color(20, 25, 27) : sf::Color::White);
}

void SettingsScreen::draw(sf::RenderWindow& window, const InputManager& input) const {
    sf::RectangleShape shade(window.getView().getSize());
    shade.setFillColor({10, 13, 17, 238});
    window.draw(shade);

    HUD::drawTextCentered(window, "KEY BINDINGS", {kScreenCenterX, 32.0f}, 42, {246, 233, 190});
    for (std::size_t i = 0; i < 2; ++i) {
        drawButton(window, playerTabBounds(i), "Player " + std::to_string(i + 1),
                   hoveredPlayerTab == static_cast<int>(i), selectedPlayer == i);
    }

    const auto& actionList = InputManager::actions();
    for (std::size_t i = 0; i < actionList.size(); ++i) {
        const Action action = actionList[i];
        const bool waiting = waitingAction.has_value() && *waitingAction == action;
        const std::string label = waiting
            ? InputManager::actionToString(action) + ": [ Press any key... ]"
            : InputManager::actionToString(action) + ": [ " +
                  InputManager::keyToString(input.getBinding(action, selectedPlayer)) + " ]";
        drawButton(window, actionBounds(i), label, hoveredAction == static_cast<int>(i), waiting);
    }

    if (!statusMessage.empty()) {
        HUD::drawTextCentered(window, statusMessage, {kScreenCenterX, 585.0f}, 18,
                              waitingAction.has_value() ? sf::Color(246, 233, 190) : sf::Color(170, 220, 185));
    }
    drawButton(window, backButtonBounds(), "Back", backHovered);
}

bool SettingsScreen::isWaitingForInput() const {
    return waitingAction.has_value();
}

std::size_t SettingsScreen::getSelectedPlayer() const {
    return selectedPlayer;
}
