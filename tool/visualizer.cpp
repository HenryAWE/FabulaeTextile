#include "visualizer.hpp"
#include <future>
#include <fstream>
#include <sstream>
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <imfilebrowser.h>


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
        rasterizer.line_size = 10;
    }
    void visualizer::quit()
    {
        if(m_tex)
        {
            SDL_DestroyTexture(m_tex);
            m_tex = nullptr;
        }
        m_win = nullptr;
        m_ren = nullptr;
    }

    void visualizer::update_imgui()
    {
        auto& io = ImGui::GetIO();

        if(m_show_bitmap_visualizer)
        {
            const int flags =
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_AlwaysAutoResize;
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 1, 1));
            if(ImGui::Begin("Bitamp Visualizer", &m_show_bitmap_visualizer, flags))
            {
                ImGui::Image(
                    m_tex,
                    m_tex_size
                );
            }
            ImGui::End();
            ImGui::PopStyleColor();
        }
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

    void visualizer::visualize_bitmap(const fabtex::bitmap_rgba8888& bm)
    {
        if(m_tex)
            SDL_DestroyTexture(m_tex);
        m_tex = SDL_CreateTexture(
            m_ren, SDL_PIXELFORMAT_ABGR8888,
            SDL_TEXTUREACCESS_STATIC,
            bm.size()[0], bm.size()[1]
        );
        update_texture(bm, m_tex);
        m_tex_size = ImFabtex::GetImVec(bm.size());

        m_show_bitmap_visualizer = true;
    }
}

// Will be replaced to SDL_main on Windows by the macro of SDL
int main(int argc, char* argv[])
{
    using namespace fabtextool;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        visualizer::report_error(SDL_GetError(), "SDL_Init() failed");
        return EXIT_FAILURE;
    }

    SDL_Window* win = SDL_CreateWindow(
        "Fabulae Textile Visualizer v0.1",
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
    ImGui::FileBrowser filebrowser_open;
    filebrowser_open.SetTitle("打开/Open");
    filebrowser_open.SetTypeFilters({ ".xml" });

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
                if(ImGui::MenuItem("打开文件/Open File"))
                {
                    filebrowser_open.Open();
                }
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

        if(appdata.show_about)
            ImFabtex::AboutVisualizer(&appdata.show_about);

        ImGui::SetNextWindowContentSize(ImVec2(400, 400));
        ImFabtex::ShowParsedData(app.parser);
        app.update_imgui();

        filebrowser_open.Display();
        if(filebrowser_open.HasSelected())
        {
            auto file = filebrowser_open.GetSelected();
            SDL_LogInfo(
                SDL_LOG_CATEGORY_APPLICATION,
                "Open file: %s",
                file.u8string().c_str()
            );
            filebrowser_open.ClearSelected();
            filebrowser_open.Close();

            try
            {
                std::ifstream ifs(file);
                std::stringstream ss;
                ss << ifs.rdbuf();
                app.parser.parse(ss.str());
            }
            catch(const std::exception& e)
            {
                visualizer::report_error(
                    e.what(),
                    (std::string("Parsing file \"") + file.string() + "\" failed").c_str()
                );
            }
        }

        ImGui::Render();

        SDL_RenderClear(ren);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(ren);
    }

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
