#include <World/LevelLoader.h>
#include <Utils/Constants.h>
#include <fstream>

bool LevelLoader::load(const std::string& path, LevelData& out, AssetManager& assets){
    std::ifstream file(path);
    if(!file)return 0;
    std::vector< std::string> lines;
    std::string line;
    while(std::getline(file, line)){
        if(!line.empty() && line.back() == '\r')line.pop_back();
        if(!line.empty())lines.push_back(line);
    }
    const sf::Texture& SpikeTexture = assets.texture("spike");
    const sf::Texture& GoalTexture = assets.texture("goal");
    int rows = static_cast<int>(lines.size());
    int cols = static_cast<int>(lines[0].size());
    out.solidtiles.assign(rows + 1, std::vector<bool>(cols + 1, false));
    out.playerspawns.assign(2, {0.0f, 0.0f});
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            const sf::Vector2f pos({x * Constants::TILE_SIZE, y * Constants::TILE_SIZE});
            const char ch = lines[y][x];
            if(ch == '#')out.solidtiles[y][x] = 1;
            if(lines[y][x] == '1')out.playerspawns[0] = sf::Vector2f({static_cast<float>(x), static_cast<float>(y)});
            if(lines[y][x] == '2')out.playerspawns[1] = sf::Vector2f({static_cast<float>(x), static_cast<float>(y)});
            if(ch == '^'){
                out.spikes.emplace_back(
                    sf::FloatRect({pos.x, pos.y}, {Constants::TILE_SIZE, Constants::TILE_SIZE}),
                    SpikeTexture   
                );
            }
            if(ch == 'G'){
                out.goals.emplace_back(
                    sf::FloatRect({pos.x, pos.y}, {Constants::TILE_SIZE, Constants::TILE_SIZE}),
                    GoalTexture 
                );
            }

        }
    }
    return 1;
}