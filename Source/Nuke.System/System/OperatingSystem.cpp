
#include "_pch.h"
#include "OperatingSystem.h"

namespace Nuke::System
{
    bool OperatingSystem::IsWindows()
    {
#ifdef _WIN32
        return true;
#else
        return false;
#endif
    }
    bool OperatingSystem::IsMacOS()
    {
#ifdef __APPLE__
        return true;
#else
        return false;
#endif
    }
    bool OperatingSystem::IsLinux()
    {
#ifdef __linux__
        return true;
#else
        return false;
#endif
    }
}