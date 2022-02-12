#include "visualizer.hpp"
#include <future>
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>


namespace fabtextool
{
    visualizer& visualizer::instance() noexcept
    {
        static visualizer ins;
        return ins;
    }

    void visualizer::init(SDL_Window* win, SDL_Renderer* ren)
    {
        m_win = win;
        m_ren = ren;
    }
    void visualizer::quit()
    {
        m_win = nullptr;
        m_ren = nullptr;
    }

    void visualizer::report_error(
        const char* msg,
        const char* title
    ) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "%s: %s",
            title, msg
        );
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            title,
            msg,
            instance().window()
        );
    }
}

int SDL_main(int argc, char* argv[])
{
    using namespace fabtextool;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        visualizer::report_error(SDL_GetError(), "SDL_Init() failed");
        return EXIT_FAILURE;
    }

    SDL_Window* win = SDL_CreateWindow(
        "Visualizer",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
    );
    if(!win)
    {
        visualizer::report_error(SDL_GetError(), "SDL_CreateWindow() failed");
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(
        win,
        -1,
        SDL_RENDERER_PRESENTVSYNC
    );
    if(!ren)
    {
        visualizer::report_error(SDL_GetError(), "SDL_CreateRenderer() failed");
        SDL_DestroyWindow(win);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_RendererInfo ren_info;
    SDL_GetRendererInfo(ren, &ren_info);
    SDL_RenderSetVSync(ren, 1);

    ImGuiContext* imctx = ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(
        "SourceHanSansHWSC-Regular.otf", 24.0f,
        nullptr,
        io.Fonts->GetGlyphRangesChineseSimplifiedCommon()
    );
    auto font_ready = std::async(std::launch::async, [&io]{ return io.Fonts->Build(); });
    ImGui_ImplSDL2_InitForSDLRenderer(win, ren);
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

    auto& app = visualizer::instance();
    app.init(win, ren);

    ImFabtex::AppData appdata;

    ImFabtex::ShowStrokeData data;
    data.stroke;
    data.texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, 100, 100);
    update_texture(data.stroke.generate_preview({ 100, 100 }), (SDL_Texture*)data.texture);

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

            ImGui::Separator();
            if(ImGui::BeginMenu("文件/File"))
            {
                if(ImGui::MenuItem("退出/Quit"))
                {
                    SDL_QuitEvent event{};
                    event.type = SDL_QUIT;
                    event.timestamp = SDL_GetTicks();
                    SDL_PushEvent(reinterpret_cast<SDL_Event*>(&event));
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("帮助/Help"))
            {
                if(ImGui::MenuItem("关于/About"))
                {
                    appdata.show_about = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ShowStroke("横", data);
        if(ImGui::Button("Save Bitmap"))
        {
            save_bitmap(data.stroke.generate_preview({ 100, 100 }), "bitmap.bmp");
        }

        if(appdata.show_about)
            ImFabtex::AboutVisualizer(&appdata.show_about);

        ImGui::Render();

        SDL_RenderClear(ren);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(ren);
    }

    SDL_DestroyTexture((SDL_Texture*)data.texture);
    data.texture = nullptr;
    app.quit();

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
