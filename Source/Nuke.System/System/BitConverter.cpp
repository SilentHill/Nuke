
#include "_pch.h"
#include "BitConverter.h"

namespace Nuke::System
{

    template <typename NumericType>
    static std::array<std::byte, sizeof(NumericType)> GenericGetBytes(NumericType value)
    {
        std::array<std::byte, sizeof(NumericType)> bytes;
        *(NumericType*)(&bytes[0]) = value;
        return bytes;
    }

    template <typename NumericType>
    static bool GenericTryWriteBytes(std::span<std::byte> destination, NumericType value)
    {
        if (destination.size() < sizeof(NumericType))
        {
            return false;
        }
        *(NumericType*)(&destination[0]) = value;
        return true;
    }

#if BIGENDIAN
    const bool BitConverter::IsLittleEndian = false;
#else
    const bool BitConverter::IsLittleEndian = true;
#endif

    auto BitConverter::GetBytes(bool value)
    {
        return GenericGetBytes(value);
    }

    bool BitConverter::TryWriteBytes(std::span<std::byte> destination, bool value)
    {
        return GenericTryWriteBytes(destination, value);
    }

    auto BitConverter::GetBytes(char value)
    {
        return GenericGetBytes(value);
    }

    bool BitConverter::TryWriteBytes(std::span<std::byte> destination, char value)
    {
        return GenericTryWriteBytes(destination, value);
    }

    auto BitConverter::GetBytes(int16_t value)
    {
        return GenericGetBytes(value);
    }

    bool BitConverter::TryWriteBytes(std::span<std::byte> destination, int16_t value)
    {
        return GenericTryWriteBytes(destination, value);
    }

    auto BitConverter::GetBytes(int32_t value)
    {
        return GenericGetBytes(value);
    }

    bool BitConverter::TryWriteBytes(std::span<std::byte> destination, int32_t value)
    {
        return GenericTryWriteBytes(destination, value);
    }

    auto BitConverter::GetBytes(int64_t value)
    {
        return GenericGetBytes(value);
    }

    bool BitConverter::TryWriteBytes(std::span<std::byte> destination, int64_t value)
    {
        return GenericTryWriteBytes(destination, value);
    }

    auto BitConverter::GetBytes(uint16_t value)
    {
        return GenericGetBytes(value);
    }

    bool BitConverter::TryWriteBytes(std::span<std::byte> destination, uint16_t value)
    {
        return GenericTryWriteBytes(destination, value);
    }

    auto BitConverter::GetBytes(uint32_t value)
    {
        return GenericGetBytes(value);
    }

    bool BitConverter::TryWriteBytes(std::span<std::byte> destination, uint32_t value)
    {
        return GenericTryWriteBytes(destination, value);
    }

    auto BitConverter::GetBytes(uint64_t value)
    {
        return GenericGetBytes(value);
    }

    bool BitConverter::TryWriteBytes(std::span<std::byte> destination, uint64_t value)
    {
        return GenericTryWriteBytes(destination, value);
    }

    auto BitConverter::GetBytes(float value)
    {
        return GenericGetBytes(value);
    }

    bool BitConverter::TryWriteBytes(std::span<std::byte> destination, float value)
    {
        return GenericTryWriteBytes(destination, value);
    }

    auto BitConverter::GetBytes(double value)
    {
        return GenericGetBytes(value);
    }

    bool BitConverter::TryWriteBytes(std::span<std::byte> destination, double value)
    {
        return GenericTryWriteBytes(destination, value);
    }

    int64_t BitConverter::DoubleToInt64Bits(double value)
    {
        return *((int64_t*)&value);
    }

    double BitConverter::Int64BitsToDouble(int64_t value)
    {
        return *((double*)&value);
    }

    int32_t BitConverter::SingleToInt32Bits(float value)
    {
        return *((int32_t*)&value);
    }

    float BitConverter::Int32BitsToSingle(int32_t value)
    {
        return *((float*)&value);
    }

    uint64_t BitConverter::DoubleToUInt64Bits(double value)
    {
        return *((uint64_t*)&value);
    }

    double BitConverter::UInt64BitsToDouble(uint64_t value)
    {
        return *((double*)&value);
    }

    uint32_t BitConverter::SingleToUInt32Bits(float value)
    {
        return *((uint32_t*)&value);
    }

    float BitConverter::UInt32BitsToSingle(uint32_t value)
    {
        return *((float*)&value);
    }
}