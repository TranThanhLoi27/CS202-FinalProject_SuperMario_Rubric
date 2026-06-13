#include<Core/AssetManager.h>
#include<map>
#include<string>

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

const sf::Texture* AssetManager::texture(const std::string& id) const{
    const auto it = textures.find(id);
    return it == textures.end() ? nullptr : &it->second;
}

const sf::Font* AssetManager::font(const std::string& id) const{
    const auto it = fonts.find(id);
    return it == fonts.end() ? nullptr : &it->second;
}

void AssetManager::clear(){
    textures.clear();
    fonts.clear();
}