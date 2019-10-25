#ifndef TILEMAPEDITOR_HPP
#define TILEMAPEDITOR_HPP

#include "../components/tilemap.h"
#include "tilemapsystem.h"

class TilemapEditor : public frames::ISystem {
 public:
  void init(frames::Engine* engine) override;
  void cleanup() override;

  void processUpdate(entt::registry& reg, const double dt) override;
  void processDraw(entt::registry& reg, sf::RenderTarget& target) override;

 private:
 private:
  Tileset m_selectedTileset;
};

#endif // TILEMAPEDITOR_HPP
