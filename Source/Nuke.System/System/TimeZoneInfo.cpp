
#include "_pch.h"
#include "Math.h"

#include <System/AdjustmentRule.h>
#include "TimeZoneInfo.h"

namespace Nuke::System
{
    class TimeZoneInfo::TimeZoneInfoInternals
    {
    public:
        std::string _id;
        std::string _displayName;
        std::string _standardDisplayName;
        std::string _daylightDisplayName;
        TimeSpan _baseUtcOffset;
        bool _supportsDaylightSavingTime;
        std::vector<AdjustmentRule> _adjustmentRules;
    };
    const TimeSpan MaxOffset = TimeSpan::FromHours(14.0);
    const TimeSpan MinOffset = -MaxOffset;

    TimeZoneInfo::TimeZoneInfo()
    {

    }
    static bool UtcOffsetOutOfRange(TimeSpan offset)
    {
        return offset < MinOffset || offset > MaxOffset;
    }
    
    static TimeSpan GetUtcOffset(const TimeSpan& baseUtcOffset, const AdjustmentRule& adjustmentRule)
    {
        return baseUtcOffset + adjustmentRule.BaseUtcOffsetDelta + (adjustmentRule.HasDaylightSaving ? adjustmentRule.DaylightDelta : TimeSpan.Zero);
    }

    static bool IsValidAdjustmentRuleOffset(const TimeSpan& baseUtcOffset, const AdjustmentRule& adjustmentRule)
    {
        TimeSpan utcOffset = GetUtcOffset(baseUtcOffset, adjustmentRule);
        return !UtcOffsetOutOfRange(utcOffset);
    }
    static void ValidateTimeZoneInfo(std::string id, TimeSpan baseUtcOffset, std::vector<AdjustmentRule> adjustmentRules, bool& adjustmentRulesSupportDst)
    {
        if (id.empty())
        {
            throw std::invalid_argument(id);
        }

        if (UtcOffsetOutOfRange(baseUtcOffset))
        {
            throw std::out_of_range("baseUtcOffset");
        }

        if (baseUtcOffset.Ticks() % TimeSpan::TicksPerMinute != 0)
        {
            throw std::invalid_argument("baseUtcOffset");
        }

        adjustmentRulesSupportDst = false;

        if (!adjustmentRules.empty())
        {
            adjustmentRulesSupportDst = true;
            AdjustmentRule current;
            for (int i = 0; i < adjustmentRules.size(); i++)
            {
                AdjustmentRule prev = current;
                current = adjustmentRules[i];

                // if (current == null)
                // {
                //     throw new InvalidTimeZoneException(SR.Argument_AdjustmentRulesNoNulls);
                // }

                if (!IsValidAdjustmentRuleOffset(baseUtcOffset, current))
                {
                    throw new InvalidTimeZoneException(SR.ArgumentOutOfRange_UtcOffsetAndDaylightDelta);
                }

                if (prev != null && current.DateStart <= prev.DateEnd)
                {
                    // verify the rules are in chronological order and the DateStart/DateEnd do not overlap
                    throw new InvalidTimeZoneException(SR.Argument_AdjustmentRulesOutOfOrder);
                }
            }
        }
    }

    TimeZoneInfo CreateTimeZoneInfo(
        const std::string& id,
        TimeSpan baseUtcOffset,
        const std::string& displayName,
        const std::string& standardDisplayName,
        const std::string& daylightDisplayName,
        const std::vector<AdjustmentRule> adjustmentRules,
        bool disableDaylightSavingTime,
        bool hasIanaId = false)
    {
        TimeZoneInfo timeZoneInfo;
        ValidateTimeZoneInfo(id, baseUtcOffset, adjustmentRules, out bool adjustmentRulesSupportDst);

        timeZoneInfo._id = id;
        timeZoneInfo._baseUtcOffset = baseUtcOffset;
        timeZoneInfo._displayName = displayName ? ? string.Empty;
        timeZoneInfo._standardDisplayName = standardDisplayName ? ? string.Empty;
        timeZoneInfo._daylightDisplayName = disableDaylightSavingTime ? string.Empty : daylightDisplayName ? ? string.Empty;
        timeZoneInfo._supportsDaylightSavingTime = adjustmentRulesSupportDst && !disableDaylightSavingTime;
        timeZoneInfo._adjustmentRules = adjustmentRules;

        HasIanaId = _id.Equals(UtcId, StringComparison.OrdinalIgnoreCase) ? true : hasIanaId;
    }

