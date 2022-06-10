
#pragma once

namespace Nuke::System::IO
{
    class File
    {
    public:
        File() = delete;
        File(const File&) = delete;
        File(File&&) = delete;
        ~File() = delete;
        static std::string ReadAllText(const std::string& path);
    };
}