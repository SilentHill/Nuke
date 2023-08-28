
#include "_pch.h"
#include "Math.h"

#include <System/AdjustmentRule.h>
#include "TimeZoneInfo.h"

namespace Nuke::System
{
    const std::string UtcId = "UTC";
    const std::string LocalId = "Local";

    enum class TimeZoneInfoOptions
    {
        None = 1,
        NoThrowOnInvalidTime = 2
    };

    enum class TimeZoneInfoResult
    {
        Success = 0,
        TimeZoneNotFoundException = 1,
        InvalidTimeZoneException = 2,
        SecurityException = 3
    };

    const int32_t MaxKeyLength = 255;

    class TimeZoneInfo::TimeZoneInfoInternals
    {
    public:
        std::string _id;
        std::string _displayName;
        std::string _standardDisplayName;
        std::string _daylightDisplayName;
        TimeSpan _baseUtcOffset;
        bool _supportsDaylightSavingTime;
        std::vector<AdjustmentRule>  _adjustmentRules;
        std::vector<TimeZoneInfo> _equivalentZones;
        static TimeZoneInfo CreateUtcTimeZone()
        {
            auto utcTimeZone = TimeZoneInfo::CreateCustomTimeZone(
                UtcId,
                TimeSpan::Zero,
                "(UTC) Coordinated Universal Time",
                "Coordinated Universal Time",
                "Coordinated Universal Time",
                {},
                false);
            return utcTimeZone;
        }
    };

    //static TimeZoneInfo s_utcTimeZone = TimeZoneInfo::TimeZoneInfoInternals::CreateUtcTimeZone();

    class CachedData
    {

    };

    static CachedData s_cachedData;
    

    TimeZoneInfo::TimeZoneInfo()
    {
        TimeZoneInfo::TimeZoneInfoInternals::CreateUtcTimeZone();
        internals = new TimeZoneInfoInternals();
    }

    TimeZoneInfo::~TimeZoneInfo()
    {
        delete internals;
    }

    TimeZoneInfo TimeZoneInfo::Utc()
    {
        return TimeZoneInfo();
    }

    TimeZoneInfo TimeZoneInfo::CreateCustomTimeZone(
        std::string id, 
        TimeSpan baseUtcOffset, 
        std::string displayName, 
        std::string standardDisplayName)
    {
        return CreateCustomTimeZone(id, baseUtcOffset, displayName, standardDisplayName, "", {}, false);
    }

    TimeZoneInfo TimeZoneInfo::CreateCustomTimeZone(
        std::string id,
        TimeSpan baseUtcOffset,
        std::string displayName,
        std::string standardDisplayName,
        std::string daylightDisplayName,
        const std::vector<AdjustmentRule>& adjustmentRules)
    {
        return CreateCustomTimeZone(id, baseUtcOffset, displayName, standardDisplayName, daylightDisplayName, adjustmentRules, false);
    }

    TimeZoneInfo TimeZoneInfo::CreateCustomTimeZone(
        std::string id,
        TimeSpan baseUtcOffset,
        std::string displayName,
        std::string standardDisplayName,
        std::string daylightDisplayName,
        const std::vector<AdjustmentRule>& adjustmentRules,
        bool disableDaylightSavingTime)
    {
        TimeZoneInfo timeZoneInfo;
        timeZoneInfo.internals->_id = id;
        timeZoneInfo.internals->_baseUtcOffset = baseUtcOffset;
        timeZoneInfo.internals->_displayName = displayName;
        timeZoneInfo.internals->_standardDisplayName = standardDisplayName;
        timeZoneInfo.internals->_daylightDisplayName = disableDaylightSavingTime ? "" : daylightDisplayName;
        timeZoneInfo.internals->_adjustmentRules = adjustmentRules;
        return timeZoneInfo;
    }
}
