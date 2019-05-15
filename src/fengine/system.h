#ifndef SYSTEM_H
#define SYSTEM_H

#include <entt/entt.hpp>

#include "engine.h"
#include "utils/timing.h"

namespace frames {

class ISystem {
public:
    virtual ~ISystem();

    virtual void init()                                                  = 0;
    virtual void update(entt::registry& reg, timing::Clock::duration dt) = 0;

    virtual uint16_t getUpdateRate();
    virtual std::string getName();
    virtual bool isPaused();

private:
    bool m_paused = false;
};

} // namespace frames

#endif // SYSTEM_H
