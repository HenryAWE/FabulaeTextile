#pragma once

#include <variant>
#include <fabtex/radical.hpp>
#include <fabtex/path.hpp>


namespace fabtex
{
    class character
    {
    public:
        typedef std::variant<
            character,
            radical
        > value_type;
    };
}
