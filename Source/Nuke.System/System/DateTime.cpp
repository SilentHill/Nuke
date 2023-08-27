
#include "_pch.h"
#include "Math.h"
#include "DateTime.h"
#include "TimeZoneInfo.h"

#ifdef __linux__

#elif _WIN32
#include <windows.h>
#include <timezoneapi.h>
#elif __APPLE__

#endif 

namespace Nuke::System
{
    static_assert(sizeof(DateTime) == sizeof(uint64_t), "[静态断言] DateTime大小禁止超过64位");

    // DateTime常量集
    // 每毫秒的微秒数
    const int32_t MicrosecondsPerMillisecond = 1000;
    const int64_t TicksPerMicrosecond = 10;
    const int64_t TicksPerMillisecond = TicksPerMicrosecond * MicrosecondsPerMillisecond;

    // 每单位的Tick数
    const int32_t HoursPerDay = 24;
    const int64_t TicksPerSecond = TicksPerMillisecond * 1000;
    const int64_t TicksPerMinute = TicksPerSecond * 60;
    const int64_t TicksPerHour = TicksPerMinute * 60;
    const int64_t TicksPerDay = TicksPerHour * HoursPerDay;

    // 每单位的毫秒数
    const int32_t MillisPerSecond = 1000;
    const int32_t MillisPerMinute = MillisPerSecond * 60;
    const int32_t MillisPerHour = MillisPerMinute * 60;
    const int32_t MillisPerDay = MillisPerHour * HoursPerDay;

    // 每单位的天数
    const int32_t DaysPerYear = 365;
    const int32_t DaysPer4Years = DaysPerYear * 4 + 1;       // 1461
    const int32_t DaysPer100Years = DaysPer4Years * 25 - 1;  // 36524
    const int32_t DaysPer400Years = DaysPer100Years * 4 + 1; // 146097

    const int32_t DaysTo1601 = DaysPer400Years * 4;          // 584388
    const int32_t DaysTo1899 = DaysPer400Years * 4 + DaysPer100Years * 3 - 367;
    const int32_t DaysTo1970 = DaysPer400Years * 4 + DaysPer100Years * 3 + DaysPer4Years * 17 + DaysPerYear; // 719,162
    const int32_t DaysTo10000 = DaysPer400Years * 25 - 366;  // 3652059

    // Tick范围值
    const int64_t MinTicks = 0;
    const int64_t MaxTicks = DaysTo10000 * TicksPerDay - 1;
    const int64_t MaxMicroseconds = MaxTicks / TicksPerMicrosecond;
    const int64_t MaxMillis = MaxTicks / TicksPerMillisecond;
    const int64_t MaxSeconds = MaxTicks / TicksPerSecond;
    const int64_t MaxMinutes = MaxTicks / TicksPerMinute;
    const int64_t MaxHours = MaxTicks / TicksPerHour;
    const int64_t MaxDays = (long)DaysTo10000 - 1;

    const int64_t UnixEpochTicks = DaysTo1970 * TicksPerDay;
    const int64_t FileTimeOffset = DaysTo1601 * TicksPerDay;
    const int64_t DoubleDateOffset = DaysTo1899 * TicksPerDay;

    const uint32_t EafMultiplier = (uint32_t)(((1UL << 32) + DaysPer4Years - 1) / DaysPer4Years);   // 2,939,745
    const uint32_t EafDivider = EafMultiplier * 4;
    const uint64_t TicksPer6Hours = TicksPerHour * 6;
    const int32_t March1BasedDayOfNewYear = 306;              // 3月1日到1月1日

    const uint32_t DaysToMonth365[13] =
    {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
    };

    const uint32_t DaysToMonth366[13] =
    {
        0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
    };

