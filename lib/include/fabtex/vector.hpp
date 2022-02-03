#pragma once

#include <cstddef>
#include <type_traits>


namespace fabtex
{
    namespace detailed
    {
        template <std::size_t N, typename T>
        class vec_impl
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

            vec_impl(const vec_impl&) = default;
            vec_impl(vec_impl&&) = default;
            template <typename... U>
            vec_impl(U&&... args)
                : m_data{ args... } { static_assert(sizeof...(args) == N, "sizeof...(args) == N failed"); };

            reference operator[](size_type idx) noexcept
            {
                return m_data[idx];
            }
            const_reference operator[](size_type idx) const noexcept
            {
                return m_data[idx];
            }

            static constexpr size_type size() noexcept { return N; }

            friend bool operator==(const vec_impl& lhs, const vec_impl& rhs)
            {
                bool result = true;
                for(size_type i = 0; i < size(); ++i)
                    result &= lhs[i] == rhs[i];
                return result;
            }
            friend bool operator!=(const vec_impl& lhs, const vec_impl& rhs)
            {
                return !(lhs == rhs);
            }

            constexpr pointer data() noexcept { return m_data; }
            constexpr const_pointer data() const noexcept { return m_data; }

            iterator begin() noexcept { return data(); }
            iterator end() noexcept { return data() + N; }
            const_iterator begin() const noexcept { return data(); }
            const_iterator end() const noexcept { return data() + N; }
            const_iterator cbegin() const noexcept { return data(); }
            const_iterator cend() const noexcept { return data() + N; }

            vec_impl& operator=(const vec_impl&) = default;

        protected:
            T m_data[N];
        };
    }

    template <std::size_t N, typename T>
    class vec;

    template <typename T>
    class vec<2, T> : public detailed::vec_impl<2, T>
    {
    public:
        typedef typename vec_impl::value_type value_type;
        typedef typename vec_impl::size_type size_type;
        typedef typename vec_impl::reference reference;
        typedef typename vec_impl::const_reference const_reference;
        typedef typename vec_impl::pointer pointer;
        typedef typename vec_impl::const_pointer const_pointer;
        typedef typename vec_impl::iterator iterator;
        typedef typename vec_impl::const_iterator const_iterator;

        vec() : vec(static_cast<value_type>(0)) {}
        explicit vec(const_reference v) : vec_impl(v, v) {}
        vec(const_reference v0, const_reference v1)
            : vec_impl({ v0, v1 }) {}

        friend vec operator+(const vec& lhs, const vec& rhs)
        {
            return vec(
                lhs[0] + rhs[0],
                lhs[1] + rhs[1]
            );
        }
        friend vec operator+(const vec& lhs, const_reference rhs) { return lhs + vec(rhs); }
        friend vec operator+(const_reference lhs, const vec& rhs) { return vec(lhs) + rhs; }

        friend vec operator-(const vec& lhs, vec& rhs)
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
    };
    template <typename T>
    class vec<3, T> : public detailed::vec_impl<3, T>
    {
    public:
        typedef typename vec_impl::value_type value_type;
        typedef typename vec_impl::size_type size_type;
        typedef typename vec_impl::reference reference;
        typedef typename vec_impl::const_reference const_reference;
        typedef typename vec_impl::pointer pointer;
        typedef typename vec_impl::const_pointer const_pointer;
        typedef typename vec_impl::iterator iterator;
        typedef typename vec_impl::const_iterator const_iterator;

        vec() : vec(static_cast<value_type>(0)) {}
        explicit vec(const_reference v) : vec_impl(v, v, v) {}
        vec(const_reference v0, const_reference v1, const_reference v2)
            : vec_impl({ v0, v1, v2 }) {}

        friend vec operator+(const vec& lhs, const vec& rhs)
        {
            return vec(
                lhs[0] + rhs[0],
                lhs[1] + rhs[1],
                lhs[2] + rhs[2]
            );
        }
        friend vec operator+(const vec& lhs, const_reference rhs) { return lhs + vec(rhs); }
        friend vec operator+(const_reference lhs, const vec& rhs) { return vec(lhs) + rhs; }

        friend vec operator-(const vec& lhs, vec& rhs)
        {
            return vec(
                lhs[0] - rhs[0],
                lhs[1] - rhs[1],
                lhs[2] - rhs[2]
            );
        }
        friend vec operator-(const vec& lhs, const_reference rhs) { return lhs - vec(rhs); }
        friend vec operator-(const_reference lhs, const vec& rhs) { return vec(lhs) - rhs; }

        friend vec operator*(const vec& lhs, const_reference rhs)
        {
            return vec(
                lhs[0] * rhs,
                lhs[1] * rhs,
                lhs[2] * rhs
            );
        }
        friend vec operator*(const_reference lhs, const vec& rhs)
        {
            return vec(
                lhs * rhs[0],
                lhs * rhs[1],
                lhs * rhs[2]
            );
        }

        friend vec operator/(const vec& lhs, const_reference rhs)
        {
            return vec(
                lhs[0] / rhs,
                lhs[1] / rhs,
                lhs[2] / rhs
            );
        }

        vec operator-() const
        {
            return vec(
                -m_data[0],
                -m_data[1],
                -m_data[2]
            );
        }
    };
}
