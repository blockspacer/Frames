#include "tilemapsystem.h"

#include <easylogging++/easylogging++.h>
#include <fstream>

#include <fengine/resources/textureloader.h>

using namespace frames;
using TextureCache = entt::resource_cache<sf::Texture>;

void TilemapSystem::init(Engine* engine) {
  ZoneScoped;
  ISystem::init(engine);

  m_textureCache = m_engine->getTextureCache();

  load(engine->getRegistry());

  LOG(INFO) << "TilemapSystem::init finished";
}

void TilemapSystem::load(entt::registry& reg) const {
  auto maps = reg.view<Tilemap, Tileset>();

  for (auto e : maps) {
    LOG(INFO) << "Loading map";
    auto&& map = reg.get<Tilemap>(e);
    auto&& set = reg.get<Tileset>(e);
    if (!map.loaded) {
      // 1. Load the tileset
      if (loadTileset(set)) {
        // 2. Load the map
        loadMap(map, set);

        // 3. Finish
        map.loaded = true;
      }
    }
  }
}

void TilemapSystem::cleanup() {
  ZoneScoped;

  LOG(INFO) << "TilemapSystem::cleanup finished";
}

void TilemapSystem::processUpdate(entt::registry& reg, const double dt) {
  ZoneScoped;
}

void TilemapSystem::processDraw(entt::registry& reg, sf::RenderTarget& target) {
  ZoneScoped;

  sf::RenderStates states;
  // states.transform *= getTransform();

  auto view = reg.view<Tilemap, Tileset>();
  // Draw each map
  for (auto e : view) {
    auto&& map = reg.get<Tilemap>(e);
    auto&& set = reg.get<Tileset>(e);

    states.texture = &set.handle.get();

    // Draw each layer of the map
    for (auto layerData : map.vertices) {
      target.draw(layerData.first, states);
    }
  }
}

bool TilemapSystem::loadTileset(Tileset& set) const {
  ZoneScoped;

  // (Re)Loads the tileset
  set.handle = m_textureCache->load<TextureLoader>(set.tilesetId, set.path);

  // Store the tileset size (number of rows and columns)
  set.tilesetSize.x = set.handle->getSize().x / set.tileSize.x;
  set.tilesetSize.y = set.handle->getSize().y / set.tileSize.y;

  if (set.tilesetSize.x == 0 || set.tilesetSize.y == 0) {
    LOG(ERROR) << "TilemapSystem::loadMap error: "
               << "Cannot determine tileset size.";
    return false;
  }

  return true;
}

/*!
 * \brief TilemapSystem::loadMap
 *
 * Loading a map consists of a few steps:
 *  1. For each layer
 *    1. Load data
 *    2. Decode and save the map vertices
 *
 * \param map
 * \param set
 */
