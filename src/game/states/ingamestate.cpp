#include "ingamestate.h"

#include <easylogging++/easylogging++.h>

using namespace frames;

DEFINE_SINGLETON(IngameState)

void IngameState::init(Engine* engine)
{
    ZoneScoped;
    IGameState::init(engine);

    TilemapSystem::MapProperties map;
    map.tilesetFile = "data/tileset.png";
    map.type        = TilemapSystem::MapProperties::CSV;
    map.tileSize    = sf::Vector2u(16, 16);

    map.file = "data/map_foreground.csv";
    m_mapFG.init(engine, map);

    map.file = "data/map_background.csv";
    m_mapBG.init(engine, map);

    LOG(INFO) << "IngameState::init finished";
}

void IngameState::cleanup()
{
    ZoneScoped;

    m_mapFG.cleanup();
    m_mapBG.cleanup();

    LOG(INFO) << "IngameState::cleanup finished";
}

void IngameState::processEvent(entt::registry& reg, sf::Event& event)
{
    ZoneScoped;
}

void IngameState::processUpdate(entt::registry& reg, const double dt)
{
    ZoneScoped;

    m_mapFG.processUpdate(reg, dt);
    m_mapBG.processUpdate(reg, dt);
}

void IngameState::processDraw(entt::registry& reg, sf::RenderTarget& target)
{
    ZoneScoped;

    target.clear(sf::Color(135, 206, 235));

    m_mapBG.processDraw(reg, target);
    m_mapFG.processDraw(reg, target);
}
