#ifndef FRAMES_GRAPH_H
#define FRAMES_GRAPH_H

#include <imgui/imgui.h>

#include <array>
#include <functional>
#include <vector>

#include <Tracy.hpp>

namespace ImGui {
template <std::size_t n>
void PlotLinesStats(const std::string& label,
                    const std::array<double, n>& values,
                    double scale_min = std::numeric_limits<double>::min(),
                    double scale_max = std::numeric_limits<double>::max(),
                    double avg       = std::numeric_limits<double>::max());
}

namespace frames {
namespace ui {

    template <std::size_t n>
    class Graph {
    public:
        Graph(const std::string& name)
            : m_name(name)
        {
            // Initialize data
            m_data.fill(0);
        }

        Graph(std::array<double, n>* data)
            : m_data(data)
        {
        }

        void push(double value)
        {
            ZoneScoped;
            // Offset all the previous data by 1
            std::rotate(m_data.begin(), m_data.begin() + 1, m_data.end());
            // Append the new value to the end
            m_data[n - 1] = value;
        }

        void render() const
        {
            ZoneScoped;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(.2f, .2f, .2f, 1.f));

            ImGui::SetNextWindowPos(ImVec2(15, 15));
            //ImGui::SetNextWindowPos(ImVec2(15 - padding.x, ImGui::GetIO().DisplaySize.y - padding.y - 75 - 15));
            ImGui::Begin("frametime_plot", nullptr,
                         ImGuiWindowFlags_NoTitleBar
                             | ImGuiWindowFlags_NoResize
                             | ImGuiWindowFlags_NoScrollbar
                             | ImGuiWindowFlags_NoInputs
                             | ImGuiWindowFlags_NoSavedSettings
                             | ImGuiWindowFlags_NoFocusOnAppearing
                             | ImGuiWindowFlags_NoBringToFrontOnFocus);
            ImGui::PlotLinesStats<200>(this->m_name, this->m_data,
                                       0, 40, this->m_avg);
            ImGui::End();

            ImGui::PopStyleColor();
            ImGui::PopStyleVar(4);
        }

    protected:
        std::array<double, n> m_data;
        std::string m_name;

        double m_avg = 0;
    };

} // namespace ui
} // namespace frames

#include "graph.tpp"

#endif // FRAMES_GRAPH_H
