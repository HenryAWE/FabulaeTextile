#include <fabtex/data.hpp>
#include <stdexcept>


namespace fabtex
{
    namespace detailed
    {
        std::string_view get_name(rapidxml::xml_node<char>* xml_node)
        {
            return std::string_view(xml_node->name(), xml_node->name_size());
        }
        std::string_view get_value(rapidxml::xml_attribute<char>* xml_attr)
        {
            if(!xml_attr)
                return std::string_view();
            return std::string_view(xml_attr->value(), xml_attr->value_size());
        }

        stroke_type map_stroke_type(std::string_view str)
        {
            using namespace std;

            if(str == "h"sv) return stroke_type::heng;
            if(str == "s"sv) return stroke_type::shu;

            throw std::invalid_argument("invalid stroke type name: " + std::string(str));
        }
        void parse_stroke(radical& output, rapidxml::xml_node<char>* xn)
        {
            auto* type = xn->first_attribute("type");
            stroke result(map_stroke_type(get_value(type)));
            for(auto* n = xn->first_node("anchor"); n; n = n->next_sibling("anchor"))
            {
                auto src = std::string(get_value(n->first_attribute("src")));
                auto dst = std::string(get_value(n->first_attribute("dst")));
                if(!src.empty() && !dst.empty())
                    result.connections.emplace(std::move(src), std::move(dst));
            }

            output.strokes.push_back(std::move(result));
        }
    }

    void xml_parser::parse(const std::string& text)
    {
        using namespace std;

        rapidxml::xml_document<char> xml_doc{};
        xml_doc.parse<rapidxml::parse_non_destructive>(
            (char*)text.c_str()
        );

        auto* fabtex_node = xml_doc.first_node();
        if(detailed::get_name(fabtex_node) != "fabtex"sv)
        {
            throw std::runtime_error("root node is not \"fabtex\"");
        }

        for(auto* n = fabtex_node->first_node(); n; n = n->next_sibling())
        {
            auto name = detailed::get_name(n);
            if(name == "radical"sv)
            {
                parse_radical(n);
            }
        }
    }

    void xml_parser::parse_radical(rapidxml::xml_node<char>* xn)
    {
        auto* base = xn->first_node("base");
        if(!xn)
        {
            raise_error("missing <base> tag");
        }

        radical result;
        result.name = detailed::get_value(xn->first_attribute("name"));
        if(result.name.empty())
            raise_error("missing \"name\" attribute");
        for(auto* n = base->first_node("stroke"); n; n = n->next_sibling("stroke"))
        {
            detailed::parse_stroke(result, n);
        }

        m_radicals.push_back(std::move(result));
    }

    void xml_parser::raise_error(std::string_view what)
    {
        throw std::runtime_error(std::string(what));
    }
}
