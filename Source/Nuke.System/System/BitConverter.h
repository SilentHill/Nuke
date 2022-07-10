
#pragma once

namespace Nuke::System
{
    class BitConverter
    {
    public:
#if BIGENDIAN
        static const bool IsLittleEndian /* = false */;
#else
        static const bool IsLittleEndian = true;
#endif
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

        //static char ToChar(byte[] value, int32_t startIndex) = > unchecked((char)ToInt16(value, startIndex));
        //
        //static char ToChar(ReadOnlySpan<byte> value)
        //{
        //    if (value.Length < sizeof(char))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<char>(ref MemoryMarshal.GetReference(value));
        //}
        //static int16_t ToInt16(byte[] value, int32_t startIndex)
        //{
        //    if (value == null)
        //        ThrowHelper.ThrowArgumentNullException(ExceptionArgument.value);
        //    if (unchecked((uint32_t)startIndex) >= unchecked((uint32_t)value.Length))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.startIndex, ExceptionResource.ArgumentOutOfRange_IndexMustBeLess);
        //    if (startIndex > value.Length - sizeof(int16_t))
        //        ThrowHelper.ThrowArgumentException(ExceptionResource.Arg_ArrayPlusOffTooSmall, ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<int16_t>(ref value[startIndex]);
        //}
        //
        //static int16_t ToInt16(ReadOnlySpan<byte> value)
        //{
        //    if (value.Length < sizeof(int16_t))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<int16_t>(ref MemoryMarshal.GetReference(value));
        //}
        //static int32_t ToInt32(byte[] value, int32_t startIndex)
        //{
        //    if (value == null)
        //        ThrowHelper.ThrowArgumentNullException(ExceptionArgument.value);
        //    if (unchecked((uint32_t)startIndex) >= unchecked((uint32_t)value.Length))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.startIndex, ExceptionResource.ArgumentOutOfRange_IndexMustBeLess);
        //    if (startIndex > value.Length - sizeof(int32_t))
        //        ThrowHelper.ThrowArgumentException(ExceptionResource.Arg_ArrayPlusOffTooSmall, ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<int32_t>(ref value[startIndex]);
        //}
        //
        //static int32_t ToInt32(ReadOnlySpan<byte> value)
        //{
        //    if (value.Length < sizeof(int32_t))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<int32_t>(ref MemoryMarshal.GetReference(value));
        //}
        //static int64_t ToInt64(byte[] value, int32_t startIndex)
        //{
        //    if (value == null)
        //        ThrowHelper.ThrowArgumentNullException(ExceptionArgument.value);
        //    if (unchecked((uint32_t)startIndex) >= unchecked((uint32_t)value.Length))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.startIndex, ExceptionResource.ArgumentOutOfRange_IndexMustBeLess);
        //    if (startIndex > value.Length - sizeof(int64_t))
        //        ThrowHelper.ThrowArgumentException(ExceptionResource.Arg_ArrayPlusOffTooSmall, ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<int64_t>(ref value[startIndex]);
        //}
        //
        //static int64_t ToInt64(ReadOnlySpan<byte> value)
        //{
        //    if (value.Length < sizeof(int64_t))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<int64_t>(ref MemoryMarshal.GetReference(value));
        //}
        //
        //static uint16_t ToUInt16(byte[] value, int32_t startIndex) = > unchecked((uint16_t)ToInt16(value, startIndex));
        //
        //static uint16_t ToUInt16(ReadOnlySpan<byte> value)
        //{
        //    if (value.Length < sizeof(uint16_t))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<uint16_t>(ref MemoryMarshal.GetReference(value));
        //}
        //
        //static uint32_t ToUInt32(byte[] value, int32_t startIndex) = > unchecked((uint32_t)ToInt32(value, startIndex));
        //
        //static uint32_t ToUInt32(ReadOnlySpan<byte> value)
        //{
        //    if (value.Length < sizeof(uint32_t))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<uint32_t>(ref MemoryMarshal.GetReference(value));
        //}
        //
        //static uint64_t ToUInt64(byte[] value, int32_t startIndex) = > unchecked((uint64_t)ToInt64(value, startIndex));
        //
        //static uint64_t ToUInt64(ReadOnlySpan<byte> value)
        //{
        //    if (value.Length < sizeof(uint64_t))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<uint64_t>(ref MemoryMarshal.GetReference(value));
        //}
        //static Half ToHalf(byte[] value, int32_t startIndex) = > Int16BitsToHalf(ToInt16(value, startIndex));
        //
        //static Half ToHalf(ReadOnlySpan<byte> value)
        //{
        //    if (value.Length < sizeof(Half))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<Half>(ref MemoryMarshal.GetReference(value));
        //}
        //static float ToSingle(byte[] value, int32_t startIndex) = > Int32BitsToSingle(ToInt32(value, startIndex));
        //
        //static float ToSingle(ReadOnlySpan<byte> value)
        //{
        //    if (value.Length < sizeof(float))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<float>(ref MemoryMarshal.GetReference(value));
        //}
        //static double ToDouble(byte[] value, int32_t startIndex) = > Int64BitsToDouble(ToInt64(value, startIndex));
        //
        //static double ToDouble(ReadOnlySpan<byte> value)
        //{
        //    if (value.Length < sizeof(double))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<double>(ref MemoryMarshal.GetReference(value));
        //}
        //static std::string ToString(byte[] value, int32_t startIndex, int32_t length)
        //{
        //    if (value == null)
        //        ThrowHelper.ThrowArgumentNullException(ExceptionArgument.value);
        //    if (startIndex < 0 || startIndex >= value.Length && startIndex > 0)
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.startIndex, ExceptionResource.ArgumentOutOfRange_IndexMustBeLess);
        //    if (length < 0)
        //        throw new ArgumentOutOfRangeException(nameof(length), SR.ArgumentOutOfRange_GenericPositive);
        //    if (startIndex > value.Length - length)
        //        ThrowHelper.ThrowArgumentException(ExceptionResource.Arg_ArrayPlusOffTooSmall, ExceptionArgument.value);
        //    if (length == 0)
        //    {
        //        return std::string.Empty;
        //    }
        //    if (length > (int32_t.MaxValue / 3))
        //    {
        //        // (int32_t.MaxValue / 3) == 715,827,882 Bytes == 699 MB
        //        throw new ArgumentOutOfRangeException(nameof(length), SR.Format(SR.ArgumentOutOfRange_LengthTooLarge, int32_t.MaxValue / 3));
        //    }
        //    return std::string.Create(length * 3 - 1, (value, startIndex, length), static (dst, state) = >
        //    {
        //        var src = new ReadOnlySpan<byte>(state.value, state.startIndex, state.length);
        //        int32_t i = 0;
        //        int32_t j = 0;
        //        byte b = src;
        //        dst[j++] = HexConverter.ToCharUpper(b >> 4);
        //        dst[j++] = HexConverter.ToCharUpper(b);
        //        while (i < src.Length)
        //        {
        //            b = src;
        //            dst[j++] = '-';
        //            dst[j++] = HexConverter.ToCharUpper(b >> 4);
        //            dst[j++] = HexConverter.ToCharUpper(b);
        //        }
        //    });
        //}
        //static std::string ToString(byte[] value)
        //{
        //    if (value == null)
        //        ThrowHelper.ThrowArgumentNullException(ExceptionArgument.value);
        //    return ToString(value, 0, value.Length);
        //}
        //static std::string ToString(byte[] value, int32_t startIndex)
        //{
        //    if (value == null)
        //        ThrowHelper.ThrowArgumentNullException(ExceptionArgument.value);
        //    return ToString(value, startIndex, value.Length - startIndex);
        //}
        //static bool ToBoolean(byte[] value, int32_t startIndex)
        //{
        //    if (value == null)
        //        ThrowHelper.ThrowArgumentNullException(ExceptionArgument.value);
        //    if (startIndex < 0)
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.startIndex, ExceptionResource.ArgumentOutOfRange_IndexMustBeLess);
        //    if (startIndex >= value.Length)
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.startIndex, ExceptionResource.ArgumentOutOfRange_IndexMustBeLess); // differs from other overloads, which throw base ArgumentException
        //    return value[startIndex] != 0;
        //}
        //
        //static bool ToBoolean(ReadOnlySpan<byte> value)
        //{
        //    if (value.Length < sizeof(byte))
        //        ThrowHelper.ThrowArgumentOutOfRangeException(ExceptionArgument.value);
        //    return Unsafe.ReadUnaligned<byte>(ref MemoryMarshal.GetReference(value)) != 0;
        //}

        static int64_t DoubleToInt64Bits(double value)
        {
            return *((int64_t*)&value);
        }

        static double Int64BitsToDouble(int64_t value)
        {
            return *((double*)&value);
        }

        static int32_t SingleToInt32Bits(float value)
        {
            return *((int32_t*)&value);
        }

        static float Int32BitsToSingle(int32_t value)
        {
            return *((float*)&value);
        }

        static uint64_t DoubleToUInt64Bits(double value)
        {
            return *((uint64_t*)&value);
        }

        static double UInt64BitsToDouble(uint64_t value)
        {
            return *((double*)&value);
        }

        static uint32_t SingleToUInt32Bits(float value)
        {
            return *((uint32_t*)&value);
        } 

        static float UInt32BitsToSingle(uint32_t value)
        {
            return *((float*)&value);
        } 
    };
}