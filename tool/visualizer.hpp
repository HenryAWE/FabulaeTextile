#pragma once

#include <SDL.h>
#include <fabtex.hpp>
#include <fabtex/renderer.hpp>
#include <fabtex/data.hpp>
#include "widgets.hpp"
#include "utility.hpp"


namespace fabtextool
{
    class visualizer
    {
        visualizer() = default;
    public:
        static visualizer& instance() noexcept;

        void init(SDL_Window* win, SDL_Renderer* ren);
        void quit();

        void update_imgui();

        constexpr SDL_Window* window() const noexcept { return m_win; }
        constexpr SDL_Renderer* renderer() const noexcept { return m_ren; }

        static void report_error(
            const char* msg,
            const char* title = ""
        );

        fabtex::xml_parser parser;
        fabtex::simple_raster_renderer rasterizer;

        void visualize_bitmap(const fabtex::bitmap_rgba8888& bm);

    private:
        SDL_Window* m_win = nullptr;
        SDL_Renderer* m_ren = nullptr;
        SDL_Texture* m_tex = nullptr;
        ImVec2 m_tex_size;
        bool m_show_bitmap_visualizer = false;
    };
}
