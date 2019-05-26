#ifndef TILEMAPSYSTEM_H
#define TILEMAPSYSTEM_H

#include "../components/tilemap.h"
#include "../components/tilemapproperties.h"
#include <fengine/isystem.h>

class TilemapSystem : public frames::ISystem {
public:
    void init(frames::Engine* engine);
    void cleanup();

    void processUpdate(entt::registry& reg, const double dt);
    void processDraw(entt::registry& reg, sf::RenderTarget& target);

private:
    void loadMaps(entt::registry& reg);

    void loadTileset(Tilemap& map, TilemapProperties& properties);
    std::vector<int16_t> openMap(Tilemap& map, TilemapProperties& properties);
    void decodeMap(Tilemap& map, TilemapProperties& properties,
                   std::vector<int16_t> data);

private:
    TextureCachePtr m_textureCache;
};

#endif // TILEMAPSYSTEM_H
