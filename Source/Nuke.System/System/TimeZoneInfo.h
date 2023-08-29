
#pragma once

#include <System/DateTime.h>
#include <System/AdjustmentRule.h>

namespace Nuke::System
{
    /// <summary>
    /// 表示世界上的任何时区
    /// </summary>
    class TimeZoneInfo
    {
    public:
        static TimeZoneInfo Utc();
        static TimeZoneInfo Local();
        static std::vector<TimeZoneInfo> GetSystemTimeZones(); 
        static TimeZoneInfo FindSystemTimeZoneById(std::string id);

    public:
        std::string StandardName();
        std::string Id();
        bool HasIanaId();
        std::string DisplayName();
        std::string DaylightName();
        TimeSpan BaseUtcOffset();
        bool SupportsDaylightSavingTime();

        static void ClearCachedData();
        //static DateTimeOffset ConvertTime(DateTimeOffset dateTimeOffset, TimeZoneInfo destinationTimeZone);
        static DateTime ConvertTime(DateTime dateTime, TimeZoneInfo sourceTimeZone, TimeZoneInfo destinationTimeZone);
        static DateTime ConvertTime(DateTime dateTime, TimeZoneInfo destinationTimeZone);
        //static DateTimeOffset ConvertTimeBySystemTimeZoneId(DateTimeOffset dateTimeOffset, std::string destinationTimeZoneId);
        static DateTime ConvertTimeBySystemTimeZoneId(DateTime dateTime, std::string sourceTimeZoneId, std::string destinationTimeZoneId);
        static DateTime ConvertTimeBySystemTimeZoneId(DateTime dateTime, std::string destinationTimeZoneId);
        static DateTime ConvertTimeFromUtc(DateTime dateTime, TimeZoneInfo destinationTimeZone);
        static DateTime ConvertTimeToUtc(DateTime dateTime);
        static DateTime ConvertTimeToUtc(DateTime dateTime, TimeZoneInfo sourceTimeZone);
        // static TimeZoneInfo CreateCustomTimeZone(std::string id, TimeSpan baseUtcOffset, std::string displayName, std::string standardDisplayName);
        // static TimeZoneInfo CreateCustomTimeZone(std::string id, TimeSpan baseUtcOffset, std::string displayName, std::string standardDisplayName, std::string daylightDisplayName, const std::vector<AdjustmentRule>& adjustmentRules, bool disableDaylightSavingTime);
        // static TimeZoneInfo CreateCustomTimeZone(std::string id, TimeSpan baseUtcOffset, std::string displayName, std::string standardDisplayName, std::string daylightDisplayName, const std::vector<AdjustmentRule>& adjustmentRules);
        
        //static TimeZoneInfo FromSerializedString(std::string source);
        //static bool TryConvertIanaIdToWindowsId(std::string ianaId, std::string& windowsId);
        //static bool TryConvertWindowsIdToIanaId(std::string windowsId, std::string& ianaId);
        //static bool TryConvertWindowsIdToIanaId(std::string windowsId, std::string region, std::string& ianaId);
        bool Equals(const TimeZoneInfo& other);
        std::vector<AdjustmentRule> GetAdjustmentRules();
        //TimeSpan[] GetAmbiguousTimeOffsets(DateTimeOffset dateTimeOffset);
        //std::vector<TimeSpan> GetAmbiguousTimeOffsets(DateTime dateTime);
        
        TimeSpan GetUtcOffset(DateTime dateTime);
        //TimeSpan GetUtcOffset(DateTimeOffset dateTimeOffset);
        bool HasSameRules(const TimeZoneInfo& other);
        //bool IsAmbiguousTime(DateTimeOffset dateTimeOffset);
        //bool IsAmbiguousTime(DateTime dateTime);
        //bool IsDaylightSavingTime(DateTimeOffset dateTimeOffset);
        bool IsDaylightSavingTime(DateTime dateTime);
        bool IsInvalidTime(DateTime dateTime);
        //std::string ToSerializedString();
        //std::string ToString();
        ~TimeZoneInfo();
    private:
        TimeZoneInfo();
        TimeZoneInfo(const TimeZoneInfo& timeZoneInfo);
        //TimeZoneInfo(TimeZoneInfo&&) = delete;
        //~TimeZoneInfo();

        friend class TimeZoneInfoInternals;
        class TimeZoneInfoInternals;
        TimeZoneInfoInternals* internals;
    };
}