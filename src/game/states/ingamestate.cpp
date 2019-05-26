#include "ingamestate.h"

#include <easylogging++/easylogging++.h>

#include <fengine/resources/textureloader.h>

#include "../components/movement.h"
#include "../components/position.h"
#include "../components/sprite.h"
#include "../components/tilemap.h"
#include "../components/tilemapproperties.h"
#include "../components/velocity.h"

using namespace frames;

using TextureCache = entt::resource_cache<sf::Texture>;

DEFINE_SINGLETON(IngameState)

void IngameState::init(Engine* engine)
{
    ZoneScoped;
    IGameState::init(engine);

    auto&& registry = engine->getRegistry();

    // Create tilemaps
    TilemapProperties properties;
    properties.tilesetFile = "data/tileset.png";
    properties.type        = TilemapProperties::CSV;
    properties.tileSize    = sf::Vector2u(16, 16);
    properties.tilesetId   = "textures/tileset"_hs;

    // Foreground tilemap
    properties.file  = "data/map_foreground.csv";
    properties.layer = 0;
    auto&& mapFG     = registry.create();
    registry.assign<Tilemap>(mapFG);
    registry.assign<TilemapProperties>(mapFG, properties);
    //m_mapFG.init(engine, map);

    // Background tilemap
    properties.file  = "data/map_background.csv";
    properties.layer = 1;
    auto&& mapBG     = registry.create();
    registry.assign<Tilemap>(mapBG);
    registry.assign<TilemapProperties>(mapBG, properties);
    //m_mapBG.init(engine, map);

    m_tilemapSystem.init(m_engine);

    resizeToMap();

    m_textureCache = engine->getTextureCache();

    // Create the player
    //auto&& [player, tag] = registry->create<entt::tag<"player"_hs>>();
    auto&& player       = registry.create();
    auto&& playerSprite = registry.assign<sf::Sprite>(player);
    registry.assign<Position>(player, 6.f, 14.f);
    registry.assign<Movement>(player);
    registry.assign<Velocity>(player);
    registry.assign<entt::tag<"player"_hs>>(player);

    m_textureCache->load<TextureLoader>("textures/player/idle1"_hs, "data/Player/idle/anim1.png");
    auto playerTexture = m_textureCache->handle("textures/player/idle1"_hs);

    playerSprite.setTexture(playerTexture);
    playerSprite.setOrigin(playerTexture->getSize().x / 2., playerTexture->getSize().y);
    float factor = 2. / playerTexture->getSize().y;
    playerSprite.setScale(factor, factor);

    LOG(INFO) << "IngameState::init finished";
}

void IngameState::cleanup()
{
    ZoneScoped;

    m_tilemapSystem.cleanup();

    m_textureCache->discard("textures/player/idle1"_hs);

    LOG(INFO) << "IngameState::cleanup finished";
}

void IngameState::processEvent(entt::registry& reg, sf::Event& event)
{
    ZoneScoped;

    if (event.type == sf::Event::Resized) {
        resizeToMap();
    }

    // Movement
    if (event.type == sf::Event::KeyPressed
        || event.type == sf::Event::KeyReleased) {
        auto view          = reg.view<entt::tag<"player"_hs>>();
        const auto& player = *(view.begin());
        auto&& movement    = reg.get<Movement>(player);
        const bool pressed = event.type == sf::Event::KeyPressed;

        switch (event.key.code) {
        case sf::Keyboard::Z:
            movement.up = pressed;
            break;
        case sf::Keyboard::S:
            movement.down = pressed;
            break;
        case sf::Keyboard::Q:
            movement.left = pressed;
            break;
        case sf::Keyboard::D:
            movement.right = pressed;
            break;
        default:
            break;
        }
    }
}

void IngameState::processUpdate(entt::registry& reg, const double dt)
{
    ZoneScoped;

    m_tilemapSystem.processUpdate(reg, dt);

    ImGui::ShowDemoWindow();

    ImGui::Begin("IngameState");

    if (ImGui::TreeNode("Velocities")) {
        auto view = reg.view<Velocity>();
        for (const entt::entity e : view) {
            auto&& vel = reg.get<Velocity>(e);

            if (ImGui::TreeNode((void*)(intptr_t)e, "Velocity #%d", e)) {
                ImGui::DragFloat("dx", &vel.dx);
                ImGui::DragFloat("dy", &vel.dy);

                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }

    ImGui::End();

    // Movement
    auto viewMovement = reg.view<Movement, Velocity>();
    for (const entt::entity e : viewMovement) {
        auto&& mvt = reg.get<Movement>(e);
        auto&& vel = reg.get<Velocity>(e);

        // Apply side movements
        if (mvt.left)
            vel.dx = -4;
        else if (mvt.right)
            vel.dx = 4;
        else
            vel.dx = 0;

        if (mvt.up)
            vel.dy = -4;
        else if (mvt.down)
            vel.dy = 4;
        else
            vel.dy = 0;
    }

    // Physics
    auto viewPhysics = reg.view<Position, Velocity>();
    for (const entt::entity e : viewPhysics) {
        auto&& pos = reg.get<Position>(e);
        auto&& vel = reg.get<Velocity>(e);

        // Apply gravity
        vel.dy += 0 * 9.81 * dt;

        // Compute position
        pos.x += dt * vel.dx;
        pos.y += dt * vel.dy;
    }

    auto viewSprites = reg.view<sf::Sprite, Position>();
    for (const entt::entity e : viewSprites) {
        auto&& sprite = reg.get<sf::Sprite>(e);
        auto&& pos    = reg.get<Position>(e);

        // Apply position
        sprite.setPosition(pos.x, pos.y);
    }
}

void IngameState::processDraw(entt::registry& reg, sf::RenderTarget& target)
{
    ZoneScoped;

    target.clear(sf::Color(135, 206, 235));

    m_tilemapSystem.processDraw(reg, target);

    auto view = reg.view<sf::Sprite>();
    for (const entt::entity e : view) {
        target.draw(reg.get<sf::Sprite>(e));
    }
}

void IngameState::resizeToMap()
{
    sf::View view                = m_engine->getTarget()->getDefaultView();
    const unsigned int mapHeight = 18;

    view.reset(sf::FloatRect(0, 0, mapHeight * 16. / 9., mapHeight));

    m_engine->getTarget()->setView(view);
}
