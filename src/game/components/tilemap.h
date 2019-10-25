#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

struct Tilemap {
  enum Type { CSV };

  std::string name, path;
  std::vector<std::pair<std::string, int32_t>> layers;
  Type type;
  entt::hashed_string tileset;

  bool loaded = false;
  sf::Vector2u size;
  std::vector<std::pair<sf::VertexArray, int32_t>> vertices;
};

#endif // TILEMAP_H
