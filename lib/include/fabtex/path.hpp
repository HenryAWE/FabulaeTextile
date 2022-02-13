#pragma once

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>


namespace fabtex
{
    template <typename T, T Separator>
    class basic_path
    {
    public:
        typedef T value_type;
        typedef typename std::basic_string<value_type> string_type;
        typedef typename std::basic_string_view<value_type> string_view_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;

    private:
        typedef std::pair<
            typename string_view_type::const_iterator,
            typename string_view_type::const_iterator
        > walk_result;

    public:
        basic_path() = default;
        basic_path(const basic_path&) = default;
        basic_path(basic_path&&) = default;
        basic_path(string_view_type view)
        {
            assign(view);
        }

        static constexpr value_type separator = Separator;

        basic_path& operator=(const basic_path&) = default;
        basic_path& operator=(basic_path&&) = default;
        basic_path& operator=(string_view_type view)
        {
            assign(view);
            return *this;
        }

        void assign(string_view_type view)
        {
            m_data.clear();
            append(view);
        }
        void assign(const basic_path& p)
        {
            *this = p;
        }

        void append(string_view_type view)
        {
            auto result = walk_path({ view.begin(), view.end() });
            while(result.first != view.end())
            {
                m_data.emplace_back(result.first, result.second);
                result = walk_path({ result.second, view.end() });
            }
        }
        void append(const basic_path& p)
        {
            m_data.insert(
                m_data.end(),
                p.m_data.begin(),
                p.m_data.end()
            );
        }
        void append(basic_path&& p)
        {
            m_data.insert(
                m_data.end(),
                std::make_move_iterator(p.m_data.begin()),
                std::make_move_iterator(p.m_data.end())
            );
        }

        void concat(string_view_type view)
        {
            concat(basic_path(view));
        }
        void concat(const basic_path& p)
        {
            if(p.empty()) return;
            if(!m_data.empty())
            {
                auto it = p.m_data.begin();
                m_data.back() += *it++;
                if(it != p.m_data.end())
                {
                    m_data.insert(
                        m_data.end(),
                        it,
                        p.m_data.end()
                    );
                }
            }
            else
                append(p);
        }

        void clear() noexcept
        {
            m_data.clear();
        }

        string_type string() const
        {
            string_type result;
            bool first = true;
            for(auto& i : m_data)
            {
                if(first)
                    first = false;
                else
                    result += separator;
                result += i;
            }

            return std::move(result);
        }

        basic_path front() const { return basic_path(m_data.front()); }
        basic_path back() const { return basic_path(m_data.back()); }

        void pop_back() { m_data.pop_back(); }
        void pop_front() { m_data.erase(m_data.begin()); } // vector doesn't have a "pop_front()" method

        int compare(const basic_path& other) const noexcept
        {
            auto i = m_data.begin();
            auto j = other.m_data.begin();
            int result = 0;
            while(i != m_data.end() && j != other.m_data.end())
            {
                result = i->compare(*j);
                if(result != 0)
                    return result;
                ++i;
                ++j;
            }

            if(result == 0)
            {
                if(m_data.size() < other.m_data.size())
                    return -1;
                else if(m_data.size() > other.m_data.size())
                    return 1;
                else // m_data.size() == other.m_data.size()
                    return 0;
            }

            return result;
        }
        int compare(string_view_type view) const
        {
            return compare(basic_path(view));
        }
        int compare(const value_type* ptr) const
        {
            return compare(string_view_type(ptr));
        }

        friend bool operator==(const basic_path& lhs, const basic_path& rhs) noexcept
        {
            return lhs.compare(rhs) == 0;
        }
        friend bool operator==(const basic_path& lhs, string_view_type rhs)
        {
            return lhs == basic_path(rhs);
        }
        friend bool operator==(string_view_type lhs, const basic_path& rhs)
        {
            return basic_path(lhs) == rhs;
        }
        friend bool operator==(const basic_path& lhs, const value_type* rhs)
        {
            return lhs == string_view_type(rhs);
        }
        friend bool operator==(const value_type* lhs, const basic_path& rhs)
        {
            return string_view_type(lhs) == rhs;
        }

