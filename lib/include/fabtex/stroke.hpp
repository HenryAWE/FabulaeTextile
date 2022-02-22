#pragma once

#include <utility>
#include <vector>
#include <map>
#include <cmath>
#include <fabtex/anchor.hpp>
#include <fabtex/bitmap.hpp>
#include <fabtex/path.hpp>


namespace fabtex
{
    enum class stroke_type : int
    {
        heng, // 横 / Horizontal
        shu // 竖 / Vertical
    };

    class stroke
    {
    public:
        stroke(stroke_type type)
            : m_type(type) {}

        std::map<path, path> connections;

        [[nodiscard]]
        constexpr stroke_type type() const noexcept { return m_type; }

    private:
        stroke_type m_type;
    };
}
