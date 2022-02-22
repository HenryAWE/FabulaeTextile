#pragma once
/* ImGui widgets for visualizer of the Fabulae Textile */

#include <SDL.h>
#include <imgui.h>
#include <fabtex.hpp>


namespace ImFabtex
{
    ImVec2 GetImVec(fabtex::vec<2, float> v) noexcept;

    struct AppData
    {
        bool show_about = false;
    };

    void AboutVisualizer(bool* p_open = nullptr);
}
