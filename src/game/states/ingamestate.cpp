#include "ingamestate.h"

#include <easylogging++/easylogging++.h>

using namespace frames;

DEFINE_SINGLETON(IngameState)

void IngameState::init(Engine* engine)
{
    ZoneScoped;
    IGameState::init(engine);

    LOG(INFO) << "IngameState::init finished";
}

void IngameState::cleanup()
{
    ZoneScoped;

    LOG(INFO) << "IngameState::cleanup finished";
}

void IngameState::processEvent(entt::registry& reg, sf::Event& event)
{
    ZoneScoped;
}

void IngameState::processUpdate(entt::registry& reg, const double dt)
{
    ZoneScoped;
}

void IngameState::processDraw(entt::registry& reg, sf::RenderTarget& target)
{
    ZoneScoped;

    target.clear();
}
