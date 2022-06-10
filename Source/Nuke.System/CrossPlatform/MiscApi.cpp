
#include "../_pch.h"
#include "MiscApi.h"

namespace Nuke::CrossPlatform::MiscApi
{

#ifdef __linux__
    int64_t QueryPerformanceFrequency_Linux()
    {
        const int64_t SecondsToNanoSeconds = 1000000000;
        return SecondsToNanoSeconds;
    }
    int64_t QueryPerformanceCounter_Linux()
    {
        timespec ts;
        int32_t result = clock_gettime(CLOCK_MONOTONIC, &ts);
        return ((uint64_t)(ts.tv_sec) * QueryPerformanceFrequency_Linux()) + (uint64_t)(ts.tv_nsec);
    }
#elif _WIN32
    int64_t QueryPerformanceFrequency_Windows()
    {
        return ::_Query_perf_frequency();
    }
    int64_t QueryPerformanceCounter_Windows()
    {
        return ::_Query_perf_counter();
    }
#endif

    int64_t Crossplatform_QueryPerformanceFrequency()
    {
#ifdef __linux__
        return QueryPerformanceFrequency_Linux();
#elif _WIN32
        return QueryPerformanceFrequency_Windows();
#endif
    }

    int64_t Crossplatform_QueryPerformanceCounter()
    {
#ifdef __linux__
        return QueryPerformanceCounter_Linux();
#elif _WIN32
        return QueryPerformanceCounter_Windows();
#endif
    }
}