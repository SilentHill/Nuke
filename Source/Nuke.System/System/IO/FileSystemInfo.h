
#pragma once

#include "_pch.h"
#include <System/String.h>

namespace Nuke::System::IO
{
    enum class FileAttributes
    {
        Nothing             = 0,
        ReadOnly            = 0x1,
        Hidden              = 0x2,
        System              = 0x4,
        Directory           = 0x10,
        Archive             = 0x20,
        Device              = 0x40,
        Normal              = 0x80,
        Temporary           = 0x100,
        SparseFile          = 0x200,
        ReparsePoint        = 0x400,
        Compressed          = 0x800,
        Offline             = 0x1000,
        NotContentIndexed   = 0x2000,
        Encrypted           = 0x4000,
        IntegrityStream     = 0x8000,
        NoScrubData         = 0x20000
    };

    // 使FileAttributes可或
    FileAttributes operator|(FileAttributes f1, FileAttributes f2);

    class FileSystemInfo
    {
    public:
        FileSystemInfo() = delete;
        FileSystemInfo(const FileSystemInfo&) = delete;
        FileSystemInfo(FileSystemInfo&&) = delete;
        ~FileSystemInfo() = delete;

    protected:
        std::string FullPath = String::EmptyStdString;
        std::string OriginalPath = String::EmptyStdString;

    private:
        std::string _name = String::EmptyStdString;
        std::string _linkTarget = String::EmptyStdString;
        bool _linkTargetIsValid;
    };
}