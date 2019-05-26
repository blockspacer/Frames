#ifndef TILEMAPPROPERTIES_H
#define TILEMAPPROPERTIES_H

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <string>

struct TilemapProperties {
    enum MapType {
        CSV
    };

    std::string file, tilesetFile;
    entt::hashed_string tilesetId;
    MapType type;
    sf::Vector2u size, tilesetSize, tileSize;
    unsigned int layer = 0;
};

#endif // TILEMAPPROPERTIES_H
