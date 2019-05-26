#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

struct Tilemap {
    bool loaded = false;
    entt::resource_handle<sf::Texture> tilesetHandle;
    sf::VertexArray vertices;
};

#endif // TILEMAP_H
