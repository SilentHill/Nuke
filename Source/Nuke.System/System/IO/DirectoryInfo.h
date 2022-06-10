
#pragma once

#include "FileSystemInfo.h"

namespace Nuke::System::IO
{
    class DirectoryInfo : public FileSystemInfo
    {
    public:
        DirectoryInfo() = delete;
        DirectoryInfo(const DirectoryInfo&) = delete;
        DirectoryInfo(DirectoryInfo&&) = delete;
        ~DirectoryInfo() = delete;
    };
}