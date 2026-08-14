#include<Core/AssetManager.h>

#include <stdexcept>

bool AssetManager::LoadTexture(const std::string& id, const std::string& path){
    sf::Texture texture;
    if(!texture.loadFromFile(path))return false;
    textures[id] = std::move(texture);
    return true;
}

bool AssetManager::LoadFont(const std::string& id, const std::string& path){
    sf::Font font;
    if(!font.openFromFile(path))return false;
    fonts[id] = std::move(font);
    return true;    
}

const sf::Texture& AssetManager::texture(const std::string& id) const{
    const auto it = textures.find(id);
    if (it == textures.end()) throw std::out_of_range("Texture asset not loaded: " + id);
    return it->second;
}

const sf::Font& AssetManager::font(const std::string& id) const{
    const auto it = fonts.find(id);
    if (it == fonts.end()) throw std::out_of_range("Font asset not loaded: " + id);
    return it->second;
}

void AssetManager::Clear(){
    textures.clear();
    fonts.clear();
}
