#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include "widgets.hpp"
#include "visualizer.hpp"


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

    namespace detailed
    {
        const char* get_stroke_type_str(fabtex::stroke_type type)
        {
            using fabtex::stroke_type;
            switch(type)
            {
            case stroke_type::heng:
                return "heng";
            case stroke_type::shu:
                return "shu";
            default:
                return "(unknown)";
            }
        }
    }

    void ShowParsedData(
        const fabtex::xml_parser& parsed,
        bool* p_open
    ) {
        const int window_flags =
            ImGuiWindowFlags_NoSavedSettings;
        if(!ImGui::Begin("Parsed Data", p_open, window_flags))
            ImGui::End();

        const fabtex::bitmap_rgba8888::index_type bitmap_size{ 512 };

        ImGui::Text("笔画/Radical(s)");
        for(std::size_t i = 0; i < parsed.radicals().size(); ++i)
        {
            ImGui::PushID(i);
            auto strokes = parsed.radicals()[i].strokes;
            ImGui::Text(
                "Index-%d; Name \"%s\"; Stroke count=%d",
                static_cast<int>(i),
                parsed.radicals()[i].name.c_str(),
                static_cast<int>(strokes.size())
            );
            ImGui::SameLine();
            if(ImGui::Button("Visualize"))
            {
                auto& app = fabtextool::visualizer::instance();
                fabtex::bitmap_rgba8888 img;
                img.resize(bitmap_size, { 255, 255, 255, 255 });
                app.rasterizer.render_radical(img, parsed.radicals()[i]);
                app.visualize_bitmap(img);
            }
            if(ImGui::TreeNode("Strokes"))
            {
                for(std::size_t j = 0; j < strokes.size(); ++j)
                {
                    auto type = strokes[j].type();
                    bool anchors_expanded = false;
                    if(ImGui::TreeNode(
                        (const void*)j,
                        "Stroke-%d: type=%s(%d)",
                        static_cast<int>(j),
                        detailed::get_stroke_type_str(type),
                        type
                    )) {
                        anchors_expanded = true;
                        for(const auto& [src, dst] : strokes[j].connections)
                        {
                            ImGui::BulletText(
                                "Anchor: from \"%s\" to \"%s\"",
                                src.string().c_str(),
                                dst.string().c_str()
                            );
                        }
                        ImGui::TreePop();
                    }
                    if(!anchors_expanded)
                        ImGui::SameLine();
                    ImGui::PushID(j);
                    if(ImGui::Button("Visualize"))
                    {
                        auto& app = fabtextool::visualizer::instance();
                        fabtex::bitmap_rgba8888 img;
                        img.resize(bitmap_size, { 255, 255, 255, 255 });
                        app.rasterizer.render_stroke(img, strokes[j]);
                        app.visualize_bitmap(img);
                    }
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::Separator();

        ImGui::End();
    }
}
