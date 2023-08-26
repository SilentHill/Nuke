

#ifdef __APPLE__

#include <bits/stdc++.h>
#include "TimeZoneApi.h"

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
        return {};
    }

    std::vector<std::string> GetTimeZoneIds()
    {
        return {};
    }
}
#endif