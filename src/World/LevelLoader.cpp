#include <World/LevelLoader.h>
#include <Utils/Constants.h>
#include <fstream>

bool LevelLoader::load(const std::string& path, LevelData& out){
    std::ifstream file(path);
    if(!file)return 0;
    std::vector< std::string> lines;
    std::string line;
    while(std::getline(file, line)){
        if(!line.empty() && line.back() == '\r')line.pop_back();
        if(!line.empty())lines.push_back(line);
    }
    static sf::Texture spikeTexture;
    static sf::Texture goalTexture;
    static const bool texturesLoaded = spikeTexture.loadFromFile("assets/textures/spike.png") &&
                                       goalTexture.loadFromFile("assets/textures/goal.png");
    if (!texturesLoaded || lines.empty()) return false;
    int rows = static_cast<int>(lines.size());
    int cols = static_cast<int>(lines[0].size());
    out.solidTiles.assign(rows, std::vector<bool>(cols, false));
    out.playerSpawns.assign(2, {0.0f, 0.0f});
    out.spikes.clear();
    out.checkpoints.clear();
    out.goals.clear();
    out.enemySpawns.clear();
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            const sf::Vector2f pos({x * Constants::TILE_SIZE, y * Constants::TILE_SIZE});
            const char ch = lines[y][x];
            if(ch == '#')out.solidTiles[y][x] = true;
            if(ch == '1') out.playerSpawns[0] = pos;
            if(ch == '2') out.playerSpawns[1] = pos;
            if(ch == 'P' || ch == 'R' || ch == 'B') out.enemySpawns.emplace_back(ch, pos);
            if(ch == 'C') out.checkpoints.emplace_back(sf::FloatRect(pos, {Constants::TILE_SIZE, Constants::TILE_SIZE}));
            if(ch == '^'){
                out.spikes.emplace_back(
                    sf::FloatRect({pos.x, pos.y}, {Constants::TILE_SIZE, Constants::TILE_SIZE}),
                    spikeTexture
                );
            }
            if(ch == 'G'){
                out.goals.emplace_back(
                    sf::FloatRect({pos.x, pos.y}, {Constants::TILE_SIZE, Constants::TILE_SIZE}),
                    goalTexture
                );
            }

        }
    }
    return 1;
}