void TilemapSystem::loadMap(Tilemap& map, Tileset& set) const {
  ZoneScoped;

  /// Converts a map type to an extension
  auto typeToExtension = [](auto type) -> std::string {
    switch (type) {
      case Tilemap::CSV:
        return ".csv";
    }
    return "";
  };

  for (const auto& layer : map.layers) {
    LOG(INFO) << "Loading layer z=" << layer.second;

    map.vertices.emplace_back(std::make_pair(sf::VertexArray(), layer.second));
    // Find matching layerData
    /*auto it =
        std::find_if(map.vertices.begin(), map.vertices.end(),
                     [&layer](std::pair<sf::VertexArray, int32_t> const& elem) {
                       return elem.second == layer.second;
                     });*/
    auto&& layerVertices = map.vertices.back().first;

    LOG(INFO) << "Found layerVertices";

    const auto layerFile = map.path + layer.first + typeToExtension(map.type);
    std::ifstream infile(layerFile);

    std::vector<uint32_t> layerData;
    sf::Vector2u size;

    if (!infile.is_open()) {
      LOG(ERROR) << "TilemapSystem::loadMap error: Cannot open map layer file "
                 << layerFile << ".";
      continue;
    }

    // Read the data depending on type
    if (map.type == Tilemap::CSV) {
      std::string line;
      unsigned int rows = 0;
      unsigned int cols = 0;

      // For each row
      while (std::getline(infile, line)) {
        rows++;
        std::istringstream s(line);
        std::string field;
        // For each column
        while (getline(s, field, ',')) {
          cols++;
          layerData.push_back(std::stoul(field));
        }
      }
      map.size.x = cols / rows;
      map.size.y = rows;
    } /*else if (map.type == Tilemap::OtherType) {
    }*/

    // Convert the data to the vertex array
    uint32_t const& width  = map.size.x;
    uint32_t const& height = map.size.y;

    uint32_t const& tileWidth  = set.tileSize.x;
    uint32_t const& tileHeight = set.tileSize.y;

    uint32_t const& tilesetCols = set.tilesetSize.x;

    layerVertices.setPrimitiveType(sf::Quads);
    layerVertices.resize(width * height * 4);

    // Reads the tile map from left to right then from top to bottom
    for (uint32_t iCol = 0; iCol < width; iCol++)
      for (uint32_t iRow = 0; iRow < height; iRow++) {
        // Get the tile ID
        uint32_t tileId = layerData[iCol + iRow * width];

        // Find the tile ID in the tileset
        uint32_t tilesetCol = 0;
        uint32_t tilesetRow = 0;
        tilesetCol          = tileId % tilesetCols;
        tilesetRow          = tileId / tilesetCols;

        // Modifiable reference
        auto&& quad = &layerVertices[(iCol + iRow * width) * 4];

        // Set the 4 corner's positions
        quad[0].position = sf::Vector2f(iCol, iRow);
        quad[1].position = sf::Vector2f((iCol + 1), iRow);
        quad[2].position = sf::Vector2f((iCol + 1), (iRow + 1));
        quad[3].position = sf::Vector2f(iCol, (iRow + 1));

        // Set the 4 corner's texture coordinates
        quad[0].texCoords =
            sf::Vector2f(tilesetCol * tileWidth, tilesetRow * tileHeight);
        quad[1].texCoords =
            sf::Vector2f((tilesetCol + 1) * tileWidth, tilesetRow * tileHeight);
        quad[2].texCoords = sf::Vector2f((tilesetCol + 1) * tileWidth,
                                         (tilesetRow + 1) * tileHeight);
        quad[3].texCoords =
            sf::Vector2f(tilesetCol * tileWidth, (tilesetRow + 1) * tileHeight);
      }
  }

  // Sort the layers by z order
  std::sort(map.vertices.begin(), map.vertices.end(),
            [](auto& left, auto& right) { return left.second < right.second; });
}

/*
std::vector<int32_t> TilemapSystem::openMap(Tilemap& map, Tileset& set) {
  ZoneScoped;

auto typeToExtension = [](auto type) -> std::string {
  switch (type) {
    case Tilemap::CSV:
      return ".csv";
  }
  return "";
};

for (auto&& layer : map.layers) {
  std::string layerfile = map.path + layer.first + typeToExtension(map.type);
  std::ifstream infile(layerfile);

  std::vector<int32_t> mapData;
  sf::Vector2u size;

  if (!infile.is_open()) {
    LOG(ERROR) << "TilemapSystem::openMap error: Cannot open map layer file "
               << layerfile;
    return mapData;
  }

  if (map.type == Tilemap::CSV) {
    std::string line;
    unsigned int rows = 0;
    unsigned int cols = 0;

    // For each row
    while (std::getline(infile, line)) {
      rows++;
      std::istringstream s(line);
      std::string field;
      // For each column
      while (getline(s, field, ',')) {
        cols++;
        mapData.push_back(std::stoi(field));
      }
    }
    map.size.x = cols / rows;
    map.size.y = rows;
  }
}


}

void TilemapSystem::decodeMap(Tilemap& map,
                              Tileset& properties,
                              std::vector<int32_t> data) {
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
      quad[0].texCoords =
          sf::Vector2f(tilesetCol * tileWidth, tilesetRow * tileHeight);
      quad[1].texCoords =
          sf::Vector2f((tilesetCol + 1) * tileWidth, tilesetRow * tileHeight);
      quad[2].texCoords = sf::Vector2f((tilesetCol + 1) * tileWidth,
                                       (tilesetRow + 1) * tileHeight);
      quad[3].texCoords =
          sf::Vector2f(tilesetCol * tileWidth, (tilesetRow + 1) * tileHeight);
    }
}
*/
