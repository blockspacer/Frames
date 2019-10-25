#ifndef TILEMAPPROPERTIES_H
#define TILEMAPPROPERTIES_H

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <string>

struct Tileset {
  std::string name, path;
  entt::hashed_string tilesetId;
  sf::Vector2u size, tilesetSize, tileSize;
  unsigned int layer = 0;
  entt::resource_handle<sf::Texture> handle;
};

#endif // TILEMAPPROPERTIES_H
