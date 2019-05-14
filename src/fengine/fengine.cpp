// Initialize Easylogging++
#define ELPP_STL_LOGGING
#include "libs/easylogging++/easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "fengine.h"

#include <imgui/imgui.h>
#include <imgui/imgui_SFML.h>

void FEngine::init(std::string title, unsigned int width, unsigned int height)
{
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "[%levshort] %msg");

    m_window = new sf::RenderWindow(sf::VideoMode(width, height),
                                    title, sf::Style::Default);

    ImGui::SFML::Init(*m_window);

    m_running = false;

    LOG(INFO) << "Initialization finished";
}

void FEngine::start()
{
    while (!m_window->isOpen()) {
    }

    m_running = true;
}

void FEngine::update()
{
    // Events
    processEvents();

    // Update
    processPhysics();

    // Render
    processRender();
}

bool FEngine::running()
{
    return m_running;
}

void FEngine::cleanup()
{
    ImGui::SFML::Shutdown();

    if (m_window->isOpen())
        m_window->close();
    delete m_window;

    LOG(INFO) << "Cleanup finished";
}

void FEngine::quit()
{
    m_running = false;
}

void FEngine::processRender()
{
    m_window->clear();

    ImGui::SFML::Render(*m_window);

    m_window->display();
}

void FEngine::processPhysics()
{
    ImGui::SFML::Update(*m_window, 0.016);

    ImGui::Begin("Hello world!");
    ImGui::End();
}

void FEngine::processEvents()
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
