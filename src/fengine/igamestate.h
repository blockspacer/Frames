#ifndef IGAMESTATE_H
#define IGAMESTATE_H

#include "utils/timing.h"
#include <entt/entt.hpp>

namespace frames {

class IGameState {
public:
    virtual void init()    = 0;
    virtual void cleanup() = 0;

    // Frame-related functions
    virtual void update(
        const timing::Clock::duration& delta, entt::registry& reg)
        = 0;
    virtual void draw() = 0;
    virtual void setPaused(bool paused)
    {
        m_paused = paused;
    }

    // Resize & Input callbacks
    virtual void windowResized(int width, int height) = 0;

    virtual void inputKey(int key, int scancode, int action, int mods) = 0;
    virtual void inputMouseButton(int button, int action, int mods)    = 0;
    virtual void inputMousePos(double xpos, double ypos)               = 0;
    virtual void inputScroll(float dx, float dy)                       = 0;

protected:
    IGameState() {}

    bool m_paused;
};

}

#endif // IGAMESTATE_H
