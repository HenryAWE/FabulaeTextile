#pragma once

#include <cstddef>
#include <array>
#include <vector>
#include <fabtex/vector.hpp>


namespace fabtex
{
    template <
        typename T,
        std::size_t Channel,
        typename Allocator = std::allocator<std::array<T, Channel>>
    >
    class basic_bitmap; // Forward declaration

    template <typename Bitmap>
    class basic_bitmap_view
    {
    public:
        typedef Bitmap bitmap_type;
        typedef typename Bitmap::value_type value_type;
        typedef typename Bitmap::size_type size_type;
        typedef typename Bitmap::index_type index_type;
        typedef typename Bitmap::reference reference;
        typedef typename Bitmap::const_reference const_reference;
        typedef typename Bitmap::pointer pointer;
        typedef typename Bitmap::const_pointer const_pointer;

        basic_bitmap_view() = default;
        basic_bitmap_view(const basic_bitmap_view&) = default;
        basic_bitmap_view(bitmap_type& bm, index_type offset_, index_type size_)
        {
            assign(bm, offset_, size_);
        }

        basic_bitmap_view& operator=(const basic_bitmap_view&) = default;

        reference index(index_type idx)
        {
            return bitmap().index(locate(idx));
        }
        const_reference index(index_type idx) const
        {
            return bitmap().index(locate(idx));
        }
        reference operator[](index_type idx) { return index(idx); }
        const_reference operator[](index_type idx) const { return index(idx); }

        void assign(bitmap_type& bm, index_type offset_, index_type size_) noexcept
        {
            m_ptr = &bm;
            m_offset = offset_;
            m_size = size_;
        }

        void copy(basic_bitmap_view v)
        {
            index_type min_size{
                std::min(size()[0], v.size()[0]),
                std::min(size()[1], v.size()[1])
            };
            for(size_type i = 0; i < min_size[0]; ++i)
            {
                for(size_type j = 0; j < min_size[1]; ++j)
                {
                    index_type idx{ i, j };
                    if(!valid(idx) || !v.valid(idx))
                        continue;
                    index(idx) = v.index(idx);
                }
            }
        }

        void fill(const_reference val)
        {
            for(size_type i = 0; i < m_size[0]; ++i)
            {
                for(size_type j = 0; j < m_size[1]; ++j)
                {
                    index({ i, j }) = val;
                }
            }
        }

        constexpr index_type size() const noexcept { return m_size; }

        bool empty() const noexcept
        {
            return m_size[0] * m_size[1] == 0;
        }

        bool valid(index_type idx) const noexcept
        {
            if(idx[0] < m_size[0] && idx[1] < m_size[1])
                return bitmap().valid(idx);
            else
                return false;
        }

        void rect(index_type pos, index_type rect_size, const_reference val)
        {
            for(size_type i = 0; i < rect_size[0]; ++i)
            {
                for(size_type j = 0; j < rect_size[1]; ++j)
                {
                    auto idx = pos + index_type(i, j);
                    if (valid(idx))
                        index(idx) = val;
                }
            }
        }

        bitmap_type& bitmap() const noexcept { return *m_ptr; }

    private:
        index_type locate(index_type idx) const noexcept
        {
            return m_offset + idx;
        }

        bitmap_type* m_ptr = nullptr;
        index_type m_offset;
        index_type m_size;
    };

    template <
        typename T,
        std::size_t Channel,
        typename Allocator
    >
    class basic_bitmap
    {
    public:
        typedef std::array<T, Channel> value_type;
        typedef std::vector<value_type, Allocator> container_type;
        typedef std::size_t size_type;
        typedef vec<2, size_type> index_type;
        typedef typename container_type::reference reference;
        typedef typename container_type::const_reference const_reference;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef basic_bitmap_view<basic_bitmap> view_type;

        basic_bitmap() : m_size(T(0)) {}
        basic_bitmap(const basic_bitmap&) = default;
        basic_bitmap(basic_bitmap&&) = default;
        basic_bitmap(index_type new_size)
        {
            resize(new_size);
        }

        reference index(index_type idx)
        {
            return m_data[locate(idx)];
        }
        const_reference index(index_type idx) const
        {
            return m_data[locate(idx)];
        }
        reference operator[](index_type idx) { return index(idx); }
        const_reference operator[](index_type idx) const { return index(idx); }

        pointer data() noexcept { return &m_data.data()[0]; }
        const_pointer data() const noexcept { return &m_data.data()[0]; }

        void resize(index_type new_size)
        {
            m_data.resize(new_size[0] * new_size[1]);
            m_size = new_size;
        }
        void resize(index_type new_size, const_reference val)
        {
            size_type buflen = new_size[0] * new_size[1];
            m_data.reserve(buflen);
            m_size = new_size;
            m_data.clear();
            m_data.resize(buflen, val);
        }

        void fill(const_reference val)
        {
            for(auto& i : m_data)
                i = val;
        }

        constexpr index_type size() const noexcept { return m_size; }

        bool empty() const noexcept
        {
            return m_size[0] * m_size[1] == 0;
        }

        bool valid(index_type idx) const noexcept
        {
            return idx[0] < m_size[0] && idx[1] < m_size[1];
        }

        void rect(index_type pos, index_type rect_size, const_reference val)
        {
            as_view().rect(pos, rect_size, val);
        }

        view_type subview(index_type offset, index_type size_) noexcept
        {
            return view_type(*this, offset, size_);
        }
        view_type as_view() noexcept
        {
            return subview({ 0, 0 }, size());
        }
        operator view_type() noexcept
        {
            return as_view();
        }

        friend bool operator==(const basic_bitmap& lhs, const basic_bitmap& rhs) = default;

    private:
        index_type m_size;
        container_type m_data;

        size_type locate(index_type idx) const noexcept
        {
            return idx[1] * m_size[0] + idx[0];
        }
    };

    typedef basic_bitmap<std::uint8_t, 4> bitmap_rgba8888;
    typedef bitmap_rgba8888::view_type bitmap_rgba8888_view;
}
