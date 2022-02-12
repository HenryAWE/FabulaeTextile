#pragma once

#include <utility>
#include <vector>
#include <cmath>
#include <fabtex/anchor.hpp>
#include <fabtex/bitmap.hpp>


namespace fabtex
{
    class stroke_base
    {
    public:
        virtual ~stroke_base();
    };

    namespace detailed
    {
        template <typename Derived>
        class stroke_impl : public stroke_base
        {
        public:
            typedef vec<2, float> vector_type;

            stroke_impl() = default;
            stroke_impl(vec<2, float> size_)
                : m_size(size_) {}

            constexpr const vector_type& size() const noexcept { return m_size; }

            vector_type center() const noexcept
            {
                return m_size / 2.0f;
            }

        protected:
            vec<2, float> m_size;
        };
    }

    // 横（Héng） / Horizontal
    class stroke_h : public detailed::stroke_impl<stroke_h>
    {
    public:
        typedef stroke_impl::vector_type vector_type;
        typedef std::vector<anchor> anchor_container;

        stroke_h();

        const anchor_container& anchors() const { return m_anchors; }
        anchor_container::iterator anchor_begin() { return m_anchors.begin(); }
        anchor_container::iterator anchor_end() { return m_anchors.end(); }

        bitmap_rgba8888 generate_preview(vec<2, std::size_t> img_size) const;

    private:
        anchor_container m_anchors;

        void create_anchors();
    };
}
