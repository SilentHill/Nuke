

#include "TimeZoneApi.h"

#ifdef __linux__
    #include <unistd.h>
#endif

namespace Nuke::CrossPlatform::TimeZoneApi
{
    void GetAllTimeZones()
    {
        tzset();
        auto timezone_names = tzname;
        long numTimezones = sysconf(_SC_TZNAME_MAX);
        

        char* timezoneNames[numTimezones];
        
        std::vector<std::string> timeZones;
        for (long i = 0; i < numTimezones; ++i) 
        {
            timeZones.push_back(tzname[i]);
            std::cout << tzname[i] << std::endl;
        }
    }
}