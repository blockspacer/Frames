#ifndef IGAMESTATE_H
#define IGAMESTATE_H

#include <SFML/Window/Event.hpp>
#include <entt/entt.hpp>

#include "engine.h"
#include "utils/timing.h"

namespace frames {

class IGameState {
public:
    virtual void init(Engine& engine) = 0;
    virtual void cleanup()            = 0;

    // Frame-related functions
    virtual void processEvent(Engine& engine, sf::Event& event) = 0;
    virtual void processUpdate(Engine& engine,
                               const timing::Clock::duration& delta,
                               entt::registry& reg)
        = 0;
    virtual void processDraw(Engine& engine) = 0;

    void setPaused(bool paused)
    {
        m_paused = paused;
    }

    void changeState(Engine& engine, IGameState* state)
    {
        engine.changeState(state);
    }

protected:
    IGameState() {}

    bool m_paused;
};

}

#endif // IGAMESTATE_H
