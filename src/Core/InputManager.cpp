#include "Core/InputManager.h"

void InputManager::update() {
    // BƯỚC 1: ĐỌC TRẠNG THÁI PHÍM VẬT LÝ HIỆN TẠI
    
    // Player 1 (Left keyboard)
    bool currP1A = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    bool currP1D = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
    bool currP1W = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    bool currP1F = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);
    bool currP1G = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G);
    bool currP1H = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H);
    bool currP1R = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);

    // Player 2 (Right keyboard + Arrows)
    bool currP2Left  = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
    bool currP2Right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
    bool currP2Up    = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
    bool currP2J     = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J);
    bool currP2K     = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K);
    bool currP2L     = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L);
    bool currP2O     = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O);

    // BƯỚC 2: TÍNH TOÁN LOGIC BẤM 1 LẦN (JUST PRESSED) CHO PLAYER 1
    p1State.moveLeft   = processKey(currP1A, prevState.p1A);
    p1State.moveRight  = processKey(currP1D, prevState.p1D);
    p1State.jump       = processKey(currP1W, prevState.p1W);
    p1State.attack     = processKey(currP1F, prevState.p1F);
    p1State.dash       = processKey(currP1G, prevState.p1G);
    p1State.placeBlock = processKey(currP1H, prevState.p1H);
    p1State.throwItem  = processKey(currP1R, prevState.p1R);

    // BƯỚC 3: TÍNH TOÁN LOGIC BẤM 1 LẦN (JUST PRESSED) CHO PLAYER 2
    p2State.moveLeft   = processKey(currP2Left, prevState.p2Left);
    p2State.moveRight  = processKey(currP2Right, prevState.p2Right);
    p2State.jump       = processKey(currP2Up, prevState.p2Up);
    p2State.attack     = processKey(currP2J, prevState.p2J);
    p2State.dash       = processKey(currP2K, prevState.p2K);
    p2State.placeBlock = processKey(currP2L, prevState.p2L);
    p2State.throwItem  = processKey(currP2O, prevState.p2O);

    // BƯỚC 4: CẬP NHẬT TRẠNG THÁI HIỆN TẠI THÀNH FRAME CŨ DÙNG CHO FRAME SAU
    prevState.p1A = currP1A; prevState.p1D = currP1D; prevState.p1W = currP1W;
    prevState.p1F = currP1F; prevState.p1G = currP1G; prevState.p1H = currP1H; prevState.p1R = currP1R;

    prevState.p2Left = currP2Left; prevState.p2Right = currP2Right; prevState.p2Up = currP2Up;
    prevState.p2J = currP2J; prevState.p2K = currP2K; prevState.p2L = currP2L; prevState.p2O = currP2O;
}

const InputState& InputManager::getPlayer1Input() const {
    return p1State;
}

const InputState& InputManager::getPlayer2Input() const {
    return p2State;
}