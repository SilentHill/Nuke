
#pragma once

#include "TimeSpan.h"

namespace Nuke::System
{
	// 时间种类
	enum class DateTimeKind
	{
		Unspecified = 0,	// 未指定
		Utc = 1,			// UTC时间
		Local = 2,			// 本地时间
	};

	// 星期
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

	// 
	// class DateTime 
	// 表示时间上的一刻，通常以日期和当天的时间表示
	// 同时支持UTC时间和本机时间
	// 本类是只读类，不支持 / 请勿继承
	// 
	class DateTime final
	{
	public: // 构造析构
		DateTime();
		DateTime(uint64_t dateData);
		DateTime(int64_t ticks);
		DateTime(int64_t ticks, DateTimeKind kind);
		DateTime(int32_t year, int32_t month, int day);
		DateTime(const DateTime&) = default;
		DateTime(DateTime&&) = default;
		~DateTime();

	public: // 静态构造
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
		
	public: // 字段：静态边界对象
		static const DateTime MinValue;
		static const DateTime MaxValue;
		static const DateTime UnixEpoch;

	public: // 属性：静态构造

		// 获取一个 DateTime 对象，该对象设置为此计算机上的当前日期和时间，表示为协调通用时间(UTC)。
		static DateTime Now(); 

		// 获取一个 DateTime 对象，该对象设置为此计算机上的当前日期和时间，表示为本地时间。
		static DateTime UtcNow();

		// 获取当前日期
		static DateTime Today();

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

		// Todo: 等完成TimeZoneInfo后再实现
		// 判断是否在当前时区的夏时制范围内
		// bool IsDaylightSavingTime();	

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

	private: // 私有
		// 此处未使用impl的考虑到性能和单一成员类型清洁且标准
		uint64_t _dateData;
	};
}