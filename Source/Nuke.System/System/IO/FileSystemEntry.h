
#pragma once

namespace Nuke::System::IO
{
    class FileSystemEntry
    {
    public:
        FileSystemEntry() = delete;
        FileSystemEntry(const FileSystemEntry&) = delete;
        FileSystemEntry(FileSystemEntry&&) = delete;
        ~FileSystemEntry() = delete;
    };
}