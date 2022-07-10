
#pragma once

#include "_pch.h"
#include "System/Event.h"
#include "System/DateTime.h"
#include "System/BitConverter.h"


namespace Nuke::System::Buffers::Binary
{
    class BinaryPrimitives
    {
    public:
        static int8_t ReverseEndianness(int8_t value);

        static int16_t ReverseEndianness(int16_t value);

        static int32_t ReverseEndianness(int32_t value);

        static int64_t ReverseEndianness(int64_t value);

        static uint8_t ReverseEndianness(uint8_t value);

        static uint16_t ReverseEndianness(uint16_t value);

        static uint32_t ReverseEndianness(uint32_t value);

        static uint64_t ReverseEndianness(uint64_t value);

        static double ReadDoubleBigEndian(std::span<std::byte> source);

        static int16_t ReadInt16BigEndian(std::span<std::byte> source);

        static int32_t ReadInt32BigEndian(std::span<std::byte> source);

        static int64_t ReadInt64BigEndian(std::span<std::byte> source);

        static float ReadSingleBigEndian(std::span<std::byte> source);

        static uint16_t ReadUInt16BigEndian(std::span<std::byte> source);

        static uint32_t ReadUInt32BigEndian(std::span<std::byte> source);

        static uint64_t ReadUInt64BigEndian(std::span<std::byte> source);
    };
}