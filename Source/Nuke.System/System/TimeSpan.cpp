﻿
#include "_pch.h"
#include <System/Math.h>
#include "TimeSpan.h"

namespace Nuke::System
{
    static_assert(sizeof(TimeSpan) == sizeof(int64_t), "[编译期异常] TimeSpan大小禁止超过64bit");

    const int64_t TicksPerMillisecond = 10000;
    const int64_t TicksPerSecond = TicksPerMillisecond * 1000;   // 10,000,000
    const int64_t TicksPerMinute = TicksPerSecond * 60;         // 600,000,000
    const int64_t TicksPerHour = TicksPerMinute * 60;        // 36,000,000,000
    const int64_t TicksPerDay = TicksPerHour * 24;          // 864,000,000,000

    const int64_t MaxSeconds = std::numeric_limits<int64_t>::max() / TicksPerSecond;
    const int64_t MinSeconds = std::numeric_limits<int64_t>::min() / TicksPerSecond;
    const int64_t MaxMilliSeconds = std::numeric_limits<int64_t>::max() / TicksPerMillisecond;
    const int64_t MinMilliSeconds = std::numeric_limits<int64_t>::min() / TicksPerMillisecond;
    const int64_t TicksPerTenthSecond = TicksPerMillisecond * 100;

    const TimeSpan Zero(0);

    const TimeSpan MaxValue(std::numeric_limits<int64_t>::max());
    const TimeSpan MinValue(std::numeric_limits<int64_t>::min());

    TimeSpan TimeSpan::MaxValue(INT64_MAX);
    TimeSpan TimeSpan::MinValue(INT64_MIN);
    TimeSpan TimeSpan::Zero(0);

    static int64_t TimeToTicks(int32_t hour, int32_t minute, int32_t second)
    {
        int64_t totalSeconds = static_cast<int64_t>(hour) * 3600
            + static_cast<int64_t>(minute) * 60
            + static_cast<int64_t>(second);
        if (totalSeconds > MaxSeconds || totalSeconds < MinSeconds)
        {
            throw std::out_of_range("TimeSpan too int64_t!");
        }
        return totalSeconds * TicksPerSecond;
    }

    static TimeSpan IntervalFromDoubleTicks(double ticks)
    {
        if ((ticks > static_cast<double>(std::numeric_limits<int64_t>::max()))
            || (ticks < static_cast<double>(std::numeric_limits<int64_t>::min()))
            || std::isnan(ticks))
        {
            throw std::out_of_range("TimeSpan too int64_t!");
        }
        if (ticks == static_cast<double>(std::numeric_limits<int64_t>::max()))
        {
            return MaxValue;
        }
        return TimeSpan((int64_t)ticks);
    }

    static TimeSpan Interval(double value, double scale)
    {
        if (std::isnan(value))
        {
            throw std::invalid_argument("value cannot be NaN");
        }
        return IntervalFromDoubleTicks(value * scale);
    }

    static int32_t Compare(TimeSpan t1, TimeSpan t2)
    {
        if (t1.Ticks() > t2.Ticks())
        {
            return 1;
        }
        if (t1.Ticks() < t2.Ticks())
        {
            return -1;
        }
        return 0;
    }

    // 成员
    TimeSpan::TimeSpan(int64_t ticks)
    {
        _ticks = ticks;
    }

    TimeSpan::TimeSpan(int32_t hours, int32_t minutes, int32_t seconds)
    {
        _ticks = TimeToTicks(hours, minutes, seconds);
    }

