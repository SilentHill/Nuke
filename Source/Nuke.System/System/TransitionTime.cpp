
#include "_pch.h"
#include "Math.h"
#include "TransitionTime.h"

namespace Nuke::System
{
    class TransitionTime::TransitionTimeInternals
    {
    public:
        DateTime _timeOfDay;
        int8_t _month;
        int8_t _week;
        int8_t _day;
        System::DayOfWeek _dayOfWeek;
        bool _isFixedDateRule;
    };

    static void ValidateTransitionTime(const DateTime& timeOfDay, int month, int week, int day, DayOfWeek dayOfWeek)
    {
        if (timeOfDay.Kind() != DateTimeKind::Unspecified)
        {
            throw std::invalid_argument("timeOfDay");
        }

        // Month range 1-12
        if (month < 1 || month > 12)
        {
            throw std::out_of_range("month");
        }

        // Day range 1-31
        if (day < 1 || day > 31)
        {
            throw std::out_of_range("day");
        }

        // Week range 1-5
        if (week < 1 || week > 5)
        {
            throw std::out_of_range("week");
        }

        // DayOfWeek range 0-6
        if ((int)dayOfWeek < 0 || (int)dayOfWeek > 6)
        {
            throw std::out_of_range("dayOfWeek");
        }

        timeOfDay.GetDate(out int timeOfDayYear, out int timeOfDayMonth, out int timeOfDayDay);
        if (timeOfDayYear != 1 || timeOfDayMonth != 1 || timeOfDayDay != 1 || (timeOfDay.Ticks() % TimeSpan::TicksPerMillisecond != 0))
        {
            throw std::invalid_argument("timeOfDay");
        }
    }

    TransitionTime::TransitionTime()
    {
        internals = new TransitionTimeInternals();
    }

    TransitionTime::~TransitionTime()
    {
        delete internals;
    }

    int TransitionTime::Day()
    {
        return internals->_day;
    }

    DayOfWeek TransitionTime::DayOfWeek()
    {
        return internals->_dayOfWeek;
    }

    bool TransitionTime::IsFixedDateRule()
    {
        return internals->_isFixedDateRule;
    }

    int TransitionTime::Month()
    {
        return internals->_month;
    }

    DateTime TransitionTime::TimeOfDay()
    {
        return internals->_timeOfDay;
    }

    int TransitionTime::Week()
    {
        return internals->_week;
    }

    TransitionTime TransitionTime::CreateFixedDateRule(DateTime timeOfDay, int month, int day)
    {
        return TransitionTime();
    }
    TransitionTime TransitionTime::CreateFloatingDateRule(DateTime timeOfDay, int month, int week, System::DayOfWeek dayOfWeek)
    {
        return TransitionTime();
    }
    bool TransitionTime::Equals(const TransitionTime& other)
    {
        auto isEqual = internals->_isFixedDateRule == other.internals->_isFixedDateRule &&
            internals->_timeOfDay == other.internals->_timeOfDay &&
            internals->_month == other.internals->_month &&
            (other.internals->_isFixedDateRule ?
                internals->_day == other.internals->_day :
                internals->_week == other.internals->_week && internals->_dayOfWeek == other.internals->_dayOfWeek);
        return isEqual;
    }
    bool operator==(TransitionTime t1, TransitionTime t2)
    {
        return t1.Equals(t2);
    }
    bool operator!=(TransitionTime t1, TransitionTime t2)
    {
        return !t1.Equals(t2);
    }
}