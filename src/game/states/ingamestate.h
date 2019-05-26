#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include <fengine/igamestate.h>

#include "../systems/tilemapsystem.h"

class IngameState : public frames::IGameState {
public:
    void init(frames::Engine* engine);
    void cleanup();

    void processEvent(entt::registry& reg, sf::Event& event);
    void processUpdate(entt::registry& reg, const double dt);
    void processDraw(entt::registry& reg, sf::RenderTarget& target);

private:
    void resizeToMap();

private:
    TilemapSystem m_tilemapSystem;

    TextureCachePtr m_textureCache;

    /// Singleton
    DECLARE_SINGLETON(IngameState)
};

#endif // INGAMESTATE_H
