
#ifdef _WIN32

#include "../_pch.h"
#include "ProcessApi.h"
#include <System/String.h>
#include <sdkddkver.h>
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#define WINVER _WIN32_WINNT_WIN7
#include <Windows.h>
#include <psapi.h>
#include <winternl.h>
using namespace Nuke::System;


namespace Nuke::CrossPlatform::ProcessApi
{
    int32_t GetCurrentProcessId()
    {
        static int32_t processId = ::GetCurrentProcessId();
        return processId;
    }
    std::vector<int32_t> GetProcessIds()
    {
        std::vector<int32_t> processIds;
        processIds.resize(1024);
        DWORD needed;
        while (true)
        {
            if (!EnumProcesses((DWORD*)(processIds.data()), processIds.size(), &needed))
            {
                throw std::runtime_error("bad EnumProcesses()");
            }

            if (needed == processIds.size())
            {
                processIds.resize(processIds.size() * 2);
                continue;
            }

            break;
        }
        return processIds;
    }

    const uint32_t STATUS_INFO_LENGTH_MISMATCH = 0xC0000004;

    uint32_t GetEstimatedBufferSize(uint32_t actualSize)
    {
        return actualSize + 1024 * 10;
    }

    int32_t SystemProcessID()
    {
        const int32_t systemProcessIDOnXP = 4;
        return systemProcessIDOnXP;
    }
    const int32_t IdleProcessID = 0;

    struct UNICODE_STRING
    {
        uint16_t Length;
        uint16_t MaximumLength;
        wchar_t* Buffer;
    };
    
    struct SYSTEM_PROCESS_INFORMATION
    {
        uint32_t NextEntryOffset;
        uint32_t NumberOfThreads;
        uint8_t Reserved1[48];
        UNICODE_STRING ImageName;
        KPRIORITY BasePriority;
        HANDLE UniqueProcessId;
        void* Reserved2;
        uint32_t HandleCount;
        uint32_t SessionId;
        void* Reserved3;
        size_t PeakVirtualSize;
        size_t VirtualSize;
        uint32_t Reserved4;
        size_t PeakWorkingSetSize;
        size_t WorkingSetSize;
        void* Reserved5;
        size_t QuotaPagedPoolUsage;
        void* Reserved6;
        size_t QuotaNonPagedPoolUsage;
        size_t PagefileUsage;
        size_t PeakPagefileUsage;
        size_t PrivatePageCount;
        LARGE_INTEGER Reserved7[6];
    };

    struct SYSTEM_THREAD_INFORMATION
    {
        LARGE_INTEGER Reserved1[3];
        uint32_t Reserved2;
        void* StartAddress;
        CLIENT_ID ClientId;
        KPRIORITY Priority;
        int32_t BasePriority;
        uint32_t Reserved3;
        uint32_t ThreadState;
        uint32_t WaitReason;
    };

