#include <fabtex/stroke.hpp>
#include <iterator>


namespace fabtex
{
    stroke_base::~stroke_base() = default;

    stroke_h::stroke_h()
        : stroke_impl({80.0f, 4.0f})
    {
        create_anchors();
    }

    void stroke_h::create_anchors()
    {
        auto inserter = std::back_inserter(m_anchors);
        *inserter++ = anchor("center", center());
    }

    bitmap_rgba8888 stroke_h::generate_preview(vec<2, std::size_t> img_size) const
    {
        bitmap_rgba8888 img;
        img.resize(img_size, {255, 255, 255, 255});
        auto pos = vector_type(img_size) / 2.0f - size() / 2.0f;
        img.rect(pos, size(), { 0, 0, 0, 255 });

        return std::move(img);
    }
}
