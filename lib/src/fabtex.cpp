#include <fabtex.hpp>


namespace fabtex
{
    const char* get_library_info()
    {
#ifdef _DEBUG
        return "libfabtex v0.1.0 DEBUG";
#else
        return "libfabtex v0.1.0";
#endif
    }
}
