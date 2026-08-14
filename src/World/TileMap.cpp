#include <World/TileMap.h>

#include <cmath>


void TileMap::setTiles(std::vector<std::vector<bool>> tiles){
    solid = std::move(tiles);
    rows = solid.size();
    cols = rows > 0 ? solid[0].size() : 0;
}
bool TileMap::IsSolid(const int& x, const int& y) const {
    if(x < 0 || y < 0 || y >= rows || x >= cols) return false;
    return solid[y][x];
}

bool TileMap::isSolidAt(sf::Vector2f position) const {
    const int x = static_cast<int>(std::floor(position.x / Constants::TILE_SIZE));
    const int y = static_cast<int>(std::floor(position.y / Constants::TILE_SIZE));
    return IsSolid(x, y);
}

bool TileMap::setSolid(int x, int y, bool value) {
    if(x < 0 || y < 0 || y >= rows || x >= cols) return false;
    solid[y][x] = value;
    return true;
}

void TileMap::Draw(sf::RenderWindow& window, sf::Vector2f camera, const sf::Texture* tileTexture) const {
    const sf::Vector2f viewSize = window.getView().getSize();
    const int startX = std::max(0, static_cast<int>(camera.x / Constants::TILE_SIZE) - 1);
    const int endX = std::min(cols - 1, static_cast<int>((camera.x + viewSize.x) / Constants::TILE_SIZE) + 2);
    const int startY = std::max(0, static_cast<int>(camera.y / Constants::TILE_SIZE) - 1);
    const int endY = std::min(rows - 1, static_cast<int>((camera.y + viewSize.y) / Constants::TILE_SIZE) + 2);

    if (tileTexture) {
        sf::Sprite tile(*tileTexture);
        const sf::Vector2u texSize = tileTexture->getSize();
        const sf::Vector2f scale(
            static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(texSize.x),
            static_cast<float>(Constants::TILE_SIZE) / static_cast<float>(texSize.y)
        );
        tile.setScale(scale);

        for (int y = startY; y <= endY; ++y) {
            for (int x = startX; x <= endX; ++x) {
                if (!solid[y][x]) continue;
                tile.setPosition({
                    static_cast<float>(x * Constants::TILE_SIZE) - camera.x,
                    static_cast<float>(y * Constants::TILE_SIZE) - camera.y
                });
                window.draw(tile);
            }
        }
        return;
    }

    sf::RectangleShape tile({static_cast<float>(Constants::TILE_SIZE), static_cast<float>(Constants::TILE_SIZE)});
    tile.setFillColor({57, 73, 50});
    sf::RectangleShape grass({static_cast<float>(Constants::TILE_SIZE), 6.0f});
    grass.setFillColor({86, 114, 74});

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            if (!solid[y][x]) continue;
            const sf::Vector2f p(
                static_cast<float>(x * Constants::TILE_SIZE) - camera.x,
                static_cast<float>(y * Constants::TILE_SIZE) - camera.y
            );
            tile.setPosition(p);
            window.draw(tile);
            grass.setPosition(p);
            window.draw(grass);
        }
    }
}
