
#pragma once

#include <_pch.h>

namespace Nuke::CrossPlatform::TimeZoneApi
{
    const std::string& GetTimeZoneDataBaseJsonString();
    std::vector<std::string> GetTimeZoneIds();
    std::string GetTimeZoneDirectory();
    std::vector<std::string> ParseTimeZoneIds(std::istream& reader);
    extern const std::string DefaultTimeZoneDirectory;
}