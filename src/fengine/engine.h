#ifndef FRAMES_ENGINE_H
#define FRAMES_ENGINE_H

#include <string>

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "scheduler.h"
#include "ui/metrics.h"

namespace frames {

class Engine {
public:
    Engine() {};

    void init(std::string title, unsigned int width = 1280, unsigned int height = 720);
    void start();

    void update();
    bool running();

    void quit();
    void cleanup();

private:
    void processEvents();
    void processRender(timing::Clock::duration delta);
    void processPhysics(timing::Clock::duration delta);

private:
    sf::RenderWindow* m_window;
    Scheduler m_scheduler;
    entt::registry m_registry;

    ui::Metric<200>* m_frametime;

    bool m_running = false;
};

}

#endif // FRAMES_ENGINE_H
