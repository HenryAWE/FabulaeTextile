#pragma once

#include <string_view>
#include <fabtex/bitmap.hpp>
#include <fabtex/character.hpp>


namespace fabtex
{
    // 用于渲染简易预览的光栅化渲染器
    // Raster renderer for rendering simple preview
    class simple_raster_renderer
    {
    public:
        typedef bitmap_rgba8888_view::size_type size_type;
        typedef bitmap_rgba8888_view::index_type index_type;

        void render_stroke(bitmap_rgba8888_view v, const stroke& s, index_type off = index_type(0));
        void render_radical(bitmap_rgba8888_view v, const radical&);

        size_type line_size = 3;

    private:
        void render_heng(bitmap_rgba8888_view v, index_type off);
        void render_shu(bitmap_rgba8888_view v, index_type off);
        index_type heng_locate_pos(index_type size, std::string_view id);
        index_type shu_locate_pos(index_type size, std::string_view id);
        index_type locate_pos(stroke_type type, index_type size, std::string_view id);
    };
}
