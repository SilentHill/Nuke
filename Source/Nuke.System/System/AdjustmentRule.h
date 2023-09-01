
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
        TimeSpan BaseUtcOffsetDelta() const;
        DateTime DateEnd() const;
        DateTime DateStart() const;
        TimeSpan DaylightDelta() const;
        TransitionTime DaylightTransitionEnd() const;
        TransitionTime DaylightTransitionStart() const;
        bool HasDaylightSaving() const; // todo privated
        static AdjustmentRule CreateAdjustmentRule(DateTime dateStart, DateTime dateEnd, TimeSpan daylightDelta, const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd);
        static AdjustmentRule CreateAdjustmentRule(DateTime dateStart, DateTime dateEnd, TimeSpan daylightDelta, const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd, TimeSpan baseUtcOffsetDelta);
        static AdjustmentRule CreateAdjustmentRule(DateTime dateStart, DateTime dateEnd, TimeSpan daylightDelta, const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd, TimeSpan baseUtcOffsetDelta, bool noDaylightTransitions);
        bool Equals(const AdjustmentRule& other) const;
        AdjustmentRule(const AdjustmentRule&) = default;
        ~AdjustmentRule();

    private:
        friend class TimeZoneInfo;
        AdjustmentRule() = default;
        AdjustmentRule(
            DateTime dateStart,
            DateTime dateEnd,
            TimeSpan daylightDelta,
            const TransitionTime& daylightTransitionStart,
            const TransitionTime& daylightTransitionEnd,
            TimeSpan baseUtcOffsetDelta,
            bool noDaylightTransitions);
        class AdjustmentRuleInternals;
        AdjustmentRuleInternals* internals;
    };
}