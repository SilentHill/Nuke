
#pragma once

#include <_pch.h>
#include <System/TimeSpan.h>

namespace Nuke::System
{
    /// <summary>
    /// 时间种类
    /// </summary>
    enum class DateTimeKind
    {
        Unspecified = 0,	// 未指定
        Utc = 1,			// UTC时间
        Local = 2,			// 本地时间
    };

    /// <summary>
    /// 周N
    /// </summary>
    enum class DayOfWeek
    {
        Sunday		= 0,	// 周日
        Monday		= 1,	// 周一
        Tuesday		= 2,	// 周二
        Wednesday	= 3,	// 周三
        Thursday	= 4,	// 周四
        Friday		= 5,	// 周五
        Saturday	= 6,	// 周六
    };

    /// <summary>
    /// 日期时间
    /// </summary>
    class DateTime final
    {
    public:
        /*
           构造析构
        */ 
        DateTime();
        DateTime(int64_t ticks);
        DateTime(int64_t ticks, DateTimeKind kind);
        DateTime(int32_t year, int32_t month, int32_t day);
        DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second);
        DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, DateTimeKind kind);
        DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond);
        DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond, DateTimeKind kind);
        DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond, int32_t microsecond);
        DateTime(int32_t year, int32_t month, int32_t day, int32_t hour, int32_t minute, int32_t second, int32_t millisecond, int32_t microsecond, DateTimeKind kind);

        DateTime(const DateTime&) = default;
        DateTime(DateTime&&) = default;
        ~DateTime() = default;

    public: // 静态兼容构造器
        static DateTime FromFileTime(int64_t fileTime);
        static DateTime FromFileTimeUtc(int64_t fileTime);

    public: // 赋值比较操作符
        DateTime& operator=(const DateTime& dateTime);
        bool operator==(const DateTime& dateTime);
        bool operator!=(const DateTime& dateTime);
        bool operator<(const DateTime& dateTime);
        bool operator<=(const DateTime& dateTime);
        bool operator>(const DateTime& dateTime);
        bool operator>=(const DateTime& dateTime);
        
    public:
        /*
            静态常值
        */
        static const DateTime MinValue;
        static const DateTime MaxValue;
        static const DateTime UnixEpoch;

    public:
        /*
            静态属性
        */
        /// <summary>
        /// 获取一个 DateTime 对象，该对象设置为此计算机上的当前日期和时间，表示为协调通用时间(UTC)。 
        /// </summary>
        /// <returns></returns>
        static DateTime Now(); 

        /// <summary>
        /// 获取一个 DateTime 对象，该对象设置为此计算机上的当前日期和时间，表示为本地时间。
        /// </summary>
        /// <returns></returns>
        static DateTime UtcNow();
        
        /// <summary>
        /// 获取当前日期
        /// </summary>
        /// <returns></returns>
        static DateTime Today();

    public:
        // 静态方法
        // 返回指定年和月中的天数
        static int32_t DaysInMonth(int32_t year, int32_t month);
        
        // 返回指定的年份是否为闰年的指示
        static bool IsLeapYear(int32_t year);

        // 创建新的 DateTime 对象，该对象具有与指定的 DateTime 相同的刻度数，但是根据指定的 DateTimeKind 值的指示，指定为本地时间或协调世界时 (UTC)，或者两者皆否。
        static DateTime SpecifyKind(DateTime value, DateTimeKind kind);

    public: // 属性：时间部件
        DateTime Date() const;			// 纯日期副本（时间为0）
        int32_t Year() const;			// 年
        int32_t Month() const;			// 月
        int32_t Day() const;			// 日
        int32_t Hour() const;			// 时
        int32_t Minute() const;			// 分
        int32_t Second() const;			// 秒
        int32_t Millisecond() const;	// 毫秒
        int64_t Ticks() const;			// 计时周期
        enum DayOfWeek DayOfWeek() const;	// 星期几
        int32_t DayOfYear() const;		// 该年中的第几天
        TimeSpan TimeOfDay() const;		// 当天已经过的时间段
        TimeSpan Subtract(DateTime value);	// 返回一个新的 TimeSpan，从此实例的值中减去指定的日期和时间。
        DateTime Subtract(TimeSpan value);	// 返回一个新的 DateTime，从此实例的值中减去指定持续时间。

    public: // 属性方法
        DateTimeKind Kind() const;

    public: // 方法
        DateTime Add(TimeSpan value) const;
        DateTime AddYears(double value) const;
        DateTime AddMonths(double value) const;
        DateTime AddDays(double value) const;
        DateTime AddHours(double value) const;
        DateTime AddMinutes(double value) const;
        DateTime AddSeconds(double value) const;
        DateTime AddMilliseconds(double value) const;
        DateTime AddTicks(double value) const;
        DateTime AddTicks(int64_t value) const;

        DateTime ToLocalTime();

        std::string ToString() const; // yyyy-MM-dd HH:mm::ss

    private:
        /// <summary>
        /// 存储
        /// </summary>
        uint64_t _dateData;
    };
}