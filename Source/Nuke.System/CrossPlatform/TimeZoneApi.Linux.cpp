

#include <bits/stdc++.h>
#include "TimeZoneApi.h"

#ifdef __linux__
#include <unistd.h>
#endif

namespace Nuke::CrossPlatform::TimeZoneApi
{
    const std::string TimeZoneFileName = "zone.tab";

    // 如果设置了此环境变量，则该环境变量的时区数据库路径比系统默认路径优先级更高
    const std::string TimeZoneDirectoryEnvironmentVariable = "TZDIR";

    // 如果设置了此环境变量，则该环境变量时区比系统当前时区优先级更高
    const std::string TimeZoneEnvironmentVariable = "TZ";

    // 默认系统时区数据库路径
    const std::string DefaultTimeZoneDirectory = "/usr/share/zoneinfo/";

    static std::string GetTimeZoneDirectory()
    {
        auto tzDirectoryPtr = ::getenv(TimeZoneDirectoryEnvironmentVariable.c_str());
        std::string tzDirectory;

        if (tzDirectoryPtr == nullptr)
        {
            tzDirectory = DefaultTimeZoneDirectory;
        }
        else
        {
            tzDirectory = tzDirectoryPtr;
        }

        // 保证目录分隔符必须存在
        if (tzDirectory.back() != '/')
        {
            tzDirectory += '/';
        }

        return tzDirectory;
    }

    std::vector<std::string> ParseTimeZoneIds(std::istream& reader)
    {
        std::vector<std::string> timeZoneIds;

        std::string zoneTabFileLine;

        while (!std::getline(reader, zoneTabFileLine).eof())
        {
            if (!zoneTabFileLine.empty() && zoneTabFileLine[0] != '#')
            {
                // 行格式 "ISO 3166 领地码 \t coordinates \t TimeZone Id \t comments"

                int firstTabIndex = zoneTabFileLine.find('\t');
                               if (firstTabIndex >= 0)
                {
                    int secondTabIndex = zoneTabFileLine.find('\t', firstTabIndex + 1);
                    if (secondTabIndex >= 0)
                    {
                        std::string timeZoneId;
                        int startIndex = secondTabIndex + 1;
                        int thirdTabIndex = zoneTabFileLine.find('\t', startIndex);
                        if (thirdTabIndex >= 0)
                        {
                            int length = thirdTabIndex - startIndex;
                            timeZoneId = zoneTabFileLine.substr(startIndex, length);
                        }
                        else
                        {
                            timeZoneId = zoneTabFileLine.substr(startIndex);
                        }
                        
                        if (!timeZoneId.empty())
                        {
                            timeZoneIds.emplace_back(timeZoneId);
                        }
                    }
                }
            }
        }

        return timeZoneIds;
    }

    std::vector<std::string> GetTimeZoneIds()
    {
        using namespace std::filesystem;
        auto zoneTabPath = path(GetTimeZoneDirectory()) / TimeZoneFileName;
        std::ifstream streamReader(zoneTabPath);
        ParseTimeZoneIds(streamReader);

    }
}