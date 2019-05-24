#ifndef TILEMAPSYSTEM_H
#define TILEMAPSYSTEM_H

#include <fengine/isystem.h>
#include <fengine/resources/textureloader.h>

class TilemapSystem : public frames::ISystem,
                      public sf::Drawable,
                      public sf::Transformable {
public:
    struct MapProperties {
        enum MapType {
            CSV
        };

        std::string file, tilesetFile;
        MapType type;
        sf::Vector2u tileSize;
    };

    void
    init(frames::Engine* engine, MapProperties map);
    void cleanup();

    void processUpdate(entt::registry& reg, const double dt);
    void processDraw(entt::registry& reg, sf::RenderTarget& target);

private:
    void loadTileset(const std::string& filename, sf::Vector2u tileSize);
    void loadMap(const std::vector<int16_t>& map,
                 const sf::Vector2u size, const sf::Vector2u tilesize);
    std::tuple<std::vector<int16_t>, sf::Vector2u>
    decodeMap(const MapProperties& mapfile);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::VertexArray m_vertices;
    entt::resource_cache<sf::Texture> m_textureCache;

    sf::Vector2u m_tileSize;
    unsigned int m_tilesetCols;
};

#endif // TILEMAPSYSTEM_H