    std::string GetProcessShortName(const std::string& name)
    {
        if (name.empty())
        {
            return name;
        }

        int32_t slash = -1;
        int32_t period = -1;

        for (int32_t i = 0; i < name.size(); i++)
        {
            if (name[i] == '\\')
            {
                slash = i;
            }
            else if (name[i] == '.')
            {
                period = i;
            }
        }

        if (period == -1)
        {
            period = name.size() - 1;
        }
        else
        {
            std::string extension = name.substr(period);

            if (_stricmp(extension.c_str(), ".exe") == 0)
            {
                period--;
            }
            else
            {
                period = name.size() - 1;
            }
        }

        if (slash == -1)
        {
            slash = 0;
        }
        else
        {
            slash++;
        }
        return name.substr(slash, period - slash + 1);
    }
    ThreadWaitReason GetThreadWaitReason(int value)
    {
        switch (value)
        {
        case 0:
        case 7: return ThreadWaitReason::Executive;
        case 1:
        case 8: return ThreadWaitReason::FreePage;
        case 2:
        case 9: return ThreadWaitReason::PageIn;
        case 3:
        case 10: return ThreadWaitReason::SystemAllocation;
        case 4:
        case 11: return ThreadWaitReason::ExecutionDelay;
        case 5:
        case 12: return ThreadWaitReason::Suspended;
        case 6:
        case 13: return ThreadWaitReason::UserRequest;
        case 14: return ThreadWaitReason::EventPairHigh;
        case 15: return ThreadWaitReason::EventPairLow;
        case 16: return ThreadWaitReason::LpcReceive;
        case 17: return ThreadWaitReason::LpcReply;
        case 18: return ThreadWaitReason::VirtualMemory;
        case 19: return ThreadWaitReason::PageOut;
        default: return ThreadWaitReason::Unknown;
        }
    }
    std::vector<ProcessInfo> _GetProcessInfos(const std::vector<uint8_t>& data)
    {
        std::map<int32_t, ProcessInfo> processInfosMap;

        int processInformationOffset = 0;

        while (true)
        {
            SYSTEM_PROCESS_INFORMATION& pi = *(SYSTEM_PROCESS_INFORMATION*)(data.data() + processInformationOffset);


            ProcessInfo processInfo;
            processInfo.ProcessId = reinterpret_cast<int32_t>(pi.UniqueProcessId);
            processInfo.SessionId = (int32_t)pi.SessionId;
            processInfo.PoolPagedBytes = (int64_t)pi.QuotaPagedPoolUsage;
            processInfo.PoolNonPagedBytes = (int64_t)pi.QuotaNonPagedPoolUsage;
            processInfo.VirtualBytes = (int64_t)pi.VirtualSize;
            processInfo.VirtualBytesPeak = (int64_t)pi.PeakVirtualSize;
            processInfo.WorkingSetPeak = (int64_t)pi.PeakWorkingSetSize;
            processInfo.WorkingSet = (int64_t)pi.WorkingSetSize;
            processInfo.PageFileBytesPeak = (int64_t)pi.PeakPagefileUsage;
            processInfo.PageFileBytes = (int64_t)pi.PagefileUsage;
            processInfo.PrivateBytes = (int64_t)pi.PrivatePageCount;
            processInfo.BasePriority = pi.BasePriority;
            processInfo.HandleCount = (int32_t)pi.HandleCount;

            if (pi.ImageName.Buffer == nullptr)
            {
                if (processInfo.ProcessId == SystemProcessID())
                {
                    processInfo.ProcessName = "System";
                }
                else if (processInfo.ProcessId == IdleProcessID)
                {
                    processInfo.ProcessName = "Idle";
                }
                else
                {
                    // 普通进程没有名字，用ID代替了
                    processInfo.ProcessName = std::to_string(processInfo.ProcessId);
                }
            }
            else
            {
                std::wstring imageName;
                imageName.assign(pi.ImageName.Buffer, (size_t)(pi.ImageName.Length/sizeof(wchar_t)));
                std::string processName = GetProcessShortName(String::ConvertWstringToString(imageName));
                processInfo.ProcessName = processName;
            }

            // 获取当前进程
            processInfosMap[processInfo.ProcessId] = processInfo;

            int threadInformationOffset = processInformationOffset + sizeof(SYSTEM_PROCESS_INFORMATION);
            for (int i = 0; i < pi.NumberOfThreads; i++)
            {
                SYSTEM_THREAD_INFORMATION& ti = *(SYSTEM_THREAD_INFORMATION*)(data.data() + threadInformationOffset);
                ThreadInfo threadInfo;
                threadInfo._processId = (int32_t)ti.ClientId.UniqueProcess;
                threadInfo._threadId = (uint64_t)ti.ClientId.UniqueThread;
                threadInfo._basePriority = ti.BasePriority;
                threadInfo._currentPriority = ti.Priority;
                threadInfo._startAddress = ti.StartAddress;
                threadInfo._startAddress = ti.StartAddress;
                threadInfo._threadState = (ThreadState)ti.ThreadState;
                threadInfo._threadWaitReason = GetThreadWaitReason((int)ti.WaitReason);
                processInfo._threadInfoList.push_back(threadInfo);
                threadInformationOffset += sizeof(SYSTEM_THREAD_INFORMATION);
            }

            if (pi.NextEntryOffset == 0)
            {
                break;
            }
            processInformationOffset += (int)pi.NextEntryOffset;
        }

        std::vector<ProcessInfo> temp;
        std::transform(processInfosMap.begin(), processInfosMap.end(), std::back_inserter(temp),
            [](const std::map<int32_t, ProcessInfo>::value_type& value) { return value.second;  });

        return temp;
    }

    NTSTATUS DynamicInvokeNtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass,
        PVOID SystemInformation,
        ULONG SystemInformationLength,
        PULONG ReturnLength)
    {
        typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
        typedef NTSTATUS(*NtQuerySystemInformationType)(SYSTEM_INFORMATION_CLASS, PVOID, ULONG, PULONG);
        HINSTANCE hinstLib;
        BOOL fFreeResult, fRunTimeLinkSuccess = FALSE;
        hinstLib = ::LoadLibrary("Ntdll.dll");
        if (hinstLib != nullptr)
        {
            NtQuerySystemInformationType procNtQuery = (NtQuerySystemInformationType)GetProcAddress(hinstLib, "NtQuerySystemInformation");
            if (procNtQuery != nullptr)
            {
                fRunTimeLinkSuccess = true;
                procNtQuery(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
            }
            fFreeResult = FreeLibrary(hinstLib);
        }
        return 0;
    }

    std::vector<uint8_t> NtQuerySystemInformation_SystemProcessInformation()
    {
        uint32_t bufferSize = 1024 * 1024;
        uint32_t actualSize = 0;
        std::vector<uint8_t> buffer(bufferSize);
        auto status = DynamicInvokeNtQuerySystemInformation(SYSTEM_INFORMATION_CLASS::SystemProcessInformation, buffer.data(), bufferSize, reinterpret_cast<PULONG>(&actualSize));

        if (status != STATUS_INFO_LENGTH_MISMATCH)
        {
            if ((int32_t)status < 0)
            {
                throw std::runtime_error("NtQuerySystemInformation failed.");
            }

            buffer.resize(actualSize);
            bufferSize = GetEstimatedBufferSize(actualSize);
            return buffer;
        }
    }


    std::vector<ProcessInfo> GetProcessInfos()
    {
        auto sysInfobuffer = NtQuerySystemInformation_SystemProcessInformation();
        auto pinfos = _GetProcessInfos(sysInfobuffer);
        return pinfos;
    }

    TimeSpan GetTotalProcessorTime(int32_t pid)
    {
        HANDLE procHandle = ::OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
        ProcessThreadTimes processThreadTimes;
        ::GetProcessTimes(procHandle,
            reinterpret_cast<FILETIME*>(&processThreadTimes._create) ,
            reinterpret_cast<FILETIME*>(&processThreadTimes._exit),
            reinterpret_cast<FILETIME*>(&processThreadTimes._kernel),
            reinterpret_cast<FILETIME*>(&processThreadTimes._user));
        ::CloseHandle(procHandle);
        return processThreadTimes.TotalProcessorTime();
    }
}


#endif