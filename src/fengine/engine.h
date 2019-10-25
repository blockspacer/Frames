#ifndef FRAMES_ENGINE_H
#define FRAMES_ENGINE_H

#include <string>

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "scheduler.h"
#include "ui/timegraph.h"

typedef std::shared_ptr<entt::resource_cache<sf::Texture>> TextureCachePtr;

namespace frames {

class IGameState;

class Engine {
public:
    Engine() {};

    void init(std::string title, unsigned int width = 1280, unsigned int height = 720);
    void cleanup();

    void setAutoSnap(unsigned int width, unsigned int height);
    void setFramerateLimit(unsigned int framerate);

    void start();
    void quit();

    void update();
    bool running();

    void changeState(IGameState* state);
    void pushState(IGameState* state);
    void popState();

    sf::RenderTarget* getTarget() { return m_window; }
    entt::registry& getRegistry() { return m_registry; }
    TextureCachePtr getTextureCache() { return m_textureCache; }

private:
    void snap(unsigned int width, unsigned int height);

    void processEvents();
    void processPhysics(timing::Clock::duration delta);
    void processRender(timing::Clock::duration delta);

private:
    sf::RenderWindow* m_window;
    Scheduler m_scheduler;
    entt::registry m_registry;

    TextureCachePtr m_textureCache;

    std::vector<IGameState*> m_states;

    ui::TimeGraph<200>* m_frametime;

    bool m_snapping = false, m_justsnapped = false;
    sf::Vector2u m_snapSize;
    bool m_running = false;
};

}

#endif // FRAMES_ENGINE_H
