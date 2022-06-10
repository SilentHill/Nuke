
#include "_pch.h"
#include "File.h"

namespace Nuke::System::IO
{
    std::string File::ReadAllText(const std::string& path)
    {
        static const std::string emptyString("");
        std::ifstream file(path, std::ios_base::in);
        if (file)
        {
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();
            return buffer.str();
        }
        return emptyString;
    }
}
