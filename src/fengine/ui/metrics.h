#ifndef METRICS_H
#define METRICS_H

#include <cstdint>

#include "../utils/timing.h"
#include "graph.h"

namespace frames {
namespace ui {
    template <int count>
    class Metric : public Graph<count> {
    public:
        Metric(const std::string& name)
            : Graph<count>()
            , m_name(name)
        {
        }

        void update(const timing::Clock::duration& time)
        {
            const double timems = std::chrono::duration_cast<timing::dmilli>(time).count();

            // Append the new value to the end
            std::rotate(m_data.begin(), m_data.begin() + 1, m_data.end());
            m_data[count - 1] = timems;

            constexpr double alpha = 0.95;

            m_avg = alpha * m_avg + (1. - alpha) * timems;
        }

        void render() const
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            auto padding = ImGui::GetStyle().WindowPadding;
            ImGui::SetNextWindowBgAlpha(0.f);
            ImGui::SetNextWindowPos(ImVec2(15 - padding.x, ImGui::GetIO().DisplaySize.y - padding.y - 75 - 15));
            ImGui::Begin("frametime_plot", nullptr,
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
            ImGui::PlotLinesStats(m_name, m_data, 0, 40, m_avg);
            ImGui::End();

            ImGui::PopStyleVar();
        }

    private:
        std::array<double, count> m_data;
        std::string m_name;
        double m_avg = 0;
    };

} // namespace ui
} // namespace frames

#endif // METRICS_H
