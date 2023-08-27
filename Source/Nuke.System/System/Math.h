
#pragma once

#include <_pch.h>

namespace Nuke::System
{
    class Math
    {
    public:
        template <typename NumericType>
        static NumericType Min(NumericType n1, NumericType n2)
        {
            return (n1 <= n2) ? n1 : n2;
        }
        static double Round(double a);
        static float Round(float a);
        static std::tuple<uint32_t, uint32_t> DivRem(uint32_t left, uint32_t right)
        {
            uint32_t quotient = left / right;
            return { quotient, left - (quotient * right) };
        }

        template <typename NumericType>
        static std::tuple<NumericType, NumericType> DivRem(NumericType left, NumericType right)
        {
            NumericType quotient = left / right;
            return (quotient, left - (quotient * right));
        }

        static int64_t BigMul(int32_t a, int32_t b)
        {
            return ((int64_t)a) * b;
        }

        static uint64_t BigMul(uint64_t a, uint64_t b, uint64_t& low)
        {
            // if (Bmi2.X64.IsSupported)
            // {
            // 	uint64_t tmp;
            // 	uint64_t high = Bmi2.X64.MultiplyNoFlags(a, b, &tmp);
            // 	low = tmp;
            // 	return high;
            // }
            // else if (ArmBase.Arm64.IsSupported)
            // {
            // 	low = a * b;
            // 	return ArmBase.Arm64.MultiplyHigh(a, b);
            // }


            // Adaptation of algorithm for multiplication
            // of 32-bit unsigned integers described
            // in Hacker's Delight by Henry S. Warren, Jr. (ISBN 0-201-91465-4), Chapter 8
            // Basically, it's an optimized version of FOIL method applied to
            // low and high dwords of each operand

            // Use 32-bit uints to optimize the fallback for 32-bit platforms.
            uint al = (uint)a;
            uint ah = (uint)(a >> 32);
            uint bl = (uint)b;
            uint bh = (uint)(b >> 32);

            uint64_t mull = ((uint64_t)al) * bl;
            uint64_t t = ((uint64_t)ah) * bl + (mull >> 32);
            uint64_t tl = ((uint64_t)al) * bh + (uint)t;

            low = tl << 32 | (uint)mull;

            return ((uint64_t)ah) * bh + (t >> 32) + (tl >> 32);

        }

        static int64_t BigMul(int64_t a, int64_t b, int64_t& low)
        {
            // todo : arm64
            //if (ArmBase.Arm64.IsSupported)
            //{
            //	low = a * b;
            //	return ArmBase.Arm64.MultiplyHigh(a, b);
            //}
            uint64_t ulow;
            uint64_t high = BigMul((uint64_t)a, (uint64_t)b, ulow);
            low = (int64_t)ulow;
            return (int64_t)high - ((a >> 63) & b) - ((b >> 63) & a);
        }
    private:
        Math() = delete;
        Math(const Math& math) = delete;
        ~Math() = delete;
    };
}