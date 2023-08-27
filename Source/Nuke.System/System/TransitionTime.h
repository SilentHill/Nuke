
#pragma once

#include <System/DateTime.h>

namespace Nuke::System
{
    class TransitionTime
    {
    public:
        int Day();
        DayOfWeek DayOfWeek();
        bool IsFixedDateRule();
        int Month();
        DateTime TimeOfDay();
        int Week();
        static TransitionTime CreateFixedDateRule(DateTime timeOfDay, int month, int day);
        static TransitionTime CreateFloatingDateRule(DateTime timeOfDay, int month, int week, System::DayOfWeek dayOfWeek);
        bool Equals(const TransitionTime& other);

    private:
        TransitionTime();
        ~TransitionTime();
        class TransitionTimeInternals;
        TransitionTimeInternals* internals;
    };

    static bool operator ==(TransitionTime t1, TransitionTime t2);
    static bool operator !=(TransitionTime t1, TransitionTime t2);
}