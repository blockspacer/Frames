#include <easylogging++/easylogging++.h>

#include "introstate.h"
#include "menustate.h"

#include <fengine/utils/easing.h>

using namespace frames;
using namespace frames::timing;

IntroState IntroState::m_state;

void IntroState::init(Engine& engine)
{
    ZoneScoped;
    LOG(INFO) << "IntroState::init";
}

void IntroState::cleanup()
{
    ZoneScoped;
}

void IntroState::processEvent(Engine& engine, sf::Event& event)
{
    ZoneScoped;
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        case sf::Keyboard::K:
            m_easingTime = 0.f;
            break;
        case sf::Keyboard::P:
            setPaused(!m_paused);
            break;
        case sf::Keyboard::Tab:
            break;
        default:
            break;
        }
    }

    if (m_easingTime == m_easingTotal)
        engine.changeState(MenuState::instance());
}

void IntroState::processUpdate(Engine& engine, const Clock::duration& delta,
                               entt::registry& reg)
{
    ZoneScoped;

    if (ImGui::Begin("IntroState")) {
        if (ImGui::CollapsingHeader("Easing properties")) {
            ImGui::SliderFloat("Duration", &m_easingTotal, 0.f, 10.f);
            ImGui::SliderFloat("EasingTime", &m_easingTime, 0.f, m_easingTotal);
            ImGui::SliderInt("Greyscale", &m_bgGreyScale, 0, 255);

            const char* items[] = {
                "InSine", "InQuad", "InCubic", "OutSine", "OutQuad",
                "OutCubic", "InOutSine", "InOutQuad", "InOutCubic"
            };
            static const char* item_current = items[0];
            if (ImGui::BeginCombo("Easing type", item_current)) {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
                    bool is_selected = (item_current == items[n]);
                    if (ImGui::Selectable(items[n], is_selected)) {
                        item_current = items[n];
                        m_easingType = static_cast<easing::EasingFunction>(n);
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }
    }
    ImGui::End();

    if (m_easingTime < m_easingTotal)
        m_easingTime += std::chrono::duration_cast<dsec>(delta).count();
    else if (m_easingTime > m_easingTotal)
        m_easingTime = m_easingTotal;

    m_bgGreyScale = 255 * easing::ease(m_easingType, 1. - m_easingTime / m_easingTotal);
}

void IntroState::processDraw(Engine& engine)
{
    ZoneScoped;
    const auto target = engine.target();

    target->clear(sf::Color(m_bgGreyScale, m_bgGreyScale, m_bgGreyScale));
}
