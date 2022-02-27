#include <fabtex/renderer.hpp>
#include <stdexcept>


namespace fabtex
{
    void simple_raster_renderer::render_stroke(bitmap_rgba8888_view v, const stroke& s, index_type off)
    {
        switch(s.type())
        {
        case stroke_type::heng:
            render_heng(v, off);
            break;
        case stroke_type::shu:
            render_shu(v, off);
            break;
        default:
            throw std::invalid_argument("unknown stroke type");
        }
    }
    void simple_raster_renderer::render_radical(bitmap_rgba8888_view v, const radical& r)
    {
        std::vector<index_type> positions;
        positions.resize(r.strokes.size());
        for(std::size_t i = 0; i < r.strokes.size(); ++i)
        {
            auto& s = r.strokes[i];
            index_type pos;
            if(!s.connections.empty())
            {
                auto& [src, dst] = *s.connections.begin();
                if(dst.front() != "..")
                    continue;

                auto p = dst;
                p.pop_front(); // remove ".."
                int idx = std::atoi(p.front().string().c_str());
                if(idx >= i)
                    throw std::out_of_range("index out of range");
                pos = positions[idx] + locate_pos(r.strokes[idx].type(), v.size(), p.back().string());
                pos = pos - locate_pos(s.type(), v.size(), src.back().string());

                positions[i] = pos;
            }
            render_stroke(v, r.strokes[i], pos);
        }
    }

    void simple_raster_renderer::render_heng(bitmap_rgba8888_view v, index_type off)
    {
        vec<2, std::size_t> size{
            v.size()[0],
            line_size
        };
        vec<2, std::size_t> pos = v.size() / 2 - size / 2;
        v.rect(pos + off, size, { 0, 0, 0, 255 });
    }

    void simple_raster_renderer::render_shu(bitmap_rgba8888_view v, index_type off)
    {
        vec<2, std::size_t> size{
            line_size,
            v.size()[0]
        };
        vec<2, std::size_t> pos = v.size() / 2 - size / 2;
        v.rect(pos + off, size, { 0, 0, 0, 255 });
    }

    simple_raster_renderer::index_type simple_raster_renderer::heng_locate_pos(vec<2, std::size_t> size, std::string_view id)
    {
        index_type result{ 0, 0 };
        switch(id[0])
        {
        case 'b':
            result[1] = size[1] / 2;
            switch(id[1])
            {
            case '0':
                result[0] = 0;
                break;
            case '1':
                result[0] = size[0] / 2;
                break;
            case '2':
                result[0] = size[0];
                break;
            default:
                throw std::invalid_argument("invalid location");
            }
            break;
        default:
            throw std::invalid_argument("invalid id: " + std::string(id));
        }

        return result;
    }
    simple_raster_renderer::index_type simple_raster_renderer::shu_locate_pos(index_type size, std::string_view id)
    {
        index_type result{ 0, 0 };
        switch(id[0])
        {
        case 'b':
            result[0] = size[0] / 2;
            switch(id[1])
            {
            case '0':
                result[1] = 0;
                break;
            case '1':
                result[1] = size[1] / 2;
                break;
            case '2':
                result[1] = size[1];
                break;
            default:
                throw std::invalid_argument("invalid location");
            }
            break;
        default:
            throw std::invalid_argument("invalid id: " + std::string(id));
        }

        return result;
    }
    simple_raster_renderer::index_type simple_raster_renderer::locate_pos(stroke_type type, index_type size, std::string_view id)
    {
        switch(type)
        {
        case stroke_type::heng:
            return heng_locate_pos(size, id);
        case stroke_type::shu:
            return shu_locate_pos(size, id);
        default:
            throw std::invalid_argument("invalid stroke type");
        }
    }
}