    const uint8_t DaysInMonth365[12] =
    {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    const uint8_t DaysInMonth366[12] =
    {
        31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    const uint64_t TicksMask = 0x3FFFFFFFFFFFFFFF;
    const uint64_t FlagsMask = 0xC000000000000000;
    const int64_t TicksCeiling = 0x4000000000000000;
    const uint64_t KindUnspecified = 0x0000000000000000;
    const uint64_t KindUtc = 0x4000000000000000;
    const uint64_t KindLocal = 0x8000000000000000;
    const uint64_t KindLocalAmbiguousDst = 0xC000000000000000;
    const int32_t KindShift = 62;

    const std::string TicksField = "ticks"; // 序列化用
    const std::string DateDataField = "dateData"; // 序列化用

    const bool s_systemSupportsLeapSeconds = false;

    const DateTime DateTime::MinValue;
    const DateTime DateTime::MaxValue(MaxTicks, DateTimeKind::Unspecified);
    const DateTime DateTime::UnixEpoch(UnixEpochTicks, DateTimeKind::Utc);

    std::out_of_range       _tickOutOfRange("out of ticks range. ");
    std::out_of_range       _millisecondsOutOfRange("out of milliseconds range. ");
    std::invalid_argument   _invalidKind("invalid kind");
    std::invalid_argument   _invalidYearMonthDay("invalid year month day");
    std::invalid_argument   _invalidHourMinuteSecond("invalid hour minute second");

    static uint32_t DaysToYear(uint32_t year)
    {
        uint32_t y = year - 1;
        uint32_t cent = y / 100;
        return y * (365 * 4 + 1) / 4 - cent + cent / 4;
    }

    static uint64_t DateToTicks(int32_t year, int32_t month, int32_t day)
    {
        if (year < 1 || year > 9999 || month < 1 || month > 12 || day < 1)
        {
            throw _invalidYearMonthDay;
        }

        auto& days = DateTime::IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
        if ((uint32_t)day > days[month] - days[month - 1])
        {
            throw _invalidYearMonthDay;
        }

        uint32_t n = DaysToYear((uint32_t)year) + days[month - 1] + (uint32_t)day - 1;
        return n * (uint64_t)TicksPerDay;
    }
    static uint64_t TimeToTicks(int32_t hour, int32_t minute, int32_t second)
    {
        if ((uint32_t)hour >= 24 || (uint32_t)minute >= 60 || (uint32_t)second >= 60)
        {
            throw _invalidHourMinuteSecond;
        }

        int totalSeconds = hour * 3600 + minute * 60 + second;
        return (uint32_t)totalSeconds * (uint64_t)TicksPerSecond;
    }
    static uint64_t Init(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond, DateTimeKind kind = DateTimeKind::Unspecified)
    {
        if ((uint32_t)millisecond >= MillisPerSecond)
        {
            throw  _millisecondsOutOfRange;
        }
        if ((uint32_t)kind > (uint32_t)DateTimeKind::Local)
        {
            throw _invalidKind;
        }
        uint64_t ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
        ticks += (uint32_t)millisecond * (uint32_t)TicksPerMillisecond;
        return ticks | ((uint64_t)kind << KindShift);

    }
    DateTime::DateTime()
    {

    }

    DateTime::DateTime(int64_t ticks)
    {
        auto uticks = static_cast<uint64_t>(ticks);
        if (uticks > MaxTicks)
        {
            throw _tickOutOfRange;
        }
        _dateData = uticks;
    }

    DateTime::DateTime(int64_t ticks, DateTimeKind kind)
    {
        auto uticks = static_cast<uint64_t>(ticks);
        if (uticks > MaxTicks)
        {
            throw _tickOutOfRange;
        }
        auto ukind = static_cast<uint32_t>(kind);
        auto ulocal = static_cast<uint32_t>(DateTimeKind::Local);
        if (ukind > ulocal)
        {
            throw _invalidKind;
        }
        _dateData = uticks | (static_cast<uint64_t>(ukind) << KindShift);
    }

    DateTime::DateTime(int32_t year, int32_t month, int32_t day)
    {
        _dateData = DateToTicks(year, month, day);
    }

    DateTime::DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second)
    {
        _dateData = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
    }

    DateTime::DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, DateTimeKind kind)
    {
        auto ukind = static_cast<uint32_t>(kind);
        _dateData = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second) | (static_cast<uint64_t>(ukind) << KindShift);
    }

    DateTime::DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond)
    {
        _dateData = Init(year, month, day, hour, minute, second, millisecond);
    }

    DateTime::DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond, DateTimeKind kind)
    {
        _dateData = Init(year, month, day, hour, minute, second, millisecond, kind);
    }


    bool DateTime::IsLeapYear(int32_t year)
    {
        if (year < 1 || year > 9999)
        {
            throw std::out_of_range("Year");
        }
        if ((year & 3) != 0)
        {
            return false;
        }
        if ((year & 15) == 0)
        {
            return true;
        }
        return (uint32_t)year % 25 != 0 ? true : false;
    }

    uint64_t InternalKind(uint64_t dateTimeData)
    {
        return dateTimeData & FlagsMask;
    }

    uint64_t InternalTicks(uint64_t dateTimeData)
    {
        return dateTimeData & TicksMask;
    }



    // 纯私函数
    DateTime _Add(const DateTime& dateTime, double value, int32_t scale)
    {
        double millis_double = value * scale + (value >= 0 ? 0.5 : -0.5);
        if (millis_double <= -MaxMillis || millis_double >= MaxMillis)
        {
            throw std::out_of_range("millis");
        }
        return dateTime.AddTicks((int64_t)millis_double * TicksPerMillisecond);

    }

    uint64_t UTicks(const DateTime& dateTime)
    {
        return static_cast<uint64_t>(dateTime.Ticks());
    }

    void GetDate(const DateTime& dateTime, int32_t& year, int32_t& month, int32_t& day)
    {
        // y100 = number of whole 100-year periods since 3/1/0000
            // r1 = (day number within 100-year period) * 4

        auto [y100, r1] = Math::DivRem(((uint32_t)(dateTime.Ticks() / TicksPer6Hours) | 3U) + 1224, DaysPer400Years);
        uint64_t u2 = (uint64_t)Math::BigMul((int)EafMultiplier, (int)r1 | 3);
        ushort daySinceMarch1 = (ushort)((uint)u2 / EafDivider);
        int n3 = 2141 * daySinceMarch1 + 197913;
        year = (int)(100 * y100 + (uint)(u2 >> 32));
        // compute month and day
        month = (ushort)(n3 >> 16);
        day = (ushort)n3 / 2141 + 1;

        // rollover December 31
        if (daySinceMarch1 >= March1BasedDayOfNewYear)
        {
            ++year;
            month -= 12;
        }
    }
    DateTime DateTime::ToLocalTime()
    {
        throw std::runtime_error("no implemented");
        //if ((_dateData & KindLocal) != 0)
        //{
        //    return *this;
        //}
        //bool isAmbiguousLocalDst;
        //int64_t offset = TimeZoneInfo::GetUtcOffsetFromUtc(this, TimeZoneInfo::Local,  _, isAmbiguousLocalDst).Ticks();
        //int64_t tick = Ticks() + offset;
        //if ((uint64_t)tick <= MaxTicks)
        //{
        //    if (!isAmbiguousLocalDst)
        //    {
        //        return DateTime((uint64_t)tick | KindLocal);
        //    }
        //    return DateTime((uint64_t)tick | KindLocalAmbiguousDst);
        //}
        //return DateTime(tick < 0 ? KindLocal : MaxTicks | KindLocal);
    }
    DateTime DateTime::FromFileTime(int64_t fileTime)
    {
        //
        DEV_WARNING("wangkai : todo 稍后再写");
        return FromFileTimeUtc(fileTime);


        // return FromFileTimeUtc(fileTime).ToLocalTime();
    }


    DateTime FromFileTimeLeapSecondsAware_Linux(uint64_t fileTime)
    {
        return {};
    }
    DateTime FromFileTimeLeapSecondsAware_Apple(uint64_t fileTime)
    {
        throw std::invalid_argument("no apple support");
    }

