#include <easylogging++/easylogging++.h>

#include "gamestate.h"

#include <fengine/utils/easing.h>

using namespace frames;

GameState GameState::m_state;

void GameState::init(Engine* engine)
{
    ZoneScoped;
    IGameState::init(engine);

    LOG(INFO) << "GameState::init finished";
}

void GameState::cleanup()
{
    ZoneScoped;

    LOG(INFO) << "GameState::cleanup finished";
}

void GameState::processEvent(entt::registry& reg, sf::Event& event)
{
    ZoneScoped;

    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default:
            break;
        }
    }
}

void GameState::processUpdate(entt::registry& reg, const double dt)
{
    ZoneScoped;

    if (ImGui::Begin("GameState")) {
    }
    ImGui::End();
}

void GameState::processDraw(entt::registry& reg, sf::RenderTarget& target)
{
    ZoneScoped;

    target.clear();
}