    TimeSpan::TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds)
        :TimeSpan(days, hours, minutes, seconds, 0)
    {

    }

    TimeSpan::TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds)
    {
        int64_t totalMilliSeconds = (static_cast<int64_t>(days) * 3600 * 24
            + static_cast<int64_t>(hours) * 3600
            + static_cast<int64_t>(minutes) * 60 + seconds) * 1000
            + milliseconds;
        if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
        {
            throw std::out_of_range("TimeSpan too int64_t!");
        }
        _ticks = (int64_t)totalMilliSeconds * TicksPerMillisecond;
    }

    bool TimeSpan::Equals(TimeSpan t1, TimeSpan t2)
    {
        return t1.Ticks() == t2.Ticks();
    }

    bool TimeSpan::Equals(TimeSpan timeSpan) const
    {
        return Equals(*this, timeSpan);
    }

    int32_t TimeSpan::GetHashCode() const
    {
        return (int32_t)_ticks ^ (int32_t)(_ticks >> 32);
    }

    int64_t TimeSpan::Ticks() const
    {
        return _ticks;
    }

    int32_t TimeSpan::Days() const
    {
        return (int32_t)(_ticks / TicksPerDay);
    }

    int32_t TimeSpan::Hours() const
    {
        return (int32_t)((_ticks / TicksPerHour) % 24);
    }

    int32_t TimeSpan::Milliseconds() const
    {
        return (int32_t)((_ticks / TicksPerMillisecond) % 1000);
    }

    int32_t TimeSpan::Minutes() const
    {
        return (int32_t)((_ticks / TicksPerMinute) % 60);
    }

    int32_t TimeSpan::Seconds() const
    {
        return (int32_t)((_ticks / TicksPerSecond) % 60);
    }

    double TimeSpan::TotalDays() const
    {
        return ((double)_ticks) / TicksPerDay;
    }

    double TimeSpan::TotalHours() const
    {
        return (double)_ticks / TicksPerHour;
    }

    double TimeSpan::TotalMilliseconds() const
    {
        double temp = (double)_ticks / TicksPerMillisecond;
        if (temp > MaxMilliSeconds)
        {
            return (double)MaxMilliSeconds;
        }

        if (temp < MinMilliSeconds)
        {
            return (double)MinMilliSeconds;
        }

        return temp;

    }

    double TimeSpan::TotalMinutes() const
    {
        return (double)_ticks / TicksPerMinute;
    }

    double TimeSpan::TotalSeconds() const
    {
        return (double)_ticks / TicksPerSecond;
    }

    std::string TimeSpan::ToString() const
    {
        char buffer[64];
        int64_t totalMs = TotalMilliseconds();

        auto msPerHour = 3600 * 1000;
        auto msPerMinute = 60 * 1000;
        auto msPerSecond = 1000;

        int32_t hour = totalMs / msPerHour;
        int32_t minute = (totalMs - hour * msPerHour) / msPerMinute;
        int32_t second = (totalMs - hour * msPerHour - minute * msPerMinute) / msPerSecond;
        int32_t ms = (totalMs - hour * msPerHour - minute * msPerMinute - second * msPerSecond);
        sprintf(buffer, "%d:%d:%d,%d", hour, minute, second, ms);
        return buffer;
    }

    TimeSpan TimeSpan::Add(TimeSpan ts) const
    {
        int64_t result = _ticks + ts._ticks;
        if ((_ticks >> 63 == ts._ticks >> 63)
            && (_ticks >> 63 != result >> 63))
        {
            throw std::overflow_error("time span is too int64_t");
        }
        return TimeSpan(result);
    }

    int32_t TimeSpan::CompareTo(TimeSpan value) const
    {
        int64_t t = value._ticks;
        if (_ticks > t)
        {
            return 1;
        }
        if (_ticks < t)
        {
            return -1;
        }
        return 0;
    }
    TimeSpan TimeSpan::FromDays(double value)
    {
        return Interval(value, TicksPerDay);
    }

    TimeSpan TimeSpan::Duration() const
    {
        if (Ticks() == MinValue.Ticks())
        {
            throw std::overflow_error("duration overflow");
        }
        return TimeSpan(_ticks >= 0 ? _ticks : -_ticks);
    }

    TimeSpan TimeSpan::FromHours(double value)
    {
        return Interval(value, TicksPerHour);
    }

    TimeSpan TimeSpan::FromMilliseconds(double value)
    {
        return Interval(value, TicksPerMillisecond);
    }

    TimeSpan TimeSpan::FromMinutes(double value)
    {
        return Interval(value, TicksPerMinute);
    }

    TimeSpan TimeSpan::FromSeconds(double value)
    {
        return Interval(value, TicksPerSecond);
    }

    TimeSpan TimeSpan::FromTicks(int64_t value)
    {
        return TimeSpan(value);
    }
    TimeSpan TimeSpan::Subtract(TimeSpan ts) const
    {
        int64_t result = _ticks - ts._ticks;
        if ((_ticks >> 63 != ts._ticks >> 63) && (_ticks >> 63 != result >> 63))
        {
            throw std::overflow_error("TimeSpan is too long.");
        }
        return TimeSpan(result);
    }

    TimeSpan TimeSpan::Multiply(double factor) const
    {
        return *this * factor;
    }

    TimeSpan TimeSpan::Divide(double divisor) const
    {
        return *this / divisor;
    }

    double TimeSpan::Divide(TimeSpan ts) const
    {
        return *this / ts;
    }

    TimeSpan TimeSpan::operator -() const
    {
        if (_ticks == MinValue._ticks)
        {
            throw std::overflow_error("negate tows compnum");
        }
        return TimeSpan(-_ticks);
    }

    TimeSpan TimeSpan::operator -(TimeSpan t2) const
    {
        return this->Subtract(t2);
    }

    TimeSpan TimeSpan::operator +() const
    {
        return *this;
    }

    TimeSpan TimeSpan::operator +(TimeSpan t2) const
    {
        return this->Add(t2);
    }

    TimeSpan TimeSpan::operator *(double factor) const
    {
        if (std::isnan(factor))
        {
            throw std::invalid_argument("factor cannot be NaN");
        }

        double ticks = Math::Round(_ticks * factor);
        return IntervalFromDoubleTicks(ticks);
    }

    TimeSpan TimeSpan::operator /(double divisor) const
    {
        if (std::isnan(divisor))
        {
            throw std::invalid_argument("divisor cannot be NaN");
        }
        double ticks = Math::Round(_ticks / divisor);
        return IntervalFromDoubleTicks(ticks);
    }
    double TimeSpan::operator /(TimeSpan t2) const
    {
        return _ticks / (double)t2.Ticks();
    }

    bool TimeSpan::operator ==(TimeSpan t2) const
    {
        return _ticks == t2._ticks;
    }

    bool TimeSpan::operator !=(TimeSpan t2) const
    {
        return _ticks != t2._ticks;
    }

    bool TimeSpan::operator <(TimeSpan t2) const
    {
        return _ticks < t2._ticks;
    }

    bool TimeSpan::operator <=(TimeSpan t2) const
    {
        return _ticks <= t2._ticks;
    }

    bool TimeSpan::operator >(TimeSpan t2) const
    {
        return _ticks > t2._ticks;
    }

    bool TimeSpan::operator >=(TimeSpan t2) const
    {
        return _ticks >= t2._ticks;
    }
}