#ifdef _WIN32
    DateTime CreateDateTimeFromSystemTime(SYSTEMTIME time, uint64_t hundredNanoSecond)
    {
        uint32_t year = time.wYear;
        auto& days = DateTime::IsLeapYear((int32_t)year) ? s_daysToMonth366 : s_daysToMonth365;
        int32_t month = time.wMonth - 1;
        uint32_t n = DaysToYear(year) + days[month] + time.wDay - 1;
        uint64_t ticks = n * (uint64_t)TicksPerDay;

        ticks += time.wHour * (uint64_t)TicksPerHour;
        ticks += time.wMinute * (uint64_t)TicksPerMinute;
        uint32_t second = time.wSecond;
        if (second <= 59)
        {
            uint64_t tmp = second * (uint32_t)TicksPerSecond + time.wMilliseconds * (uint32_t)TicksPerMillisecond + hundredNanoSecond;
            return DateTime(ticks + tmp | KindUtc);
        }

        ticks += TicksPerMinute - 1 | KindUtc;
        return DateTime(ticks);
    }
    DateTime FromFileTimeLeapSecondsAware_Windows(uint64_t fileTime)
    {
        SYSTEMTIME time;
        if (::FileTimeToSystemTime((const FILETIME*)&fileTime, &time) == false)
        {
            throw std::invalid_argument("bad file time");
        }
        return CreateDateTimeFromSystemTime(time, fileTime % TicksPerMillisecond);
    }
