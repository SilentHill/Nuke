
#pragma once

#include <System/DateTime.h>

namespace Nuke::System
{
    /// <summary>
    /// 提供有关特定时区中特定时间更改（例如从夏时制更改为标准时间，或者从标准时间更改为夏时制）的信息
    /// </summary>
    class TransitionTime
    {
    public:
        int Day() const;
        DayOfWeek DayOfWeek() const;
        bool IsFixedDateRule() const;
        int Month() const;
        DateTime TimeOfDay() const;
        int Week() const;
        static TransitionTime CreateFixedDateRule(DateTime timeOfDay, int month, int day);
        static TransitionTime CreateFloatingDateRule(DateTime timeOfDay, int month, int week, System::DayOfWeek dayOfWeek);
        bool Equals(const TransitionTime& other) const;

    private:
        friend class AdjustmentRule;
        friend class TimeZoneInfo;
        TransitionTime();
        TransitionTime(DateTime timeOfDay, int32_t month, int32_t week, int32_t day, System::DayOfWeek dayOfWeek, bool isFixedDateRule);
        ~TransitionTime();
        TransitionTime(const TransitionTime& trainsitionTime) = default;
        TransitionTime(TransitionTime&& trainsitionTime) = default;
        class TransitionTimeInternals;
        TransitionTimeInternals* internals;
    };

    static bool operator ==(TransitionTime t1, TransitionTime t2);
    static bool operator !=(TransitionTime t1, TransitionTime t2);
}