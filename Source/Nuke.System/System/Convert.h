
#pragma once

#include <_pch.h>

namespace Nuke::System
{
    class Convert
    {
    public:
        static std::vector<char> FromBase64String(const std::string& s);
        static std::string ToBase64String(const char* inArray, int32_t offset, int32_t length);
    };
}