
#include "_pch.h"
#include "Directory.h"

namespace Nuke::System::IO
{
    enum SearchTarget
    {
        Files = 0x1,
        Directories = 0x2,
        Both = 0x3
    };

    std::vector<std::string> InternalEnumeratePaths(std::string& path, std::string& searchPattern, SearchTarget searchTarget, EnumerationOptions options)
    {
        return {};
    }

    std::vector<std::string> Directory::EnumerateDirectories(const std::string& path)
    {
        return EnumerateDirectories(path, "*", EnumerationOptions::Compatible());
    }

    std::vector<std::string> Directory::EnumerateDirectories(const std::string& path, const std::string& searchPattern, EnumerationOptions enumerationOptions)
    {
        return std::vector<std::string>();
    }
}
