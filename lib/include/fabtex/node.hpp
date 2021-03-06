#pragma once

#include <memory>
#include <string>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <fabtex/path.hpp>


namespace fabtex
{
    class node
    {
    public:
        typedef std::size_t size_type;
        typedef std::string_view string_view_type;
        struct comparator
        {
            // Support heterogeneous comparison
            using is_transparent = std::true_type;

            static string_view_type convert(const node& n)
            {
                return n.m_name;
            }
            static string_view_type convert(string_view_type str)
            {
                return str;
            }

            template <typename T, typename U>
            bool operator()(const T& lhs, const U& rhs) const
            {
                return convert(lhs) < convert(rhs);
            }
        };
        typedef std::set<node, comparator> container_type;

        node(const node& other)
            : m_parent(nullptr), m_name(other.m_name), m_children(other.m_children) {}
        node(string_view_type str)
            : m_parent(nullptr), m_name(str) {}
        node(const node& parent, string_view_type str)
            : m_parent(&parent), m_name(str) {}

        ~node() noexcept;

        node& operator=(const node& other) = delete;

        size_type size() const noexcept { return m_children.size(); }
        container_type::reference operator[](string_view_type name) { return const_cast<container_type::reference>(*m_children.find(name)); }
        container_type::const_reference operator[](string_view_type name) const { return *m_children.find(name); }

        template <typename... Args>
        std::pair<node&, bool> emplace(Args&&... args)
        {
            auto result = m_children.emplace(
                *this,
                std::forward<Args>(args)...
            );
            return std::pair<node&, bool>(
                const_cast<node&>(*result.first),
                result.second
            );
        }

        bool contains(string_view_type name) const
        {
            return m_children.contains(name);
        }

        bool is_root() const noexcept
        {
            return m_parent == nullptr;
        }
        const node& parent() const
        {
            if(!m_parent)
            {
                throw std::logic_error("logic error");
            }
            return *m_parent;
        }

        const node* root() const noexcept
        {
            const node* result = this;
            while(result->m_parent)
                result = result->m_parent;
            return result;
        }
        const node* find(const path& p) const
        {
            const node* ptr = this;
            for(const auto& i : p)
            {
                auto it = ptr->m_children.find(i.string());
                if(it == ptr->m_children.end())
                    return nullptr;
                ptr = std::to_address(it);
            }
            return ptr;
        }
        node* find(const path& p)
        {
            return const_cast<node*>(const_cast<const node*>(this)->find(p));
        }
        bool exists(const path& p) const
        {
            return find(p) != nullptr;
        }

        [[nodiscard]]
        constexpr const std::string& name() const noexcept
        {
            return m_name;
        }

    private:
        const node* m_parent = nullptr;
        const std::string m_name;
        container_type m_children;
    };
}
