#pragma once
/* ImGui widgets for visualizer of the Fabulae Textile */

#include <SDL.h>
#include <imgui.h>
#include <fabtex.hpp>


namespace ImFabtex
{
    ImVec2 GetImVec(fabtex::vec<2, float> v) noexcept;

    struct ShowStrokeData
    {
        fabtex::stroke_h stroke;
        ImTextureID texture;
    };

    inline void ShowStroke(
        const char* name,
        ShowStrokeData& data,
        ImVec2 size = ImVec2(100, 100)
    ) {
        auto& stroke = data.stroke;

        ImGui::Text("Stroke Name: %s", name);

        int flags = ImGuiWindowFlags_NoScrollbar;
        if(ImGui::BeginChild(name, ImVec2(200, 200), true, flags))
        {
            auto tex = static_cast<SDL_Texture*>(data.texture);
            int w = 0, h = 0;
            SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
            ImGui::Image(
                tex,
                ImVec2(static_cast<float>(w), static_cast<float>(h))
            );
        }
        ImGui::EndChild();

        ImGui::Text("Size (%f, %f)", stroke.size()[0], stroke.size()[1]);
        std::size_t anchor_id = 0;
        for(auto it = stroke.anchor_begin(); it != stroke.anchor_end(); ++it)
        {
            auto& pos = it->position();
            ImGui::Text(
                "Anchor-%d:\n",
                static_cast<int>(anchor_id)
            );
            ImGui::Text(
                "\tname \"%s\";"
                "\tposition (%f, %f);",
                it->name().c_str(),
                pos[0], pos[1]
            );
            auto& target = it->target();
            if(target)
                ImGui::Text("\ttarget \"%s\"", target->c_str());
            else
                ImGui::Text("\ttarget (null)");

            ++anchor_id;
        }
    }
}