#endif
    DateTime FromFileTimeLeapSecondsAware(uint64_t fileTime)
    {
#ifdef __linux__
        return FromFileTimeLeapSecondsAware_Linux(fileTime);
#elif _WIN32
        return FromFileTimeLeapSecondsAware_Linux(fileTime);
#elif __APPLE__
        return FromFileTimeLeapSecondsAware_Apple(fileTime);
#endif
    }
    DateTime DateTime::FromFileTimeUtc(int64_t fileTime)
    {
        if ((uint64_t)fileTime > MaxTicks - FileTimeOffset)
        {
            throw std::out_of_range("file time out of range");
        }

        static bool s_systemSupportsLeapSeconds = true;
        if (s_systemSupportsLeapSeconds)
        {
            return FromFileTimeLeapSecondsAware((uint64_t)fileTime);
        }

        uint64_t universalTicks = (uint64_t)fileTime + FileTimeOffset;
        return DateTime(universalTicks | KindUtc);
    }

    DateTime& DateTime::operator=(const DateTime& dateTime)
    {
        _dateData = dateTime._dateData;
        return *this;
    }

    bool DateTime::operator==(const DateTime& dateTime)
    {
        return ((_dateData ^ dateTime._dateData) << 2) == 0;
    }

    bool DateTime::operator!=(const DateTime& dateTime)
    {
        return !(*this == dateTime);
    }
    bool DateTime::operator<(const DateTime& dateTime)
    {
        return Ticks() < dateTime.Ticks();
    }

    bool DateTime::operator<=(const DateTime& dateTime)
    {
        return Ticks() <= dateTime.Ticks();
    }

    bool DateTime::operator>(const DateTime& dateTime)
    {
        return Ticks() > dateTime.Ticks();
    }

    bool DateTime::operator>=(const DateTime& dateTime)
    {
        return Ticks() >= dateTime.Ticks();
    }

    int32_t DateTime::DaysInMonth(int32_t year, int32_t month)
    {
        if (month < 1 || month > 12)
        {
            throw std::invalid_argument("bad month");
        }

        return (IsLeapYear(year) ? DaysInMonth366 : DaysInMonth365)[month - 1];
    }
    DateTime DateTime::SpecifyKind(DateTime value, DateTimeKind kind)
    {
        if ((uint32_t)kind > (uint32_t)DateTimeKind::Local)
        {
            throw std::invalid_argument("bad datetimekind");
        }
        return DateTime(UTicks(value) | ((uint64_t)kind << KindShift));
    }
    DateTime DateTime::UtcNow()
    {
        auto nowTimePoint = std::chrono::system_clock::now();
        auto unixEpochDuration = nowTimePoint.time_since_epoch();
        int64_t ticks = std::chrono::duration_cast<std::chrono::seconds>(unixEpochDuration).count() * TicksPerSecond;
        return DateTime(((uint64_t)(ticks + UnixEpochTicks)) | KindUtc);
    }

    DateTime DateTime::Now()
    {
        bool isAmbiguousLocalDst = true;
        long secondOffset;
        DEV_WARNING("wangkai : todo 需要改为高精度取时器");
#ifdef __linux__

        time_t tt;
        time(&tt);
        tm* currtt = localtime(&tt);
        secondOffset = currtt->tm_gmtoff;
#elif _WIN32

        _get_timezone(&secondOffset);
#endif


        int64_t tickOffset = secondOffset * TicksPerSecond;
        DateTime utc = UtcNow();
        int64_t tick = utc.Ticks() + tickOffset;
        if ((uint64_t)tick <= MaxTicks)
        {
            if (!isAmbiguousLocalDst)
            {
                return DateTime((uint64_t)tick | KindLocal);
            }
            return DateTime((uint64_t)tick | KindLocalAmbiguousDst);
        }
        return DateTime(tick < 0 ? KindLocal : MaxTicks | KindLocal);

    }
    DateTimeKind DateTime::Kind() const
    {
        auto internalKind = InternalKind(_dateData);
        if (internalKind == KindUnspecified)
        {
            return DateTimeKind::Unspecified;
        }
        else if (internalKind == KindUtc)
        {
            return DateTimeKind::Utc;
        }
        else
        {
            return DateTimeKind::Local;
        }
    }

    DateTime DateTime::Add(TimeSpan value) const
    {
        return AddTicks(value.Ticks());
    }

    DateTime DateTime::AddTicks(int64_t value) const
    {
        uint64_t ticks = (uint64_t)(Ticks() + value);
        if (ticks > MaxTicks)
        {
            throw std::out_of_range("bigger than max ticks ");
        }
        return DateTime(ticks | InternalKind(_dateData));
    }

    DateTime DateTime::AddYears(double value) const
    {
        if (value < -10000 || value > 10000)
        {
            throw std::out_of_range("bad years");
        }
        int32_t year, month, day;
        GetDate(*this, year, month, day);
        int y = year + value;
        if (y < 1 || y > 9999)
        {
            throw std::out_of_range("bad year");
        }
        uint32_t n = DaysToYear((uint32_t)y);

        int m = month - 1, d = day - 1;
        if (IsLeapYear(y))
        {
            n += DaysToMonth366[m];
        }
        else
        {
            if (d == 28 && m == 1) d--;
            n += DaysToMonth365[m];
        }
        n += (uint32_t)d;
        return DateTime(n * (uint64_t)TicksPerDay + UTicks(*this) % TicksPerDay | InternalKind(_dateData));
    }

    DateTime DateTime::AddMonths(double months) const
    {
        if (months < -120000 || months > 120000)
        {
            throw std::invalid_argument("bad months");
        }

        int32_t year, month, day;
        GetDate(*this, year, month, day);
        int32_t y = year, d = day;
        int32_t m = month + months;
        int32_t q = m > 0 ? (int32_t)((uint32_t)(m - 1) / 12) : m / 12 - 1;
        y += q;
        m -= q * 12;
        if (y < 1 || y > 9999)
        {
            throw std::invalid_argument("bad year");
        }
        auto daysTo = IsLeapYear(y) ? DaysToMonth366 : DaysToMonth365;
        uint32_t daysToMonth = daysTo[m - 1];
        int32_t days = (int32_t)(daysTo[m] - daysToMonth);
        if (d > days)
        {
            d = days;
        }
        uint32_t n = DaysToYear((uint32_t)y) + daysToMonth + (uint32_t)d - 1;
        return DateTime(n * (uint64_t)TicksPerDay + UTicks(*this) % TicksPerDay | InternalKind(_dateData));
    }

    DateTime DateTime::AddDays(double value) const
    {
        return _Add(*this, value, MillisPerDay);
    }

    DateTime DateTime::AddHours(double value) const
    {
        return _Add(*this, value, MillisPerHour);
    }

    DateTime DateTime::AddMinutes(double value) const
    {
        return _Add(*this, value, MillisPerMinute);
    }

    DateTime DateTime::AddSeconds(double value) const
    {
        return _Add(*this, value, MillisPerSecond);
    }

    DateTime DateTime::AddMilliseconds(double value) const
    {
        return _Add(*this, value, 1);
    }

    DateTime DateTime::AddTicks(double value) const
    {
        uint64_t ticks = (uint64_t)(Ticks() + value);
        if (ticks > MaxTicks)
        {
            throw std::out_of_range("bad ticks");
        }
        return DateTime(ticks | InternalKind(_dateData));
    }

    int64_t DateTime::Ticks() const
    {
        return (int64_t)(_dateData & TicksMask);
    }

    int32_t DateTime::Year() const
    {
        auto [y100, r1] = Math::DivRem(((uint32_t)(UTicks(*this) / TicksPer6Hours) | 3U), DaysPer400Years);
        return 1 + (int)(100 * y100 + (r1 | 3) / DaysPer4Years);
    }

    int32_t DateTime::Month() const
    {
        // r1 = (day number within 100-year period) * 4
        uint32_t r1 = (((uint32_t)(UTicks(*this) / TicksPer6Hours) | 3U) + 1224) % DaysPer400Years;
        ulong u2 = (ulong)Math::BigMul((int)EafMultiplier, (int)r1 | 3);
        ushort daySinceMarch1 = (ushort)((uint32_t)u2 / EafDivider);
        int n3 = 2141 * daySinceMarch1 + 197913;
        return (ushort)(n3 >> 16) - (daySinceMarch1 >= March1BasedDayOfNewYear ? 12 : 0);
    }

    int32_t DateTime::Day() const
    {
        // r1 = (day number within 100-year period) * 4
        uint32_t r1 = (((uint32_t)(UTicks(*this) / TicksPer6Hours) | 3U) + 1224) % DaysPer400Years;
        ulong u2 = (ulong)Math::BigMul((int)EafMultiplier, (int)r1 | 3);
        ushort daySinceMarch1 = (ushort)((uint32_t)u2 / EafDivider);
        int n3 = 2141 * daySinceMarch1 + 197913;
        // Return 1-based day-of-month
        return (ushort)n3 / 2141 + 1;
    }

    int32_t DateTime::Hour() const
    {
        return (int32_t)((uint32_t)(UTicks(*this) / TicksPerHour) % 24);
    }

    int32_t DateTime::Minute() const
    {
        return (int32_t)((UTicks(*this) / TicksPerMinute) % 60);
    }

    int32_t DateTime::Second() const
    {
        return (int32_t)((UTicks(*this) / TicksPerSecond) % 60);
    }

    int32_t DateTime::Millisecond() const
    {
        return (int32_t)((UTicks(*this) / TicksPerMillisecond) % 1000);
    }

    DateTime DateTime::Date() const
    {
        uint64_t uticks = UTicks(*this);
        return DateTime((uticks - uticks % TicksPerDay) | InternalKind(_dateData));
    }

    DayOfWeek DateTime::DayOfWeek() const
    {
        return static_cast<Nuke::System::DayOfWeek>((((uint32_t)(UTicks(*this) / TicksPerDay) + 1) % 7));
    }

    int32_t DateTime::DayOfYear() const
    {
        return 1 + (int32_t)(((((uint32_t)(UTicks(*this) / TicksPer6Hours) | 3U) % (uint32_t)DaysPer400Years) | 3U) * EafMultiplier / EafDivider);
    }

    TimeSpan DateTime::TimeOfDay() const
    {
        return TimeSpan((int64_t)(UTicks(*this) % TicksPerDay));
    }

    TimeSpan DateTime::Subtract(DateTime value)
    {
        return TimeSpan(Ticks() - value.Ticks());
    }

    DateTime DateTime::Subtract(TimeSpan value)
    {
        uint64_t ticks = (uint64_t)(Ticks() - value._ticks);
        if (ticks > MaxTicks)
        {
            throw std::out_of_range("bad ticks");
        }
        return DateTime(ticks | InternalKind(_dateData));
    }

    DateTime DateTime::Today()
    {
        return Now().Date();
    }

    std::string DateTime::ToString() const
    {
        // 默认yyyy-MM-dd HH:mm::ss
        char timeString[32] = { 0 };
        sprintf(timeString, "%04d-%02d-%02d %02d:%02d:%02d:%03d",
            Year(), Month(), Day(),
            Hour(), Minute(), Second(),
            Millisecond());
        return timeString;
    }


}