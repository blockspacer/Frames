#ifndef TILEMAPSIMPLESYSTEM_HPP
#define TILEMAPSIMPLESYSTEM_HPP

#include <fengine/isystem.h>
#include "../components/tilemap.h"
#include "../components/tileset.h"

class TilemapSimpleSystem : public frames::ISystem {
 public:
  void init(frames::Engine* engine) override;
  void cleanup() override;

  void processUpdate(entt::registry& reg, const double dt) override;
  void processDraw(entt::registry& reg, sf::RenderTarget& target) override;

 private:
  /*!
   * \brief Loads the maps in the registry.
   * \note An entity with the components Tilemap and Tileset is considered a
   * map.
   * \param reg The registry.
   */
  void load(entt::registry& reg) const;

  bool loadTileset(Tileset& set) const;
  void loadMap(Tilemap& map, Tileset& set) const;

  std::vector<int32_t> loadLayerData(const std::string file);

  /*
  std::vector<int32_t> openMap(Tilemap& map, Tileset& properties);
  void decodeMap(Tilemap& map, Tileset& properties, std::vector<int32_t> data);
  */

 private:
  TextureCachePtr m_textureCache;
};

#endif // TILEMAPSIMPLESYSTEM_HPP
