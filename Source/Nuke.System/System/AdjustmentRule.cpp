
#include "_pch.h"
#include "Math.h"
#include "AdjustmentRule.h"

namespace Nuke::System
{
    const TimeSpan DaylightDeltaAdjustment = TimeSpan::FromHours(24.0);
    const TimeSpan MaxDaylightDelta = TimeSpan::FromHours(12.0);

    class AdjustmentRule::AdjustmentRuleInternals
    {
    public:
        AdjustmentRuleInternals(const TransitionTime& daylightTransitionStart, const TransitionTime& daylightTransitionEnd)
            :_daylightTransitionStart(daylightTransitionStart), _daylightTransitionEnd(daylightTransitionEnd)
        {

        }
        DateTime _dateStart;
        DateTime _dateEnd;
        TimeSpan _daylightDelta;
        TransitionTime _daylightTransitionStart;
        TransitionTime _daylightTransitionEnd;
        TimeSpan _baseUtcOffsetDelta;   // delta from the default Utc offset (utcOffset = defaultUtcOffset + _baseUtcOffsetDelta)
        bool _noDaylightTransitions;

    };

    AdjustmentRule::AdjustmentRule(
        DateTime dateStart,
        DateTime dateEnd,
        TimeSpan daylightDelta,
        const TransitionTime& daylightTransitionStart,
        const TransitionTime& daylightTransitionEnd,
        TimeSpan baseUtcOffsetDelta,
        bool noDaylightTransitions)
    {
        internals = new AdjustmentRule::AdjustmentRuleInternals(daylightTransitionStart, daylightTransitionEnd);
        internals->_dateStart = dateStart;
        internals->_dateStart = dateEnd;
        internals->_daylightDelta = daylightDelta;
        internals->_baseUtcOffsetDelta = baseUtcOffsetDelta;
        internals->_noDaylightTransitions = noDaylightTransitions;
    }

    AdjustmentRule::~AdjustmentRule()
    {
        delete internals;
    }

    TimeSpan AdjustmentRule::BaseUtcOffsetDelta() const
    {
        return internals->_baseUtcOffsetDelta;
    }

    DateTime AdjustmentRule::DateEnd() const
    {
        return internals->_dateEnd;
    }

    DateTime AdjustmentRule::DateStart() const
    {
        return internals->_dateStart;
    }

    TimeSpan AdjustmentRule::DaylightDelta() const
    {
        return internals->_daylightDelta;
    }

    TransitionTime AdjustmentRule::DaylightTransitionEnd() const
    {
        return internals->_daylightTransitionEnd;
    }

    TransitionTime AdjustmentRule::DaylightTransitionStart() const
    {
        return internals->_daylightTransitionStart;
    }

    bool AdjustmentRule::HasDaylightSaving() const
    {
        return DaylightDelta() != TimeSpan::Zero ||
            (DaylightTransitionStart().TimeOfDay() != DateTime::MinValue) ||
            (DaylightTransitionEnd().TimeOfDay() != DateTime::MinValue.AddMilliseconds(1));
    }

    static void AdjustDaylightDeltaToExpectedRange(TimeSpan& daylightDelta, TimeSpan& baseUtcOffsetDelta)
    {
        if (daylightDelta > MaxDaylightDelta)
        {
            daylightDelta = daylightDelta - DaylightDeltaAdjustment;
            baseUtcOffsetDelta = baseUtcOffsetDelta + DaylightDeltaAdjustment;
        }
        else if (daylightDelta < -MaxDaylightDelta)
        {
            daylightDelta = daylightDelta + DaylightDeltaAdjustment;
            baseUtcOffsetDelta = baseUtcOffsetDelta -DaylightDeltaAdjustment;
        }

        //std::assert(daylightDelta <= MaxDaylightDelta && daylightDelta >= -MaxDaylightDelta, "DaylightDelta should not ever be more than 24h");
    }
    AdjustmentRule AdjustmentRule::CreateAdjustmentRule(
        DateTime dateStart,
        DateTime dateEnd,
        TimeSpan daylightDelta,
        const TransitionTime& daylightTransitionStart,
        const TransitionTime& daylightTransitionEnd,
        TimeSpan baseUtcOffsetDelta)
    {
        return AdjustmentRule(
            dateStart,
            dateEnd,
            daylightDelta,
            daylightTransitionStart,
            daylightTransitionEnd,
            baseUtcOffsetDelta,
            false);
    }

    AdjustmentRule AdjustmentRule::CreateAdjustmentRule(
        DateTime dateStart,
        DateTime dateEnd,
        TimeSpan daylightDelta,
        const TransitionTime& daylightTransitionStart,
        const TransitionTime& daylightTransitionEnd)
    {
        return AdjustmentRule(
            dateStart,
            dateEnd,
            daylightDelta,
            daylightTransitionStart,
            daylightTransitionEnd,
            TimeSpan::Zero,
            false);
    }

    AdjustmentRule AdjustmentRule::CreateAdjustmentRule(
        DateTime dateStart,
        DateTime dateEnd,
        TimeSpan daylightDelta,
        const TransitionTime& daylightTransitionStart,
        const TransitionTime& daylightTransitionEnd,
        TimeSpan baseUtcOffsetDelta,
        bool noDaylightTransitions)
    {
        AdjustDaylightDeltaToExpectedRange(daylightDelta, baseUtcOffsetDelta);
        return AdjustmentRule(
            dateStart,
            dateEnd,
            daylightDelta,
            daylightTransitionStart,
            daylightTransitionEnd,
            baseUtcOffsetDelta,
            noDaylightTransitions);
    }

    bool AdjustmentRule::Equals(const AdjustmentRule& other) const
    {
        return internals->_dateStart == other.internals->_dateStart &&
            internals->_dateEnd == other.internals->_dateEnd &&
            internals->_daylightDelta == other.internals->_daylightDelta &&
            internals->_baseUtcOffsetDelta == other.internals->_baseUtcOffsetDelta &&
            internals->_daylightTransitionEnd.Equals(other.internals->_daylightTransitionEnd) &&
            internals->_daylightTransitionStart.Equals(other.internals->_daylightTransitionStart);
    }
}