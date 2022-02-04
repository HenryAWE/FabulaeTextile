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
    class basic_bitmap
    {
    public:
        typedef std::array<T, Channel> value_type;
        typedef std::vector<value_type, Allocator> container_type;
        typedef std::size_t size_type;
        typedef vec<2, size_type> index_type;
        typedef typename container_type::reference reference;
        typedef typename container_type::const_reference const_reference;

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

    private:
        index_type m_size;
        container_type m_data;

        size_type locate(index_type idx) const noexcept
        {
            return idx[1] * m_size[0] + idx[0];
        }
    };

    typedef basic_bitmap<std::uint8_t, 4> bitmap_rgba8888;
}
