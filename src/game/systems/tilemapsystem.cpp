#include "tilemapsystem.h"

#include <easylogging++/easylogging++.h>
#include <fstream>

using namespace frames;
using TextureCache = entt::resource_cache<sf::Texture>;

void TilemapSystem::init(Engine* engine, MapProperties map)
{
    ZoneScoped;
    ISystem::init(engine);

    loadTileset(map.tilesetFile, map.tileSize);
    auto [mapData, size] = decodeMap(map);
    loadMap(mapData, size, map.tileSize);

    LOG(INFO) << "TilemapSystem::init finished";
}

void TilemapSystem::cleanup()
{
    ZoneScoped;

    m_textureCache.clear();

    LOG(INFO) << "TilemapSystem::cleanup finished";
}

void TilemapSystem::processUpdate(entt::registry& reg, const double dt)
{
    ZoneScoped;
}

void TilemapSystem::processDraw(entt::registry& reg, sf::RenderTarget& target)
{
    ZoneScoped;

    target.draw(*this);
}

void TilemapSystem::loadTileset(const std::string& filename, sf::Vector2u tileSize)
{
    ZoneScoped;

    // Loads or reloads the tileset as needed
    m_textureCache.reload<TextureLoader>("textures/tileset"_hs, filename);
    m_tileSize = tileSize;

    // Finds the number of tile columns in the tileset
    const auto& tileset = m_textureCache.handle("textures/tileset"_hs).get();
    m_tilesetCols       = tileset.getSize().x / tileSize.x;

    LOG(DEBUG) << "TilemapSystem::loadTileset finished";
}

std::tuple<std::vector<int16_t>, sf::Vector2u>
TilemapSystem::decodeMap(const TilemapSystem::MapProperties& map)
{
    ZoneScoped;

    std::ifstream infile(map.file);

    std::vector<int16_t> mapData;
    sf::Vector2u size;

    if (!infile.is_open()) {
        LOG(ERROR) << "TilemapSystem::decodeMap error: Cannot open map file "
                   << map.file;
        return std::tie(mapData, size);
    }

    if (map.type == MapProperties::CSV) {
        // Process row by row
        std::string line;
        unsigned int rows = 0;
        LOG(DEBUG) << "Decoding CSV map file: " << map.file;
        while (std::getline(infile, line)) {
            rows++;
            // Process column by column
            std::istringstream s(line);
            std::string field;
            unsigned int cols = 0;
            while (getline(s, field, ',')) {
                cols++;
                mapData.push_back(std::stoi(field));
            }
            // TODO: Number of columns is computed mutltiple times, do this only
            // once
            size.x = cols;
        }
        size.y = rows;
    }

    LOG(DEBUG) << "TilemapSystem::decodeMap finished";

    return std::tie(mapData, size);
}

void TilemapSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    ZoneScoped;

    states.transform *= getTransform();

    states.texture = &m_textureCache.handle("textures/tileset"_hs).get();

    target.draw(m_vertices, states);
}

void TilemapSystem::loadMap(
    const std::vector<int16_t>& map,
    const sf::Vector2u size, const sf::Vector2u tilesize)
{
    ZoneScoped;

    const unsigned int& width  = size.x;
    const unsigned int& height = size.y;

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    // Reads the tile map from left to right then from top to bottom
    for (unsigned int iCol = 0; iCol < width; iCol++)
        for (unsigned int iRow = 0; iRow < height; iRow++) {
            // Get the tile ID
            unsigned int tileId = map[iCol + iRow * width];

            // Find the tile ID in the tileset
            unsigned int tilesetCol = 0;
            unsigned int tilesetRow = 0;
            if (tileId != -1) {
                tilesetCol = tileId % m_tilesetCols;
                tilesetRow = tileId / m_tilesetCols;
            }

            auto&& quad = &m_vertices[(iCol + iRow * width) * 4];

            // Set the 4 corner's positions
            quad[0].position = sf::Vector2f(iCol * tilesize.x,
                                            iRow * tilesize.y);
            quad[1].position = sf::Vector2f((iCol + 1) * tilesize.x,
                                            iRow * tilesize.y);
            quad[2].position = sf::Vector2f((iCol + 1) * tilesize.x,
                                            (iRow + 1) * tilesize.y);
            quad[3].position = sf::Vector2f(iCol * tilesize.x,
                                            (iRow + 1) * tilesize.y);

            // Set the 4 corner's texture coordinates
            quad[0].texCoords = sf::Vector2f(tilesetCol * tilesize.x,
                                             tilesetRow * tilesize.y);
            quad[1].texCoords = sf::Vector2f((tilesetCol + 1) * tilesize.x,
                                             tilesetRow * tilesize.y);
            quad[2].texCoords = sf::Vector2f((tilesetCol + 1) * tilesize.x,
                                             (tilesetRow + 1) * tilesize.y);
            quad[3].texCoords = sf::Vector2f(tilesetCol * tilesize.x,
                                             (tilesetRow + 1) * tilesize.y);
        }

    LOG(DEBUG) << "TilemapSystem::loadMap finished";
}
