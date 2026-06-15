#include <World/LevelLoad.h>
#include <Utils/Constants.h>
#include <fstream>
void LevelLoad::LoadFromFile(const std::string& path, LevelData& out){
    std::ifstream file(path);
    if(!file)return ;
    std::vector< std::string> lines;
    std::string line;
    while(std::getline(file, line)){
        if(!line.empty() && line.back() == '\r')line.pop_back();
        if(!line.empty())lines.push_back(line);
    }
    int rows = static_cast<int>(lines.size());
    int cols = static_cast<int>(lines[0].size());
    out.solidtiles.assign(rows, std::vector<bool>(cols, false));

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            const sf::Vector2f pos({j * Constants::TILE_SIZE, i * Constants::TILE_SIZE});
            const char ch = lines[i][j];
            if(ch == '#')out.solidtiles[i][j] = 1;
            // if(lines[i][j] == 'S')out.playerspawn.push_back(sf::Vector2i{j, i});
            if(ch == '^'){
                out.spikes.emplace_back(
                    sf::FloatRect({pos.x, pos.y - 10}, {Constants::TILE_SIZE, Constants::TILE_SIZE - 10}));
            }
        }
    }
}