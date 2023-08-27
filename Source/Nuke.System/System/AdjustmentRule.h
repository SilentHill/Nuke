
#pragma once

#include <System/DateTime.h>
#include <System/TransitionTime.h>

namespace Nuke::System
{
    /// <summary>
    /// 提供有关时区调整的信息，例如与夏令时的相互转换
    /// </summary>
    class AdjustmentRule
    {
    public:
        TimeSpan BaseUtcOffsetDelta();
        DateTime DateEnd();
        DateTime DateStart();
        TimeSpan DaylightDelta();
        TransitionTime DaylightTransitionEnd();
        TransitionTime DaylightTransitionStart();
        static AdjustmentRule CreateAdjustmentRule(DateTime dateStart, DateTime dateEnd, TimeSpan daylightDelta, const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd);
        static AdjustmentRule CreateAdjustmentRule(DateTime dateStart, DateTime dateEnd, TimeSpan daylightDelta, const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd, TimeSpan baseUtcOffsetDelta);
        static AdjustmentRule CreateAdjustmentRule(DateTime dateStart, DateTime dateEnd, TimeSpan daylightDelta, const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd, TimeSpan baseUtcOffsetDelta, bool noDaylightTransitions);
        bool Equals(const AdjustmentRule& other);

    private:
        AdjustmentRule(
            DateTime dateStart,
            DateTime dateEnd,
            TimeSpan daylightDelta,
            const TransitionTime& daylightTransitionStart,
            const TransitionTime& daylightTransitionEnd,
            TimeSpan baseUtcOffsetDelta,
            bool noDaylightTransitions);
        ~AdjustmentRule();
        class AdjustmentRuleInternals;
        AdjustmentRuleInternals* internals;
    };
}