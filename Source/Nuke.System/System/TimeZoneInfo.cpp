
#include "_pch.h"
#include "Math.h"

#include <System/AdjustmentRule.h>
#include "TimeZoneInfo.h"
#include "CrossPlatform/TimeZoneApi.h"
#include "CrossPlatform/json.hpp"

namespace Nuke::System
{
    class TimeZoneInfo::TimeZoneInfoInternals
    {
    public:
        TimeZoneInfoInternals()
        {
            if (_timeZoneInfos.empty())
            {
                _timeZoneInfos = _loadTimeZoneInfos();
            }
        }
        std::string _id;
        std::string _displayName;
        std::string _standardDisplayName;
        std::string _daylightDisplayName;
        TimeSpan _baseUtcOffset;
        bool _supportsDaylightSavingTime;
        std::vector<AdjustmentRule>  _adjustmentRules;

        static std::vector<TimeZoneInfo> _loadTimeZoneInfos()
        {
            const std::string& fullJson = CrossPlatform::TimeZoneApi::GetTimeZoneDataBaseJsonString();
            auto j = nlohmann::json::parse(fullJson);

            std::vector<TimeZoneInfo> allTimeZoneInfos;
            for (const auto element : j)
            {
                auto timeZoneInfoInternals = new TimeZoneInfo::TimeZoneInfoInternals;
                element["StandardName"].get<std::string>();

                allTimeZoneInfos.push_back(TimeZoneInfo(timeZoneInfoInternals));
            }
            return {};
        }
        static std::vector<TimeZoneInfo> _timeZoneInfos;
    };
    
    TimeZoneInfo::TimeZoneInfo(TimeZoneInfoInternals * internals)
    {
        this->internals = internals;
    }
    
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


    //static TimeZoneInfo s_utcTimeZone = TimeZoneInfo::TimeZoneInfoInternals::CreateUtcTimeZone();

    class CachedData
    {
    public:
        static std::vector<TimeZoneInfo> AllTimeZoneInfos;
    };

    static CachedData s_cachedData;


    TimeZoneInfo::TimeZoneInfo()
    {
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

    TimeZoneInfo TimeZoneInfo::Local()
    {
        return TimeZoneInfo();
    }

    std::vector<TimeZoneInfo> TimeZoneInfo::GetSystemTimeZones()
    {
        return TimeZoneInfo::TimeZoneInfoInternals::_timeZoneInfos;
    }

}
