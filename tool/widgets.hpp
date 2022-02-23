#pragma once
/* ImGui widgets for visualizer of the Fabulae Textile */

#include <SDL.h>
#include <imgui.h>
#include <fabtex.hpp>
#include <fabtex/data.hpp>


namespace ImFabtex
{
    ImVec2 GetImVec(fabtex::vec<2, float> v) noexcept;

    struct AppData
    {
        bool show_about = false;
    };

    void AboutVisualizer(bool* p_open = nullptr);

    void ShowParsedData(
        const fabtex::xml_parser& parsed,
        bool* p_open = nullptr
    );
}
