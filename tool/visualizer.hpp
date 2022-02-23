#pragma once

#include <SDL.h>
#include <fabtex.hpp>
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

        constexpr SDL_Window* window() const noexcept { return m_win; }
        constexpr SDL_Renderer* renderer() const noexcept { return m_ren; }

        static void report_error(
            const char* msg,
            const char* title = ""
        );

        fabtex::xml_parser parser;

    private:
        SDL_Window* m_win = nullptr;
        SDL_Renderer* m_ren = nullptr;
    };
}
