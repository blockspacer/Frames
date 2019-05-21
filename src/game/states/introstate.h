#ifndef INTROSTATE_H
#define INTROSTATE_H

#include <fengine/igamestate.h>
#include <fengine/utils/easing.h>

class IntroState : public frames::IGameState {
public:
    void init(frames::Engine* engine);
    void cleanup();

    void processEvent(entt::registry& reg, sf::Event& event);
    void processUpdate(entt::registry& reg, const double dt);
    void processDraw(entt::registry& reg, sf::RenderTarget& target);

    static IntroState* instance()
    {
        return &m_state;
    }

private:
    IntroState() {}
    static IntroState m_state;

    int m_bgGreyScale  = 0;
    float m_easingTime = 0.f, m_easingTotal = 1.5f;
    frames::easing::EasingFunction m_easingType;
};

#endif // INTROSTATE_H