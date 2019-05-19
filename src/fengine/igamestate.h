#ifndef IGAMESTATE_H
#define IGAMESTATE_H

#include <SFML/Window/Event.hpp>
#include <entt/entt.hpp>

#include "engine.h"
#include "utils/timing.h"

namespace frames {

class IGameState {
public:
    virtual void init(Engine* engine) { m_engine = engine; }
    virtual void cleanup() = 0;

    virtual void processEvent(entt::registry& reg, sf::Event& event)        = 0;
    virtual void processUpdate(entt::registry& reg, const double dt)        = 0;
    virtual void processDraw(entt::registry& reg, sf::RenderTarget& target) = 0;

protected:
    IGameState() {}

    Engine* m_engine = nullptr;
};

}

#endif // IGAMESTATE_H
