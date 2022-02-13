#include "utility.hpp"
#include <cassert>
#include <stb_image_write.h>
#include "visualizer.hpp"


namespace fabtextool
{
    bool update_texture(
        const fabtex::bitmap_rgba8888& bm,
        SDL_Texture* tex
    ) {
        assert(tex);
        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = static_cast<int>(bm.size()[0]);
        rect.h = static_cast<int>(bm.size()[1]);
        auto pitch = sizeof(fabtex::bitmap_rgba8888::value_type) * bm.size()[0];
        int result = SDL_UpdateTexture(
            tex,
            &rect,
            bm.data(),
            static_cast<int>(pitch)
        );

        return result == 0;
    }

    bool save_bitmap(
        const fabtex::bitmap_rgba8888& bm,
        const std::filesystem::path& file
    ) {
        if(file.extension() != ".bmp")
            return false;

        return stbi_write_bmp(
            reinterpret_cast<const char*>(file.u8string().c_str()),
            static_cast<int>(bm.size()[0]),
            static_cast<int>(bm.size()[1]),
            4,
            bm.data()
        );
    }
}
