#include "UI/HUD.h"
#include "World/Level.h"
#include <algorithm>
namespace { sf::Font& menuFont(){static sf::Font font;static bool attempted=false;if(!attempted){attempted=true;font.openFromFile("assets/fonts/arial.ttf")||font.openFromFile("C:/Windows/Fonts/arial.ttf");}return font;} }
void HUD::drawText(sf::RenderWindow& window,const std::string& text,sf::Vector2f position,unsigned size,sf::Color color){sf::Text label(menuFont(),text,size);label.setPosition(position);label.setFillColor(color);window.draw(label);} 
void HUD::draw(sf::RenderWindow& window,const Level& level)const{const auto& players=level.getPlayers();for(std::size_t i=0;i<players.size();++i){sf::RectangleShape bar({150,14});bar.setPosition({20.f,20.f+25.f*static_cast<float>(i)});bar.setFillColor({52,56,61});window.draw(bar);sf::RectangleShape hp({150.f*std::max(0.f,static_cast<float>(players[i]->getHealth()))/players[i]->getMaxHealth(),14});hp.setPosition(bar.getPosition());hp.setFillColor({202,74,79});window.draw(hp);}}
void HUD::drawPanel(sf::RenderWindow&,const Player&,sf::Vector2f,const std::string&){} void HUD::drawBar(sf::RenderWindow&,sf::Vector2f,float,float,float,sf::Color){}
