#pragma once

#include <vector>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <fabtex/radical.hpp>


namespace fabtex
{
    class xml_parser
    {
    public:
        void parse(const std::string& text);

        const std::vector<radical>& radicals() const { return m_radicals; }

    private:
        std::vector<radical> m_radicals;

        void parse_radical(rapidxml::xml_node<char>* xn);

        [[noreturn]]
        static void raise_error(std::string_view what);
    };
}
