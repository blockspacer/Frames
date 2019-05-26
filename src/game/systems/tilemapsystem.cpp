#include "tilemapsystem.h"

#include <easylogging++/easylogging++.h>
#include <fstream>

#include <fengine/resources/textureloader.h>

using namespace frames;
using TextureCache = entt::resource_cache<sf::Texture>;

void TilemapSystem::init(Engine* engine)
{
    ZoneScoped;
    ISystem::init(engine);

    m_textureCache = m_engine->getTextureCache();

    loadMaps(engine->getRegistry());

    LOG(INFO) << "TilemapSystem::init finished";
}

void TilemapSystem::loadMaps(entt::registry& reg)
{
    auto view = reg.view<Tilemap, TilemapProperties>();

    for (auto e : view) {
        auto&& map        = reg.get<Tilemap>(e);
        auto&& properties = reg.get<TilemapProperties>(e);
        if (!map.loaded) {
            // Step 1. Load the tileset
            loadTileset(map, properties);

            // Step 2. Load the map data
            auto mapData = openMap(map, properties);

            // Step 3. Translate the map data
            decodeMap(map, properties, mapData);

            // Finish
            map.loaded = true;
        }
    }
}

void TilemapSystem::cleanup()
{
    ZoneScoped;

    LOG(INFO) << "TilemapSystem::cleanup finished";
}

void TilemapSystem::processUpdate(entt::registry& reg, const double dt)
{
    ZoneScoped;
}

void TilemapSystem::processDraw(entt::registry& reg, sf::RenderTarget& target)
{
    ZoneScoped;

    sf::RenderStates states;
    //states.transform *= getTransform();

    auto view = reg.view<Tilemap>();
    for (auto e : view) {
        auto&& map = reg.get<Tilemap>(e);

        states.texture = &*map.tilesetHandle;
        // Equivalent to:
        //states.texture = &map.tilesetHandle.get();

        target.draw(map.vertices, states);
    }
}

void TilemapSystem::loadTileset(Tilemap& map, TilemapProperties& properties)
{
    ZoneScoped;

    // Loads the tileset as needed
    map.tilesetHandle = m_textureCache->load<TextureLoader>(properties.tilesetId,
                                                            properties.tilesetFile);

    // Find the tileset size (number of rows and columns)
    properties.tilesetSize.x = map.tilesetHandle->getSize().x
                               / properties.tileSize.x;
    properties.tilesetSize.y = map.tilesetHandle->getSize().y
                               / properties.tileSize.y;
}

std::vector<int16_t> TilemapSystem::openMap(Tilemap& map, TilemapProperties& properties)
{
    ZoneScoped;

    std::ifstream infile(properties.file);

    std::vector<int16_t> mapData;
    sf::Vector2u size;

    if (!infile.is_open()) {
        LOG(ERROR) << "TilemapSystem::decodeMap error: Cannot open map file "
                   << properties.file;
        return mapData;
    }

    if (properties.type == TilemapProperties::CSV) {
        // Process row by row
        std::string line;
        unsigned int rows = 0;

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
            properties.size.x = cols;
        }
        properties.size.y = rows;
    }

    return mapData;
}

void TilemapSystem::decodeMap(Tilemap& map, TilemapProperties& properties,
                              std::vector<int16_t> data)
{
    const unsigned int& width  = properties.size.x;
    const unsigned int& height = properties.size.y;

    const unsigned int& tileWidth  = properties.tileSize.x;
    const unsigned int& tileHeight = properties.tileSize.y;

    const unsigned int& tilesetCols = properties.tilesetSize.x;

    map.vertices.setPrimitiveType(sf::Quads);
    map.vertices.resize(width * height * 4);

    // Reads the tile map from left to right then from top to bottom
    for (unsigned int iCol = 0; iCol < width; iCol++)
        for (unsigned int iRow = 0; iRow < height; iRow++) {
            // Get the tile ID
            unsigned int tileId = data[iCol + iRow * width];

            // Find the tile ID in the tileset
            unsigned int tilesetCol = 0;
            unsigned int tilesetRow = 0;
            if (tileId != -1) {
                tilesetCol = tileId % tilesetCols;
                tilesetRow = tileId / tilesetCols;
            }

            // Modifiable reference
            auto&& quad = &map.vertices[(iCol + iRow * width) * 4];

            // Set the 4 corner's positions
            quad[0].position = sf::Vector2f(iCol, iRow);
            quad[1].position = sf::Vector2f((iCol + 1), iRow);
            quad[2].position = sf::Vector2f((iCol + 1), (iRow + 1));
            quad[3].position = sf::Vector2f(iCol, (iRow + 1));

            // Set the 4 corner's texture coordinates
            quad[0].texCoords = sf::Vector2f(tilesetCol * tileWidth,
                                             tilesetRow * tileHeight);
            quad[1].texCoords = sf::Vector2f((tilesetCol + 1) * tileWidth,
                                             tilesetRow * tileHeight);
            quad[2].texCoords = sf::Vector2f((tilesetCol + 1) * tileWidth,
                                             (tilesetRow + 1) * tileHeight);
            quad[3].texCoords = sf::Vector2f(tilesetCol * tileWidth,
                                             (tilesetRow + 1) * tileHeight);
        }
}
