
#include "_pch.h"
#include "Math.h"

#include <System/AdjustmentRule.h>
#include <System/String.h>
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

        }
        std::string _id;
        std::string _displayName;
        std::string _standardDisplayName;
        std::string _daylightDisplayName;
        TimeSpan _baseUtcOffset;
        bool _supportsDaylightSavingTime;
        std::vector<AdjustmentRule>  _adjustmentRules;


        static std::vector<TimeZoneInfo> _loadTimeZoneInfos();
        static std::vector<TimeZoneInfo> _timeZoneInfos;
    };

    TimeZoneInfo::TimeZoneInfo(TimeZoneInfoInternals* internals)
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

    }



    std::vector<TimeZoneInfo> TimeZoneInfo::TimeZoneInfoInternals::_timeZoneInfos = TimeZoneInfo::TimeZoneInfoInternals::_loadTimeZoneInfos();

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

    std::vector<TimeZoneInfo> TimeZoneInfo::TimeZoneInfoInternals::_loadTimeZoneInfos()
    {
        const std::string& fullJsonString = CrossPlatform::TimeZoneApi::GetTimeZoneDataBaseJsonString();
        auto timeZoneJson = nlohmann::json::parse(fullJsonString);

        std::vector<TimeZoneInfo> allTimeZoneInfos;
        for (const auto& timeZoneJsonElement : timeZoneJson)
        {
            // std::string _id;
            // std::string _displayName;
            // std::string _standardDisplayName;
            // std::string _daylightDisplayName;
            // TimeSpan _baseUtcOffset;
            // bool _supportsDaylightSavingTime;
            // std::vector<AdjustmentRule>  _adjustmentRules;

            auto timeZoneInfoInternals = new TimeZoneInfo::TimeZoneInfoInternals;
            timeZoneInfoInternals->_id = timeZoneJsonElement.at("Id").get<std::string>();
            timeZoneInfoInternals->_displayName = timeZoneJsonElement.at("DisplayName").get<std::string>();
            timeZoneInfoInternals->_standardDisplayName = timeZoneJsonElement.at("DisplayName").get<std::string>();
            timeZoneInfoInternals->_daylightDisplayName = timeZoneJsonElement.at("DaylightName").get<std::string>();

            auto timeSpanParts = String::Split(timeZoneJsonElement.at("BaseUtcOffset").get<std::string>(), ":");

            auto hour = std::stoi(timeSpanParts[0]);
            auto minute = std::stoi(timeSpanParts[1]);
            auto second = std::stoi(timeSpanParts[2]);

            timeZoneInfoInternals->_baseUtcOffset = TimeSpan(hour, minute, second);
            timeZoneInfoInternals->_supportsDaylightSavingTime = timeZoneJsonElement.at("SupportsDaylightSavingTime").get<bool>();
            timeZoneInfoInternals->_daylightDisplayName = timeZoneJsonElement.at("DaylightName").get<std::string>();

            auto& adjustmentRuleJsons = timeZoneJsonElement.at("AdjustmentRules");
            for (const auto& adjustmentRuleJson : adjustmentRuleJsons)
            {
                for (const auto& adjustmentRuleJsonElement : adjustmentRuleJson)
                {
                    auto dateStartValue = adjustmentRuleJsonElement.at("DateStart").get<std::string>();
                    auto dateEndValue = adjustmentRuleJsonElement.at("DateEnd").get<std::string>();
                    continue;
                }
                    /*
                        DateTime dateStart,
                        DateTime dateEnd,
                        TimeSpan daylightDelta,
                        const TransitionTime& daylightTransitionStart,
                        const TransitionTime& daylightTransitionEnd,
                        TimeSpan baseUtcOffsetDelta,
                        bool noDaylightTransitions
                    */

            }



            //  timeZoneInfoInternals->_adjustmentRules = element["AdjustmentRules"].get<std::vector<AdjustmentRule>>();

            allTimeZoneInfos.push_back(TimeZoneInfo(timeZoneInfoInternals));
        }
        return {};
    }
}
