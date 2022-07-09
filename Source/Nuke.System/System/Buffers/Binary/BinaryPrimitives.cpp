
#include "_pch.h"
#include "BinaryPrimitives.h"

namespace Nuke::System::Buffers::Binary
{
    int8_t BinaryPrimitives::ReverseEndianness(int8_t value)
    {
        return value;
    }

    int16_t BinaryPrimitives::ReverseEndianness(int16_t value)
    {
        return (int16_t)ReverseEndianness((uint16_t)value);
    }

    int32_t BinaryPrimitives::ReverseEndianness(int32_t value)
    {
        return (int32_t)ReverseEndianness((uint32_t)value);
    }

    int64_t BinaryPrimitives::ReverseEndianness(int64_t value)
    {
        return (int64_t)ReverseEndianness((uint64_t)value);
    }

    uint8_t BinaryPrimitives::ReverseEndianness(uint8_t value)
    {
        return value;
    }

    uint16_t BinaryPrimitives::ReverseEndianness(uint16_t value)
    {
        return (uint16_t)((value >> 8) + (value << 8));
    }

    uint32_t BinaryPrimitives::ReverseEndianness(uint32_t value)
    {
        //return BitOperations.RotateRight(value & 0x00FF00FFu, 8) // xx zz
        //    + BitOperations.RotateLeft(value & 0xFF00FF00u, 8); // ww yy
    }

    uint64_t BinaryPrimitives::ReverseEndianness(uint64_t value)
    {
        //return ((uint64_t)ReverseEndianness((uint32_t)value) << 32)
        //    + ReverseEndianness((uint32_t)(value >> 32));
    }
    
}