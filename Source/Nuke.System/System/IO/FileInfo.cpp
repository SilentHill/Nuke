
#include "_pch.h"
#include "FileInfo.h"

namespace Nuke::System::IO
{
    FileAttributes operator|(FileAttributes f1, FileAttributes f2)
    {
        return static_cast<FileAttributes>
            ((static_cast<typename std::underlying_type<FileAttributes>::type>(f1)
                | static_cast<typename std::underlying_type<FileAttributes>::type>(f2)));
    }
}
