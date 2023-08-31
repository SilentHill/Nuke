
#pragma once

#include <_pch.h>

namespace Nuke::System
{
    class Guid
    {
    public:
        static const Guid Empty;

    private:
        uint64_t high;
        uint64_t low;
    };
}