#include "widgets.hpp"


namespace ImFabtex
{
    ImVec2 GetImVec(fabtex::vec<2, float> v) noexcept
    {
        return ImVec2(v[0], v[1]);
    }
}
