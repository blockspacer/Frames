#ifndef FRAMES_TEXTURELOADER_H
#define FRAMES_TEXTURELOADER_H

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

struct TextureLoader final : entt::resource_loader<TextureLoader, sf::Texture> {
    std::shared_ptr<sf::Texture> load(
        std::string filename, sf::IntRect area = sf::IntRect())
        const;

    std::shared_ptr<sf::Texture> load(
        std::string filename, const sf::Vector2i& position,
        const sf::Vector2i& size)
        const;
};

#endif // FRAMES_TEXTURELOADER_H
