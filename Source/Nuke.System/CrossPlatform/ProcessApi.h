
#pragma once 

#include <System/DateTime.h>
#include <System/TimeSpan.h>

namespace Nuke::CrossPlatform::ProcessApi
{
    enum class ThreadState
    {
        Initialized,
        Ready,
        Running,
        Standby,
        Terminated,
        Wait,
        Transition,
        Unknown
    };

    enum class ThreadWaitReason
    {
        Executive,
        FreePage,
        PageIn,
        SystemAllocation,
        ExecutionDelay,
        Suspended,
        UserRequest,
        EventPairHigh,
        EventPairLow,
        LpcReceive,
        LpcReply,
        VirtualMemory,
        PageOut,
        Unknown
    };

    class ThreadInfo
    {
    public:
        uint64_t    _threadId;
        int32_t     _processId;
        int32_t     _basePriority;
        int32_t     _currentPriority;
        void* _startAddress;
        ThreadState _threadState;
        ThreadWaitReason _threadWaitReason;
    };

    class ProcessInfo
    {
    public:
        int32_t     BasePriority;
        std::string ProcessName;
        int32_t     ProcessId;
        int64_t     PoolPagedBytes;
        int64_t     PoolNonPagedBytes;
        int64_t     VirtualBytes;
        int64_t     VirtualBytesPeak;
        int64_t     WorkingSetPeak;
        int64_t     WorkingSet;
        int64_t     PageFileBytesPeak;
        int64_t     PageFileBytes;
        int64_t     PrivateBytes;
        int32_t     SessionId;
        int32_t     HandleCount;
        std::vector<ThreadInfo> _threadInfoList;
    };

    int32_t GetCurrentProcessId();

    std::vector<int32_t> GetProcessIds();

    std::vector<ProcessInfo> GetProcessInfos();


    using namespace Nuke::System;

    class ProcessThreadTimes
    {
    public:
        DateTime StartTime() { return DateTime::FromFileTime(_create); }
        DateTime ExitTime() { return DateTime::FromFileTime(_exit); }
        TimeSpan PrivilegedProcessorTime() { return TimeSpan(_kernel); }
        TimeSpan UserProcessorTime() { return TimeSpan(_user); }
        TimeSpan TotalProcessorTime() { return TimeSpan(_user + _kernel); }
    public:
        int64_t _create;
        int64_t _exit;
        int64_t _kernel;
        int64_t _user;

    };

    TimeSpan GetTotalProcessorTime(int32_t pid);

}