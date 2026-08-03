#include <Core/Game.h>

Game::Game():  
    window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), "Ori"){
    audio.SetVolume(70.f);
    audio.PlayMusic("assets/audio/background.ogg");
}

void Game::run(){
    while(window.isOpen()){
        while(auto event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
        }
        window.clear();
        window.display();
    }
}