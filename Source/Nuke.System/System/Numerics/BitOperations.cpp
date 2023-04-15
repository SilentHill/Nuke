
#include "_pch.h"
#include "BitOperations.h"

namespace Nuke::System::Numerics
{
    uint32_t BitOperations::RotateLeft(uint32_t value, int offset)
    {
        return (value << offset) | (value >> (32 - offset));
    }
    
    uint64_t BitOperations::RotateLeft(uint64_t value, int offset)
    {
        return (value << offset) | (value >> (64 - offset));
    }

    uint32_t BitOperations::RotateRight(uint32_t value, int offset)
    {
        return (value >> offset) | (value << (32 - offset));
    }

    uint64_t BitOperations::RotateRight(uint64_t value, int offset)
    {
        return (value >> offset) | (value << (64 - offset));
    }
}