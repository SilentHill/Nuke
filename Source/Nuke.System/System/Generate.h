
#pragma once

namespace Nuke::System
{
    class Generate
    {
    public:
        static std::string RandomString(int32_t stringLength);

        static std::string RandomNumberString(int32_t stringLength);
    };
}