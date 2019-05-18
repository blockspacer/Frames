#include <easylogging++/easylogging++.h>

#include "menustate.h"

using namespace frames;
using namespace frames::timing;

MenuState MenuState::m_state;

void MenuState::init(Engine& engine)
{
    ZoneScoped;
    LOG(INFO) << "MenuState::init";
}

void MenuState::cleanup()
{
    ZoneScoped;

    LOG(INFO) << "MenuState::cleanup";
}

void MenuState::processEvent(Engine& engine, sf::Event& event)
{
    ZoneScoped;
}

void MenuState::processUpdate(Engine& engine, const Clock::duration& delta, entt::registry& reg)
{
    ZoneScoped;
    dsec mytime = delta;
}

void MenuState::processDraw(Engine& engine)
{
    ZoneScoped;
    const auto target = engine.target();

    target->clear();
}
