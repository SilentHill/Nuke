
#pragma once

#include "FileSystemInfo.h"

namespace Nuke::System::IO
{
    class FileInfo : public FileSystemInfo
    {
    public:
        FileInfo() = delete;
        FileInfo(const FileInfo&) = delete;
        FileInfo(FileInfo&&) = delete;
        ~FileInfo() = delete;
    };
}