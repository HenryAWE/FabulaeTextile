#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include "widgets.hpp"


namespace ImFabtex
{
    ImVec2 GetImVec(fabtex::vec<2, float> v) noexcept
    {
        return ImVec2(v[0], v[1]);
    }

    void AboutVisualizer(bool* p_open)
    {
        const int window_flags =
            ImGuiWindowFlags_NoSavedSettings;
        if(!ImGui::Begin("About Visualizer", p_open, window_flags))
            ImGui::End();

        auto fb_col = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);

        ImGui::TextColored(fb_col, "Fabulae Textile");
        ImGui::Text(
            "Information: %s\n"
            "GitHub Repository: %s",
            fabtex::get_library_info(),
            "https://github.com/HenryAWE/FabulaeTextile"
        );

        ImGui::Separator();
        ImGui::TextColored(fb_col, "Visualizer");
        ImGui::Text(
            "__cplusplus = %ld\n"
            "ImGui %s (%d)\n"
            "stb_image_write - %s",
            __cplusplus,
            IMGUI_VERSION, IMGUI_VERSION_NUM,
            "v1.16"
        );

        ImGui::End();
    }
}
