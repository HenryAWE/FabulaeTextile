#include <future>
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <fabtex.hpp>


int SDL_main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "Visualizer",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
    );

    SDL_Renderer* ren = SDL_CreateRenderer(
        win,
        -1,
        SDL_RENDERER_PRESENTVSYNC
    );
    SDL_RendererInfo ren_info;
    SDL_GetRendererInfo(ren, &ren_info);
    SDL_RenderSetVSync(ren, 1);

    ImGuiContext* imctx = ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(
        "SourceHanSansHWSC-Regular.otf", 24.0f,
        nullptr,
        io.Fonts->GetGlyphRangesChineseFull()
    );
    auto font_ready = std::async(std::launch::async, [&io]{ return io.Fonts->Build(); });
    ImGui_ImplSDL2_InitForSDLRenderer(win);
    ImGui_ImplSDLRenderer_Init(ren);
    font_ready.get();
    ImGui_ImplSDLRenderer_CreateDeviceObjects();

    SDL_LogInfo(
        SDL_LOG_CATEGORY_APPLICATION,
        "Renderer Name: %s\n"
        "Fabulae Textile Information: %s",
        ren_info.name,
        fabtex::get_library_info()
    );

    bool quit = false;
    while(!quit)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;

            default:
                ImGui_ImplSDL2_ProcessEvent(&e);
                break;
            }
        }

        ImGui_ImplSDL2_NewFrame(win);
        ImGui::NewFrame();

        if(ImGui::BeginMainMenuBar())
        {
            ImGui::TextColored(
                ImVec4(0.0f, 1.0f, 1.0f, 1.0f),
                "Fabulae Textile"
            );
            ImGui::EndMainMenuBar();
        }

        ImGui::Render();

        SDL_RenderClear(ren);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(ren);
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(imctx);
    imctx = nullptr;

    SDL_DestroyRenderer(ren);
    ren = nullptr;
    SDL_DestroyWindow(win);
    win = nullptr;

    SDL_Quit();

    return 0;
}
