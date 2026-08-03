#include <Core/Game.h>

Game::Game():  
    window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), "Ori"){
    audio.SetVolume(70.f);
    audio.PlayMusic("assets/audio/background.ogg");
}

void Game::run(){
    sf::Clock clock;
    while(window.isOpen()){
        while(auto event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
        }
        audio.Update();
        const float dt = std::min(clock.restart().asSeconds(), 1.0f / 30.0f);
        render();
    }
}

void Game::render(){
    window.clear();
    level.draw();
    window.display();
}