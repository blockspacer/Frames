#ifndef FENGINE_H
#define FENGINE_H

#include <string>

#include <SFML/Graphics.hpp>

class FEngine {
public:
    FEngine() {};

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

#endif // FENGINE_H
