#include "tilemapeditor.hpp"

#include <imgui-SFML/imgui_SFML.h>
#include <imgui/imgui.h>

void TilemapEditor::init(frames::Engine* engine) {
  ZoneScoped;
  ISystem::init(engine);

  m_selectedTileset.name = "None";
}

void TilemapEditor::cleanup() {}

void TilemapEditor::processUpdate(entt::registry& reg, const double dt) {
  auto const& sets = reg.view<Tileset>();
  auto maps        = reg.view<Tilemap>();

  ImGui::Begin("Tilemap Editor");
  if (ImGui::TreeNode("Tileset")) {
    for (auto e : sets) {
      auto const& set = reg.get<Tileset>(e);

      auto const& setTexture = set.handle.get();
      auto const maxSize     = setTexture.getSize().x > setTexture.getSize().y
                               ? setTexture.getSize().x
                               : setTexture.getSize().y;
      double const scaling = maxSize / 150.;

      ImGui::TextUnformatted(set.name.data());
      ImGui::SameLine();
      if (ImGui::ImageButton(set.handle.get())) {
        m_selectedTileset = set;
      }
      /*if (ImGui::Selectable(set.name.data(),
                            set.path == m_selectedTileset.path)) {
        m_selectedTileset = set;
      }*/
    }

    ImGui::TreePop();
  }
  ImGui::End();
}

void TilemapEditor::processDraw(entt::registry& reg, sf::RenderTarget& target) {
}
