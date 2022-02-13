#pragma once

#include <cstddef>
#include <type_traits>


namespace fabtex
{
    template <std::size_t N, typename T>
    class vec;

    template <typename T>
    class vec<2, T>
    {
    public:
        typedef T value_type;
        typedef std::size_t size_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef pointer iterator;
        typedef const_pointer const_iterator;

        vec() : vec(static_cast<value_type>(0)) {}
        vec(const vec&) = default;
        vec(vec&&) = default;
        template <typename U>
        vec(const vec<2, U>& v) : vec(value_type(v[0]), value_type(v[1])) {}
        explicit vec(const_reference v) : m_data{ v, v } {}
        vec(const_reference v0, const_reference v1)
            : m_data{ v0, v1 } {}

        vec& operator=(const vec&) = default;
        vec& operator=(vec&&) = default;

        static constexpr size_type dimension = 2;
        static constexpr size_type size() noexcept { return dimension; }

        reference operator[](size_type idx) noexcept
        {
            return m_data[idx];
        }
        const_reference operator[](size_type idx) const noexcept
        {
            return m_data[idx];
        }

        friend vec operator+(const vec& lhs, const vec& rhs)
        {
            return vec(
                lhs[0] + rhs[0],
                lhs[1] + rhs[1]
            );
        }
        friend vec operator+(const vec& lhs, const_reference rhs) { return lhs + vec(rhs); }
        friend vec operator+(const_reference lhs, const vec& rhs) { return vec(lhs) + rhs; }

        friend vec operator-(const vec& lhs, const vec& rhs)
        {
            return vec(
                lhs[0] - rhs[0],
                lhs[1] - rhs[1]
            );
        }
        friend vec operator-(const vec& lhs, const_reference rhs) { return lhs - vec(rhs); }
        friend vec operator-(const_reference lhs, const vec& rhs) { return vec(lhs) - rhs; }

        friend vec operator*(const vec& lhs, const_reference rhs)
        {
            return vec(
                lhs[0] * rhs,
                lhs[1] * rhs
            );
        }
        friend vec operator*(const_reference lhs, const vec& rhs)
        {
            return vec(
                lhs * rhs[0],
                lhs * rhs[1]
            );
        }

        friend vec operator/(const vec& lhs, const_reference rhs)
        {
            return vec(
                lhs[0] / rhs,
                lhs[1] / rhs
            );
        }

        vec operator-() const
        {
            return vec(
                -m_data[0],
                -m_data[1]
            );
        }

        bool operator==(const vec& rhs) const = default;

        constexpr pointer data() noexcept { return m_data; }
        constexpr const_pointer data() const noexcept { return m_data; }

        iterator begin() noexcept { return data(); }
        iterator end() noexcept { return data() + dimension; }
        const_iterator begin() const noexcept { return data(); }
        const_iterator end() const noexcept { return data() + dimension; }
        const_iterator cbegin() const noexcept { return data(); }
        const_iterator cend() const noexcept { return data() + dimension; }

    private:
        T m_data[dimension];
    };
}
