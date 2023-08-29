
#pragma once

#include <_pch.h>

namespace Nuke::System
{
    class Convert
    {
    public:
        static std::vector<char> FromBase64String(const std::string& s);
    };
}