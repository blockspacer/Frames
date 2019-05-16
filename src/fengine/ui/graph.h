#ifndef FRAMES_GRAPH_H
#define FRAMES_GRAPH_H

#include <imgui/imgui.h>

#include <array>
#include <functional>
#include <vector>

namespace ImGui {
template <std::size_t n>
void PlotLinesStats(const std::string& label,
                    const std::array<double, n>& values,
                    double scale_min = std::numeric_limits<double>::min(),
                    double scale_max = std::numeric_limits<double>::max(),
                    double avg       = std::numeric_limits<double>::max(),
                    bool displayMax  = false,
                    bool displayMin  = false);
}

namespace frames {
namespace ui {

    template <std::size_t n>
    class Graph {
    public:
        Graph()
        {
            // Initialize data
            m_data = new std::array<float, n>;
            for (unsigned int i = 0; i < n; ++i) {
                m_data->at(i) = 0;
            }
        }

        Graph(std::array<float, n>* data)
            : m_data(data)
        {
        }

        void push(float value)
        {
            // Offset all the data by one
            for (int i = 0; i < n - 1; i++) {
                m_data[i] = m_data[i + 1];
            }
            // Put the new value at the beginning
            m_data[n - 1] = value;
        }

        void setOverlayText(std::string text)
        {
            m_overlayText = text;
        }

        void plot()
        {
            ImGui::PlotLines(m_name.c_str(), m_data->data(), n, 0, m_overlayText.c_str(), m_min, m_max);
        }

    protected:
        std::array<float, n>* m_data = nullptr;
        std::string m_name, m_overlayText;

        float m_max = 0, m_min = 0;
    };

} // namespace ui
} // namespace frames

#include "graph.tpp"

#endif // FRAMES_GRAPH_H
