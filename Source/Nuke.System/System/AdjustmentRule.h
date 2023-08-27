
#pragma once

#include <System/DateTime.h>
#include <System/TransitionTime.h>

namespace Nuke::System
{
    class AdjustmentRule
    {
    public:
        TimeSpan BaseUtcOffsetDelta();
        DateTime DateEnd();
        DateTime DateStart();
        TimeSpan DaylightDelta();
        TransitionTime DaylightTransitionEnd();
        TransitionTime DaylightTransitionStart();
        static AdjustmentRule CreateAdjustmentRule(DateTime dateStart, DateTime dateEnd, TimeSpan daylightDelta, TransitionTime daylightTransitionStart, TransitionTime daylightTransitionEnd);
        static AdjustmentRule CreateAdjustmentRule(DateTime dateStart, DateTime dateEnd, TimeSpan daylightDelta, TransitionTime daylightTransitionStart, TransitionTime daylightTransitionEnd, TimeSpan baseUtcOffsetDelta);
        bool Equals(const AdjustmentRule& other);
    };
}