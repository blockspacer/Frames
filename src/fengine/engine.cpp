#include <thread>

#include <imgui-SFML/imgui_SFML.h>
#include <imgui/imgui.h>

// Initialize Easylogging++
#define ELPP_STL_LOGGING
#include <easylogging++/easylogging++.h>
INITIALIZE_EASYLOGGINGPP

#include <Tracy.hpp>

#include "engine.h"
#include "igamestate.h"
#include "utils/timing.h"

namespace frames {

void Engine::init(std::string title, unsigned int width, unsigned int height)
{
    ZoneScoped;
    LOG(INFO) << "Engine::init";
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "[%levshort] %msg");

    m_window = new sf::RenderWindow(sf::VideoMode(width, height),
                                    title, sf::Style::Default);

    ImGui::SFML::Init(*m_window);

    m_frametime = new ui::TimeGraph<200>("Frametime (ms)");

    // Scheduler
    m_scheduler.init();
    m_scheduler.add(std::bind(&Engine::processEvents, this), 200);
    m_scheduler.add(std::bind(&Engine::processPhysics, this, std::placeholders::_1), 120);
    m_scheduler.add(std::bind(&Engine::processRender, this, std::placeholders::_1), 60);

    // Calibrate clock (if needed)
    timing::Clock::calibrate();

    m_running = false;

    LOG(INFO) << "Engine::init finished";
}

void Engine::start()
{
    ZoneScoped;
    LOG(INFO) << "Engine::start";

    m_running = true;
}

void Engine::quit()
{
    ZoneScoped;
    m_running = false;

    LOG(INFO) << "Engine::quit";
}

void Engine::update()
{
    m_scheduler.update();
}

bool Engine::running()
{
    return m_running;
}

void Engine::changeState(IGameState* state, bool init)
{
    ZoneScoped;
    LOG(INFO) << "Engine::changeState";

    if (!m_states.empty()) {
        m_states.back()->cleanup();
        m_states.pop_back();
    }

    m_states.push_back(state);
    if (init) {
        state->init();
    }
}

void Engine::pushState(IGameState* state)
{
    ZoneScoped;
    LOG(INFO) << "Engine::pushState";

    if (!m_states.empty()) {
        m_states.back()->setPaused(true);
    }

    m_states.push_back(state);
    state->init();
}

void Engine::popState()
{
    ZoneScoped;
    LOG(INFO) << "Engine::popState";

    if (!m_states.empty()) {
        m_states.back()->cleanup();
        m_states.pop_back();
    }

    if (!m_states.empty()) {
        m_states.back()->setPaused(false);
    }
}

void Engine::cleanup()
{
    ZoneScoped;
    LOG(INFO) << "Engine::cleanup";

    {
        ZoneScopedN("ImGui::SFML::Shutdown");
        ImGui::SFML::Shutdown();
    }

    {
        ZoneScopedN("Close");
        if (m_window->isOpen())
            m_window->close();
    }

    {
        ZoneScopedN("Delete");
        delete m_window;
        delete m_frametime;
    }

    while (!m_states.empty()) {
        m_states.back()->cleanup();
        m_states.pop_back();
    }

    LOG(INFO) << "Engine::cleanup finished";
}

void Engine::processEvents()
{
    ZoneScoped;

    sf::Event event;
    while (m_window->pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed)
            quit();
        if (event.type == sf::Event::LostFocus) {
        }
        if (event.type == sf::Event::Resized) {
            m_window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
        }
        if (event.type == sf::Event::KeyPressed)
            if (event.key.code == sf::Keyboard::Escape)
                quit();
    }
}

void Engine::processPhysics(timing::Clock::duration delta)
{
    ZoneScoped;

    if (!m_states.empty()) {
        m_states.back()->update(delta, m_registry);
    }
}

void Engine::processRender(timing::Clock::duration delta)
{
    ZoneScoped;

    const auto dt = std::chrono::duration_cast<timing::dsec>(delta).count();
    m_frametime->update(delta);

    m_window->clear();

    ImGui::SFML::Update(*m_window, dt);

    if (!m_states.empty()) {
        ZoneScopedN("State rendering");
        m_states.back()->draw();
    }

    ImGui::Begin("Hello world!");
    ImGui::End();

    m_frametime->render();

    ImGui::ShowStyleEditor();

    ImGui::EndFrame();

    ImGui::SFML::Render(*m_window);

    m_window->display();

    FrameMark;
}

} // namespace Frames