        friend std::strong_ordering operator<=>(const basic_path& lhs, const basic_path& rhs) noexcept
        {
            int result =  lhs.compare(rhs);
            return result == 0 ? std::strong_ordering::equal :
                result < 0 ? std::strong_ordering::less : std::strong_ordering::greater;
        }
        friend std::strong_ordering operator<=>(const basic_path& lhs, string_view_type rhs)
        {
            return lhs <=> basic_path(rhs);
        }
        friend std::strong_ordering operator<=>(string_view_type lhs, const basic_path& rhs)
        {
            return basic_path(lhs) <=> rhs;
        }
        friend std::strong_ordering operator<=>(const basic_path& lhs, const value_type* rhs)
        {
            return lhs <=> string_view_type(rhs);
        }
        friend std::strong_ordering operator<=>(const value_type* lhs, const basic_path& rhs)
        {
            return string_view_type(lhs) <=> rhs;
        }

        friend basic_path operator/(const basic_path& lhs, const basic_path& rhs)
        {
            basic_path p(lhs);
            p.append(rhs);
            return p;
        }
        friend basic_path operator/(const basic_path& lhs, string_view_type rhs)
        {
            return lhs / basic_path(rhs);
        }
        friend basic_path operator/(string_view_type lhs, const basic_path& rhs)
        {
            return basic_path(lhs) / rhs;
        }

        basic_path& operator/=(string_view_type view)
        {
            append(view);
            return *this;
        }
        basic_path& operator/=(const basic_path& p)
        {
            append(p);
            return *this;
        }

        friend basic_path operator+(const basic_path& lhs, const basic_path& rhs)
        {
            basic_path p(lhs);
            p.concat(rhs);
            return p;
        }
        friend basic_path operator+(const basic_path& lhs, string_view_type rhs)
        {
            return lhs + basic_path(rhs);
        }
        friend basic_path operator+(string_view_type lhs, const basic_path& rhs)
        {
            return basic_path(lhs) + rhs;
        }

        basic_path& operator+=(string_view_type view)
        {
            concat(view);
            return *this;
        }
        basic_path& operator+=(const basic_path& p)
        {
            concat(p);
            return *this;
        }

        bool empty() const noexcept { return m_data.empty(); }

        class path_iterator
        {
            friend basic_path;
            typedef typename std::vector<string_type>::const_iterator iterator_type;

            path_iterator(iterator_type it)
                : m_it(it) {}

        public:
            typedef basic_path value_type;

            path_iterator() = default;
            path_iterator(const path_iterator&) = default;

            path_iterator& operator=(const path_iterator&) = default;

            bool operator==(const path_iterator& other) const noexcept { return m_it == other.m_it; }
            bool operator!=(const path_iterator& other) const noexcept { return m_it != other.m_it; }
            path_iterator& operator++() { ++m_it; return *this; }
            path_iterator operator++(int) { path_iterator cp(*this); ++m_it; return cp; }
            value_type operator*() const { return value_type(*m_it); }

        private:
            iterator_type m_it;
        };

        typedef path_iterator const_iterator;
        typedef const_iterator iterator;

        const_iterator begin() const { return m_data.begin(); }
        const_iterator end() const { return m_data.end(); }

    private:
        std::vector<string_type> m_data;

        static walk_result walk_path(walk_result start)
        {
            walk_result result = start;
            auto beg = std::find_if(start.first, start.second, [](auto&& v) { return v != separator; });
            result.first = beg;
            result.second = std::find(beg, start.second, separator);

            return result;
        }
    };

    template <char Separator>
    std::string to_string(const basic_path<char, Separator>& p)
    {
        return p.string();
    }

    typedef basic_path<char, '/'> path;
}
