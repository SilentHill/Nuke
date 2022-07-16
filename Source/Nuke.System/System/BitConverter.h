
#pragma once

namespace Nuke::System
{
    class BitConverter
    {
    public:
        static const bool IsLittleEndian;

        static auto GetBytes(bool value);

        static bool TryWriteBytes(std::span<std::byte> destination, bool value);

        static auto GetBytes(char value);

        static bool TryWriteBytes(std::span<std::byte> destination, char value);

        static auto GetBytes(int16_t value);

        static bool TryWriteBytes(std::span<std::byte> destination, int16_t value);

        static auto GetBytes(int32_t value);

        static bool TryWriteBytes(std::span<std::byte> destination, int32_t value);

        static auto GetBytes(int64_t value);

        static bool TryWriteBytes(std::span<std::byte> destination, int64_t value);

        static auto GetBytes(uint16_t value);

        static bool TryWriteBytes(std::span<std::byte> destination, uint16_t value);

        static auto GetBytes(uint32_t value);

        static bool TryWriteBytes(std::span<std::byte> destination, uint32_t value);

        static auto GetBytes(uint64_t value);

        static bool TryWriteBytes(std::span<std::byte> destination, uint64_t value);

        static auto GetBytes(float value);

        static bool TryWriteBytes(std::span<std::byte> destination, float value);

        static auto GetBytes(double value);

        static bool TryWriteBytes(std::span<std::byte> destination, double value);

        static int64_t DoubleToInt64Bits(double value);

        static double Int64BitsToDouble(int64_t value);

        static int32_t SingleToInt32Bits(float value);

        static float Int32BitsToSingle(int32_t value);

        static uint64_t DoubleToUInt64Bits(double value);

        static double UInt64BitsToDouble(uint64_t value);

        static uint32_t SingleToUInt32Bits(float value);

        static float UInt32BitsToSingle(uint32_t value);
    };
}