    static std::string GetUtcStandardDisplayName()
    {
        // // Don't bother looking up the name for invariant or English cultures
        // CultureInfo uiCulture = CultureInfo.CurrentUICulture;
        // if (GlobalizationMode.Invariant || uiCulture.Name.Length == 0 || uiCulture.TwoLetterISOLanguageName == "en")
        // {
        //     return InvariantUtcStandardDisplayName;
        // }
        // 
        // // Try to get a localized version of "Coordinated Universal Time" from the globalization data
        // std::string standardDisplayName;
        // GetDisplayName(UtcId, Interop.Globalization.TimeZoneDisplayNameType.Standard, uiCulture.Name, ref standardDisplayName);
        // 
        // // Final safety check.  Don't allow null or abbreviations
        // if (standardDisplayName == null || standardDisplayName == "GMT" || standardDisplayName == "UTC")
        // {
        //     standardDisplayName = InvariantUtcStandardDisplayName;
        // }
        // 
        // return standardDisplayName;
        return {};
    }
    const std::string UtcId = "UTC";
    const std::string LocalId = "Local";
    static std::string GetUtcFullDisplayName(const std::string& timeZoneId, const std::string& standardDisplayName)
    {
        return "(UTC) " + standardDisplayName;
    }
    static TimeZoneInfo CreateCustomTimeZone(const std::string& id, TimeSpan baseUtcOffset, std::string displayName, std::string standardDisplayName)
    {
        bool hasIanaId = TryConvertIanaIdToWindowsId(id, allocate: false, out _);

        return TimeZoneInfo(
            id,
            baseUtcOffset,
            displayName,
            standardDisplayName,
            standardDisplayName,
            adjustmentRules: null,
            disableDaylightSavingTime : false,
            hasIanaId);
    }
    static TimeZoneInfo CreateUtcTimeZone()
    {
        std::string standardDisplayName = GetUtcStandardDisplayName();
        std::string displayName = GetUtcFullDisplayName(UtcId, standardDisplayName);
        return CreateCustomTimeZone(UtcId, TimeSpan.Zero, displayName, standardDisplayName);
    }

    static TimeZoneInfo s_utcTimeZone = CreateUtcTimeZone();

    class CachedData
    {
    private:
        TimeZoneInfo _localTimeZone = CreateLocal();

        TimeZoneInfo CreateLocal()
        {

            TimeZoneInfo timeZone = _localTimeZone;
            if (timeZone == null)
            {
                timeZone = GetLocalTimeZone(this);

                // this step is to break the reference equality
                // between TimeZoneInfo.Local and a second time zone
                // such as "Pacific Standard Time"
                timeZone = new TimeZoneInfo(
                    timeZone._id,
                    timeZone._baseUtcOffset,
                    timeZone.DisplayName,
                    timeZone.StandardName,
                    timeZone.DaylightName,
                    timeZone._adjustmentRules,
                    disableDaylightSavingTime: false,
                    timeZone.HasIanaId);

                _localTimeZone = timeZone;
            }
            return timeZone;
        }

    public:
        TimeZoneInfo Local = _localTimeZone;

        /// <summary>
        /// Helper function that returns the corresponding DateTimeKind for this TimeZoneInfo.
        /// </summary>
        DateTimeKind GetCorrespondingKind(const TimeZoneInfo& timeZone)
        {
            auto timeZonePtr = &timeZone;

            if (timeZonePtr == s_utcTimeZone)
            {
                return DateTimeKind::Utc;
            }
            else if (timeZonePtr == &_localTimeZone)
            {
                return DateTimeKind::Local;
            }
            else
            {
                return DateTimeKind::Unspecified;
            }
        }

        std::unordered_map<std::string, TimeZoneInfo> _systemTimeZones;
        std::vector<TimeZoneInfo> _readOnlySystemTimeZones;
        std::vector<TimeZoneInfo> _readOnlyUnsortedSystemTimeZones;
        std::unordered_map<std::string, TimeZoneInfo> _timeZonesUsingAlternativeIds;
        bool _allSystemTimeZonesRead;
    };

    enum class TimeZoneInfoResult
    {
        Success = 0,
        TimeZoneNotFoundException = 1,
        InvalidTimeZoneException = 2,
        SecurityException = 3
    };

    TimeZoneInfoResult TryGetTimeZone(const std::string& id, bool dstDisabled, TimeZoneInfo& value, std::exception& e, CachedData cachedData, bool alwaysFallbackToLocalMachine = false)
    {
        TimeZoneInfoResult result = TryGetTimeZoneUsingId(id, dstDisabled, out value, out e, cachedData, alwaysFallbackToLocalMachine);
        if (result != TimeZoneInfoResult.Success)
        {
            string ? alternativeId = GetAlternativeId(id, out bool idIsIana);
            if (alternativeId != null)
            {
                result = TryGetTimeZoneUsingId(alternativeId, dstDisabled, out value, out e, cachedData, alwaysFallbackToLocalMachine);
                if (result == TimeZoneInfoResult.Success)
                {
                    TimeZoneInfo ? zone = null;
                    if (value!._equivalentZones == null)
                    {
                        zone = new TimeZoneInfo(id, value!._baseUtcOffset, value!._displayName, value!._standardDisplayName,
                            value!._daylightDisplayName, value!._adjustmentRules, dstDisabled && value!._supportsDaylightSavingTime, idIsIana);
                        value!._equivalentZones = new List<TimeZoneInfo>();
                        lock(value!._equivalentZones)
                        {
                            value!._equivalentZones.Add(zone);
                        }
                    }
                    else
                    {
                        foreach(TimeZoneInfo tzi in value!._equivalentZones)
                        {
                            if (tzi.Id == id)
                            {
                                zone = tzi;
                                break;
                            }
                        }
                        if (zone == null)
                        {
                            zone = new TimeZoneInfo(id, value!._baseUtcOffset, value!._displayName, value!._standardDisplayName,
                                value!._daylightDisplayName, value!._adjustmentRules, dstDisabled && value!._supportsDaylightSavingTime, idIsIana);
                            lock(value!._equivalentZones)
                            {
                                value!._equivalentZones.Add(zone);
                            }
                        }
                    }

                    cachedData._timeZonesUsingAlternativeIds ? ? = new Dictionary<string, TimeZoneInfo>(StringComparer.OrdinalIgnoreCase);
                    cachedData._timeZonesUsingAlternativeIds[id] = zone;

                    Debug.Assert(zone != null);
                    value = zone;
                }
            }
        }

        return result;
    }
}