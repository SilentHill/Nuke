
#pragma once

namespace Nuke::System::Numerics
{
    class BitOperations
    {
    public:
        static uint32_t RotateLeft(uint32_t value, int offset);
        static uint64_t RotateLeft(uint64_t value, int offset);
        static uint32_t RotateRight(uint32_t value, int offset);
        static uint64_t RotateRight(uint64_t value, int offset);


    private:
        BitOperations() = delete;
    };
}