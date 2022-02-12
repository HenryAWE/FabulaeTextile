#pragma once

#include <optional>
#include <string>
#include <fabtex/vector.hpp>


namespace fabtex
{
    class anchor
    {
    public:
        typedef vec<2, float> vector_type;

        anchor(std::string name_, vector_type position_, std::optional<std::string> target_ = std::nullopt) noexcept
            : m_name(std::move(name_)), m_position(position_), m_target(std::move(target_)) {}
        anchor(const anchor&) = default;

        anchor& operator=(const anchor&) = default;

        // getters & setters
        const std::string& name() const noexcept { return m_name; }
        void name(std::string name) { m_name.swap(name); }
        const vector_type& position() const noexcept { return m_position; }
        void position(vector_type new_position) { m_position = new_position; }
        const std::optional<std::string>& target() const noexcept { return m_target; }
        void target(std::optional<std::string> new_target) { m_target = std::move(new_target); }

    private:
        std::string m_name;
        vector_type m_position;
        std::optional<std::string> m_target;
    };
}
