#ifndef INGAMESTATE_H
#define INGAMESTATE_H

#include <fengine/igamestate.h>

class IngameState : public frames::IGameState {
public:
    void init(frames::Engine* engine);
    void cleanup();

    void processEvent(entt::registry& reg, sf::Event& event);
    void processUpdate(entt::registry& reg, const double dt);
    void processDraw(entt::registry& reg, sf::RenderTarget& target);

    static IngameState* instance()
    {
        return &m_state;
    }

private:
    IngameState() {}
    static IngameState m_state;
};

#endif // INGAMESTATE_H