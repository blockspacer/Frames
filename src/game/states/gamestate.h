#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <fengine/igamestate.h>
#include <fengine/utils/easing.h>

class GameState : public frames::IGameState {
public:
    void init(frames::Engine* engine);
    void cleanup();

    void processEvent(entt::registry& reg, sf::Event& event);
    void processUpdate(entt::registry& reg, const double dt);
    void processDraw(entt::registry& reg, sf::RenderTarget& target);

    static GameState* instance()
    {
        return &m_state;
    }

private:
    GameState() {}
    static GameState m_state;
};

#endif // GAMESTATE_H
