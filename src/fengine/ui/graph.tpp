#include "graph.h"

#include <imgui/imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

#include "../utils/string.h"

namespace ImGui {

template <std::size_t n>
void PlotLinesStats(const std::string& label,
                    const std::array<double, n>& values,
                    double scale_min, double scale_max, double avg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) {
        return;
    }

    ImVec2 graph_size(225, 75);
    int values_count = n;

    ImGuiContext& g         = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + graph_size);
    const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);

    ItemSize(inner_bb, style.FramePadding.y);
    if (!ItemAdd(inner_bb, 0, &frame_bb)) {
        return;
    }
    const bool hovered = ItemHoverable(inner_bb, 0);

    // Determine scale from values if not specified
    if (scale_min == std::numeric_limits<double>::min() || scale_max == std::numeric_limits<double>::max()) {
        double v_min = std::numeric_limits<double>::max();
        double v_max = std::numeric_limits<double>::min();
        for (int i = 0; i < values_count; i++) {
            const double v = values.at(i);
            v_min          = ImMin(v_min, v);
            v_max          = ImMax(v_max, v);
        }
        if (scale_min == std::numeric_limits<double>::min()) {
            scale_min = v_min;
        }
        if (scale_max == std::numeric_limits<double>::max()) {
            scale_max = v_max;
        }
    }

    RenderFrame(frame_bb.Min, frame_bb.Max, ColorConvertFloat4ToU32(ImVec4(0.1f, 0.1f, 0.1f, 0.5f)),
                true,
                style.FrameRounding);

    if (values_count > 0) {
        int res_w      = ImMin((int)graph_size.x, values_count) - 1;
        int item_count = values_count - 1;

        const double t_step    = 1.0f / (double)res_w;
        const double inv_scale = (scale_min == scale_max) ? 0.0f : (1.0f / (scale_max - scale_min));

        double v0  = values.at(0);
        double t0  = 0.0f;
        ImVec2 tp0 = ImVec2(t0, 1.0f - ImSaturate((v0 - scale_min) * inv_scale)); // Point in the normalized space of our target rectangle

        const ImU32 col_base = ColorConvertFloat4ToU32(ImVec4(.3f, 1.f, .3f, 1.f));

        for (int k = 0; k < res_w; k++) {
            const double t1  = t0 + t_step;
            const int v1_idx = (int)(t0 * item_count + 0.5f);
            IM_ASSERT(v1_idx >= 0 && v1_idx < values_count);
            const double v1  = values.at((v1_idx + 1) % values_count);
            const ImVec2 tp1 = ImVec2(t1, 1.0f - ImSaturate((v1 - scale_min) * inv_scale));

            // NB: Draw calls are merged together by the DrawList system. Still, we should render our batch are lower level to save a bit of CPU.
            ImVec2 pos0 = ImLerp(inner_bb.Min, inner_bb.Max, tp0);
            ImVec2 pos1 = ImLerp(inner_bb.Min, inner_bb.Max, tp1);
            window->DrawList->AddLine(pos0, pos1, col_base);

            t0  = t1;
            tp0 = tp1;
        }
    }

    // Text overlay
    // Label
    if (!label.empty()) {
        RenderTextClipped(frame_bb.Min, frame_bb.Max, label.c_str(),
                          nullptr, nullptr, ImVec2(0.f, 0.f));
    }

    // Average
    if (avg != std::numeric_limits<double>::max()) {
        std::string text = "avg: " + frames::to_string_wp(avg, 2);
        RenderTextClipped(frame_bb.Min, frame_bb.Max, text.c_str(),
                          nullptr, nullptr, ImVec2(0.f, 1.f / 3.f));
    }
}

}
