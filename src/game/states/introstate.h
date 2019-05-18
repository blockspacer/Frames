#ifndef INTROSTATE_H
#define INTROSTATE_H

#include <fengine/igamestate.h>
#include <fengine/utils/easing.h>

class IntroState : public frames::IGameState {
public:
    void init(frames::Engine& engine);
    void cleanup();

    void processEvent(frames::Engine& engine, sf::Event& event);
    void processUpdate(frames::Engine& engine,
                       const frames::timing::Clock::duration& delta,
                       entt::registry& reg);
    void processDraw(frames::Engine& engine);

    static IntroState* instance()
    {
        return &m_state;
    }

protected:
    IntroState() {}

private:
    static IntroState m_state;

    int m_bgGreyScale  = 0;
    float m_easingTime = 0.f, m_easingTotal = 1.5f;
    frames::easing::EasingFunction m_easingType;
};

#endif // INTROSTATE_H
