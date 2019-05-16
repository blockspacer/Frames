// Initialize Easylogging++
#define ELPP_STL_LOGGING
#include <easylogging++/easylogging++.h>
INITIALIZE_EASYLOGGINGPP

#include <thread>

#include <imgui-SFML/imgui_SFML.h>
#include <imgui/imgui.h>

#include "engine.h"
#include "utils/timing.h"

namespace frames {

void Engine::init(std::string title, unsigned int width, unsigned int height)
{
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

    auto t1 = timing::Clock::now();
    timing::Clock::calibrate();
    auto t2 = timing::Clock::now();

    LOG(INFO) << "dt : " << std::chrono::duration_cast<timing::dmilli>(t2 - t1).count();

    m_running = false;
    LOG(INFO) << "Initialization finished";
}

void Engine::start()
{
    while (!m_window->isOpen()) {
    }

    m_running = true;

    LOG(INFO) << "Engine started";
}

void Engine::update()
{
    m_scheduler.update();
}

bool Engine::running()
{
    return m_running;
}

void Engine::cleanup()
{
    ImGui::SFML::Shutdown();

    if (m_window->isOpen())
        m_window->close();
    delete m_window;

    delete m_frametime;

    LOG(INFO) << "Cleanup finished";
}

void Engine::quit()
{
    m_running = false;

    LOG(INFO) << "Exiting";
}

void Engine::processRender(timing::Clock::duration delta)
{
    SCOPED_MEASURE([=](timing::Clock::duration time) {
    });

    const auto dt = std::chrono::duration_cast<timing::dsec>(delta).count();
    m_frametime->update(delta);

    m_window->clear();

    ImGui::SFML::Update(*m_window, dt);

    ImGui::Begin("Hello world!");
    ImGui::End();

    m_frametime->render();

    ImGui::EndFrame();

    ImGui::SFML::Render(*m_window);

    m_window->display();
}

void Engine::processPhysics(timing::Clock::duration delta)
{
    const auto dt = std::chrono::duration_cast<timing::dsec>(delta).count();
}

void Engine::processEvents()
{
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

} // namespace Frames
