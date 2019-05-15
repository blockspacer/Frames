#ifndef FRAMES_ENGINE_H
#define FRAMES_ENGINE_H

#include <string>

#include <SFML/Graphics.hpp>

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
    void processRender();
    void processPhysics();

private:
    sf::RenderWindow* m_window;

    bool m_running = false;
};

}

#endif // FRAMES_ENGINE_H
