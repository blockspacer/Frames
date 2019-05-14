// Initialize Easylogging++
#define ELPP_STL_LOGGING
#include "libs/easylogging++/easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include "fengine.h"

#include "libs/imgui/imgui.h"

void FEngine::init(std::string title, unsigned int width, unsigned int height)
{
    el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Format, "[%levshort] %func\n    %msg");

    m_window = new sf::RenderWindow(sf::VideoMode(width, height),
                                    title, sf::Style::Default);
}

void FEngine::start()
{
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
}

void FEngine::quit()
{
    m_running = false;
}

void FEngine::processRender()
{
    m_window->display();
}

void FEngine::processPhysics()
{
}

void FEngine::processEvents()
{
    sf::Event event;
    while (m_window->pollEvent(event)) {
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
