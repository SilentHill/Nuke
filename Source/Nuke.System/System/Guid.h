
#pragma once

namespace Nuke::System
{
    class Guid
    {
    public:
        static const Guid Empty;

    private:
        int32_t _a;
        int16_t _b;
        int16_t _c;
        uint8_t _d;
        uint8_t _e;
        uint8_t _f;
        uint8_t _g;
        uint8_t _h;
        uint8_t _i;
        uint8_t _j;
        uint8_t _k;
    };
}