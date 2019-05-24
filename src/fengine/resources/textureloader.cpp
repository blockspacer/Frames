#include "textureloader.h"

std::shared_ptr<sf::Texture> TextureLoader::load(std::string filename,
                                                 sf::IntRect area) const
{
    sf::Texture* res = new sf::Texture();
    if (!res->loadFromFile(filename, area)) {
        delete res;
        return std::shared_ptr<sf::Texture>(nullptr);
    }

    return std::shared_ptr<sf::Texture>(res);
}

std::shared_ptr<sf::Texture> TextureLoader::load(std::string filename,
                                                 const sf::Vector2i& position,
                                                 const sf::Vector2i& size) const
{
    sf::Texture* res = new sf::Texture();
    if (!res->loadFromFile(filename, sf::IntRect(position, size))) {
        delete res;
        return std::shared_ptr<sf::Texture>(nullptr);
    };

    return std::shared_ptr<sf::Texture>(res);
}
