#ifndef ISYSTEM_H
#define ISYSTEM_H

#include <SFML/Window/Event.hpp>
#include <entt/entt.hpp>

#include "engine.h"

namespace frames {

class ISystem {
public:
    virtual void init(Engine* engine) { m_engine = engine; }
    virtual void cleanup() = 0;

    virtual void processUpdate(entt::registry& reg, const double dt)        = 0;
    virtual void processDraw(entt::registry& reg, sf::RenderTarget& target) = 0;

protected:
    ISystem() {}

    Engine* m_engine = nullptr;
};

}

#endif // ISYSTEM_H
