#pragma once

#include <filesystem>
#include <SDL.h>
#include <imgui.h>
#include <fabtex.hpp>


namespace fabtextool
{
    bool update_texture(
        const fabtex::bitmap_rgba8888& bm,
        SDL_Texture* tex
    );

    bool save_bitmap(
        const fabtex::bitmap_rgba8888& bm,
        const std::filesystem::path& file
    );
}
