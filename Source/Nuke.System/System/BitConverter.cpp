
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
}