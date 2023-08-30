
#pragma once

#include <_pch.h>

namespace Nuke::System
{
    // 表示一个基于tick的时间间隔
    // 本类是一个只读类
    class TimeSpan final
    {
    public: // 构造析构
        TimeSpan() = default;
        TimeSpan(const TimeSpan&) = default;
        TimeSpan(TimeSpan&&) = default;
        ~TimeSpan() = default;

    public: // 赋值重载
        TimeSpan& operator=(const TimeSpan& timeSpan) = default;

    public: // 带参构造
        TimeSpan(int64_t ticks);
        TimeSpan(int32_t hours, int32_t minutes, int32_t seconds);
        TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds);
        TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds);

    public: // 解析静态构造
        static TimeSpan FromHours(double value);
        static TimeSpan FromMilliseconds(double value);
        static TimeSpan FromMinutes(double value);
        static TimeSpan FromSeconds(double value); 
        static TimeSpan FromTicks(int64_t value);

    public: // 静态常量
        static const int64_t NanosecondsPerTick = 100;
        static const int64_t TicksPerDay = 864000000000;
        static const int64_t TicksPerHour = 36000000000;
        static const int64_t TicksPerMicrosecond = 10;
        static const int64_t TicksPerMillisecond = 10000;
        static const int64_t TicksPerMinute = 600000000;
        static const int64_t TicksPerSecond = 10000000;
        static TimeSpan MaxValue;
        static TimeSpan MinValue;
        static TimeSpan Zero;

    public: // 比较与哈希
        static bool Equals(TimeSpan t1, TimeSpan t2);
        bool Equals(TimeSpan timeSpan) const;
        int32_t GetHashCode() const;

    public: // 加减乘除
        TimeSpan Add(TimeSpan ts) const;
        TimeSpan Subtract(TimeSpan ts) const;
        TimeSpan Multiply(double factor) const;
        TimeSpan Divide(double divisor) const;
        double Divide(TimeSpan ts) const;
        
    public: // 加减乘除重载
        TimeSpan operator -() const;
        TimeSpan operator -(TimeSpan t2) const;
        TimeSpan operator +() const;
        TimeSpan operator +(TimeSpan t2) const;
        TimeSpan operator *(double factor) const;
        TimeSpan operator /(double divisor) const;
        double operator /(TimeSpan t2) const;
        bool operator ==(TimeSpan t2) const;
        bool operator !=(TimeSpan t2) const;
        bool operator <(TimeSpan t2) const;
        bool operator <=(TimeSpan t2) const;
        bool operator >(TimeSpan t2) const;
        bool operator >=(TimeSpan t2) const;

    public: // 属性方法
        int64_t Ticks() const;
        int32_t Days() const;
        int32_t Hours() const;
        int32_t Milliseconds() const;
        int32_t Minutes() const;
        int32_t Seconds() const;
        double TotalDays() const;
        double TotalHours() const;
        double TotalMilliseconds() const;
        double TotalMinutes() const;
        double TotalSeconds() const;

    public: // 调试方法
        std::string ToString() const;

    public: // 方法
        int32_t CompareTo(TimeSpan value) const;
        static TimeSpan FromDays(double value);
        TimeSpan Duration() const;



    private:
        friend class DateTime;
        int64_t _ticks;
    };

}