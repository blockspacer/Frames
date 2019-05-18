#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <fengine/igamestate.h>

class MenuState : public frames::IGameState {
public:
    void init(frames::Engine& engine);
    void cleanup();

    void processEvent(frames::Engine& engine, sf::Event& event);
    void processUpdate(frames::Engine& engine,
                       const frames::timing::Clock::duration& delta,
                       entt::registry& reg);
    void processDraw(frames::Engine& engine);

    static MenuState* instance()
    {
        return &m_state;
    }

protected:
    MenuState() {}

private:
    static MenuState m_state;
};

#endif // MENUSTATE_H
