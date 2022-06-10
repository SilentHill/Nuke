
#ifdef __linux__

#include "../_pch.h"
#include <unistd.h>
#include "ProcessApi.h"
#include <System/String.h>
#include <System/IO/File.h>
#include <dirent.h>

namespace Nuke::CrossPlatform::ProcessApi
{
    namespace LinuxProcessConstants
    {
        const std::string RootPath = "/proc/";
        const std::string StatusFileName = "/status";
        const std::string ExeFileName = "/exe";
        const std::string CmdLineFileName = "/cmdline";
        const std::string StatFileName = "/stat";
        const std::string FileDescriptorDirectoryName = "/fd/";
        const std::string TaskDirectoryName = "/task/";

        const std::string SelfExeFilePath = RootPath + "self" + ExeFileName;
        const std::string SelfCmdLineFilePath = RootPath + "self" + CmdLineFileName;
        const std::string ProcStatFilePath = RootPath + "stat";
    }
    int32_t GetCurrentProcessId()
    {
        static int32_t processId = getpid();
        return processId;
    }
    std::string GetProcessSubFile(int32_t pid, const std::string& subFileName)
    {
        char pathBuffer[256];
        sprintf(pathBuffer, "%s%d%s", LinuxProcessConstants::RootPath.c_str(), pid, subFileName.c_str());
        return pathBuffer;
    }

    std::string GetThreadSubFile(int32_t pid, int32_t tid, const std::string& subFileName)
    {
        char pathBuffer[256];
        sprintf(pathBuffer, "%s%d%s%d%s", LinuxProcessConstants::RootPath.c_str(), pid,
            LinuxProcessConstants::TaskDirectoryName.c_str(), tid, subFileName.c_str());
        return pathBuffer;
    }

    std::string GetStatusFilePathForProcess(int32_t pid)
    {
        return GetProcessSubFile(pid, LinuxProcessConstants::StatusFileName);
    }

    std::string GetExeFilePathForProcess(int pid)
    {
        return GetProcessSubFile(pid, LinuxProcessConstants::ExeFileName);
    }

    std::string GetCmdLinePathForProcess(int pid)
    {
        return GetProcessSubFile(pid, LinuxProcessConstants::CmdLineFileName);
    }

    std::string GetStatFilePathForProcess(int pid)
    {
        return GetProcessSubFile(pid, LinuxProcessConstants::StatFileName);
    }

    std::string GetTaskDirectoryPathForProcess(int pid)
    {
        return GetProcessSubFile(pid, LinuxProcessConstants::TaskDirectoryName);
    }

    std::string GetFileDescriptorDirectoryPathForProcess(int pid)
    {
        return GetProcessSubFile(pid, LinuxProcessConstants::FileDescriptorDirectoryName);
    }

    std::string GetStatFilePathForThread(int pid, int tid)
    {
        return GetThreadSubFile(pid, tid, LinuxProcessConstants::StatFileName);
    }


    struct ParsedStatus
    {
#if DEBUG
        int32_t Pid;
#endif
        uint64_t VmHWM;
        uint64_t VmRSS;
        uint64_t VmData;
        uint64_t VmSwap;
        uint64_t VmSize;
        uint64_t VmPeak;
    };



    std::map<std::string, std::string> CreateProcStatusFileContentMap(const std::string& statusFilePath)
    {
        using namespace Nuke::System;
        using namespace Nuke::System::IO;
        std::string _fileContents = File::ReadAllText(statusFilePath);
        const String& fileContents = String::AsConstNukeString(_fileContents);
        if (fileContents.empty())
        {
            return {};
        }
        auto contentLines = fileContents.Split("\n");
        std::map<std::string, std::string> contentMap;
        for (const auto& line : contentLines)
        {
            auto kv = line.Split(":\t");
            contentMap[kv.at(0)] = kv.at(1);
        }
        return contentMap;
    }

    ParsedStatus TryParseStatusFile(const std::string& statusFilePath)
    {
        ParsedStatus parsedStatus;
        auto contentMap = CreateProcStatusFileContentMap(statusFilePath);
        parsedStatus.VmHWM = std::stoul(contentMap.at("VmHWM"));
        parsedStatus.VmRSS = std::stoul(contentMap.at("VmRSS"));
        parsedStatus.VmData = std::stoul(contentMap.at("VmData"));
        parsedStatus.VmSwap = std::stoul(contentMap.at("VmSwap"));
        parsedStatus.VmSize = std::stoul(contentMap.at("VmSize"));
        parsedStatus.VmPeak = std::stoul(contentMap.at("VmPeak"));
        parsedStatus.VmData += std::stoul(contentMap.at("VmStk"));

        parsedStatus.VmData *= 1024;
        parsedStatus.VmPeak *= 1024;
        parsedStatus.VmSize *= 1024;
        parsedStatus.VmSwap *= 1024;
        parsedStatus.VmRSS *= 1024;
        parsedStatus.VmHWM *= 1024;
        return parsedStatus;
    }

    ParsedStatus TryReadStatusFile(int pid)
    {
        std::string statusFilePath = GetStatusFilePathForProcess(pid);
        auto parsedStatus = TryParseStatusFile(statusFilePath);
        return parsedStatus;
    }

    

    std::vector<int32_t> EnumerateProcessIds()
    {
        DIR* procDir = opendir(LinuxProcessConstants::RootPath.c_str());
        if (procDir != nullptr)
        {
            std::vector<int32_t> ids;
            dirent* procDirEntry = nullptr;
            while ((procDirEntry = readdir(procDir)) != nullptr)
            {
                if (procDirEntry->d_type == DT_DIR)
                {
                    std::string nameStr(procDirEntry->d_name);
                    if (nameStr == "." || nameStr == "..")
                    {
                        continue;
                    }
                    try
                    {
                        auto id = std::stoi(nameStr);
                        ids.push_back(id);
                    }
                    catch (...)
                    {
                        continue;
                    }
                }
                //printf("%s\n", procDirEntry->d_name);
            };
            closedir(procDir);
            return ids;
        }
        return {};
    }
    std::vector<int32_t> GetProcessIds()
    {
        return EnumerateProcessIds();
    }






    bool TryParseStatusFile(const std::string& statusFilePath, ParsedStatus& result)
    {
        try
        {
            ParsedStatus parsedStatus;
            auto contentMap = CreateProcStatusFileContentMap(statusFilePath);
            parsedStatus.VmHWM = std::stoul(contentMap.at("VmHWM"));
            parsedStatus.VmRSS = std::stoul(contentMap.at("VmRSS"));
            parsedStatus.VmData = std::stoul(contentMap.at("VmData"));
            parsedStatus.VmSwap = std::stoul(contentMap.at("VmSwap"));
            parsedStatus.VmSize = std::stoul(contentMap.at("VmSize"));
            parsedStatus.VmPeak = std::stoul(contentMap.at("VmPeak"));
            parsedStatus.VmData += std::stoul(contentMap.at("VmStk"));

            parsedStatus.VmData *= 1024;
            parsedStatus.VmPeak *= 1024;
            parsedStatus.VmSize *= 1024;
            parsedStatus.VmSwap *= 1024;
            parsedStatus.VmRSS *= 1024;
            parsedStatus.VmHWM *= 1024;
            result = parsedStatus;
        }
        catch (...)
        {
            return false;
        }
        return true;
    }

    bool TryReadStatusFile(int pid, ParsedStatus& result)
    {
        std::string statusFilePath = GetStatusFilePathForProcess(pid);
        bool isParseOK = TryParseStatusFile(statusFilePath, result);
        return isParseOK;
    }

    //
    // 本文件的成员顺序完全依据stat流顺序，请勿更改
    // 注掉的部分用不着就不读取了，但需要占位，请勿删除
    // 
    struct ParsedStat
    {
        // 名称                             //  索引
        int32_t pid;                      	//	0
        std::string comm;                 	//	1
        char state;                       	//	2
        int32_t ppid;                     	//	3
        // int32_t pgrp;                  	//	4
        int32_t session;                  	//	5
        // int32_t tty_nr;                	//	6
        // int32_t tpgid;                 	//	7
        // uint32_t flags;                	//	8
        // uint64_t minflt;               	//	9
        // uint64_t cminflt;              	//	10
        // uint64_t majflt;               	//	11
        // uint64_t cmajflt;              	//	12
        uint64_t utime;                   	//	13
        uint64_t stime;                   	//	14
        // int64_t cutime;                	//	15
        // int64_t cstime;                	//	16
        // int64_t priority;              	//	17
        int64_t nice;                     	//	18
        // int64_t num_threads;           	//	19
        // int64_t itrealvalue;           	//	20
        uint64_t starttime;               	//	21
        uint64_t vsize;                   	//	22
        int64_t rss;                      	//	23
        uint64_t rsslim;                  	//	24
        // uint64_t startcode;            	//	25
        // uint64_t endcode;              	//	26
        // uint64_t startstack;           	//	27
        // uint64_t kstkesp;              	//	28
        // uint64_t kstkeip;              	//	29
        // uint64_t signal;               	//	30
        // uint64_t blocked;              	//	31
        // uint64_t sigignore;            	//	32
        // uint64_t sigcatch;             	//	33
        // uint64_t wchan;                	//	34
        // uint64_t nswap;                	//	35
        // uint64_t cnswap;               	//	36
        // int32_t exit_signal;           	//	37
        // int32_t processor;             	//	38
        // uint32_t rt_priority;          	//	39
        // uint32_t policy;               	//	40
        // uint64_t delayacct_blkio_ticks;	//	41
        // uint64_t guest_time;           	//	42
        // int64_t cguest_time;           	//	43

    };

    ParsedStat TryParseStatFile(const std::string& statFilePath)
    {
        using namespace Nuke::System;
        using namespace Nuke::System::IO;
        std::string statFileContents = File::ReadAllText(statFilePath);

        auto statFileParts = String::AsConstNukeString(statFileContents).Split(" ");

        ParsedStat parsedStat;

        parsedStat.pid = std::stoi(statFileParts[0]);
        parsedStat.comm = statFileParts[1];
        parsedStat.state = statFileParts[2][0];
        parsedStat.ppid = std::stoi(statFileParts[3]);
        // pgrp
        parsedStat.session = std::stoi(statFileParts[5]);
        // tty_nr
        // tpgid
        // flags
        // majflt
        // cmagflt
        // minflt
        // cminflt
        parsedStat.utime = std::stoul(statFileParts[13]);
        parsedStat.stime = std::stoul(statFileParts[14]);
        // cutime
        // cstime
        // priority
        parsedStat.nice = std::stol(statFileParts[18]);
        // num_threads
        // itrealvalue
        parsedStat.starttime = std::stoul(statFileParts[21]);
        parsedStat.vsize = std::stoul(statFileParts[22]);
        parsedStat.rss = std::stol(statFileParts[23]);
        parsedStat.rsslim = std::stoul(statFileParts[24]);

        // startcode
        // endcode
        // startstack
        // kstkesp
        // kstkeip
        // signal
        // blocked
        // sigignore
        // sigcatch
        // wchan
        // nswap
        // cnswap
        // exit_signal
        // processor
        // rt_priority
        // policy
        // delayacct_blkio_ticks
        // guest_time
        // cguest_time


        return parsedStat;
    }

    ParsedStat TryReadStatFile(int pid)
    {
        return TryParseStatFile(GetStatFilePathForProcess(pid));
    }

    ParsedStat TryReadStatFile(int pid, int tid)
    {
        return TryParseStatFile(GetStatFilePathForThread(pid, tid));
    }

    ProcessInfo CreateProcessInfo(const ParsedStat& parsedStat, const ParsedStatus& parsedStatus, const std::string& processName = "")
    {
        int pid = parsedStat.pid;

        ProcessInfo pi;
        pi.ProcessId = pid;
        DEV_WARNING("wangkai 这里增加process名获取");
        //pi.ProcessName = Process.GetUntruncatedProcessName(ref parsedStat);
        pi.BasePriority = (int32_t)parsedStat.nice;
        pi.SessionId = parsedStat.session;
        pi.PoolPagedBytes = (int64_t)parsedStatus.VmSwap;
        pi.VirtualBytes = (int64_t)parsedStatus.VmSize;
        pi.VirtualBytesPeak = (int64_t)parsedStatus.VmPeak;
        pi.WorkingSetPeak = (int64_t)parsedStatus.VmHWM;
        pi.WorkingSet = (int64_t)parsedStatus.VmRSS;
        pi.PageFileBytes = (int64_t)parsedStatus.VmSwap;
        pi.PrivateBytes = (int64_t)parsedStatus.VmData;
        // 其余值用getrusage填充
        
        // 读取 /proc/pid/task/ 来获取每个thread
        std::string tasksDir = GetTaskDirectoryPathForProcess(pid);
        try
        {
            //foreach(string taskDir in Directory.EnumerateDirectories(tasksDir))
            //{
            //    // ...and read its associated /proc/pid/task/tid/stat file to create a ThreadInfo
            //    string dirName = Path.GetFileName(taskDir);
            //    int tid;
            //    Interop.procfs.ParsedStat stat;
            //    if (int.TryParse(dirName, NumberStyles.Integer, CultureInfo.InvariantCulture, out tid) &&
            //        Interop.procfs.TryReadStatFile(pid, tid, out stat))
            //    {
            //        pi._threadInfoList.Add(new ThreadInfo()
            //            {
            //                _processId = pid,
            //                _threadId = (ulong)tid,
            //                _basePriority = pi.BasePriority,
            //                _currentPriority = (int)stat.nice,
            //                _startAddress = IntPtr.Zero,
            //                _threadState = ProcFsStateToThreadState(stat.state),
            //                _threadWaitReason = ThreadWaitReason.Unknown
            //            });
            //    }
            //}
        }
        catch (...)
        {
            // 获得pid和信息之间的时间片中，进程可能已经挂逼了，所以这里就随它去吧，无害操作。
        }

        return pi;
    }
    ProcessInfo CreateProcessInfo(int pid)
    {
        auto stat = TryReadStatFile(pid);
        auto status = TryReadStatusFile(pid);

        return CreateProcessInfo(stat, status);
    }
    std::vector<ProcessInfo> GetProcessInfos()
    {
        std::vector<ProcessInfo> processInfos;
        auto pids = EnumerateProcessIds();
        for (const int32_t pid : pids)
        {
            ProcessInfo pi = CreateProcessInfo(pid);
            processInfos.push_back(pi);
        }
        return processInfos;

    }

    enum SysConfName
    {
        _SC_CLK_TCK = 1,
        _SC_PAGESIZE = 2
    };

    TimeSpan TicksToTimeSpan(double ticks)
    {
        long ticksPerSecond = sysconf(SysConfName::_SC_CLK_TCK);
        

        return TimeSpan::FromSeconds(ticks / (double)ticksPerSecond);
    }

    TimeSpan GetTotalProcessorTime(int32_t pid)
    {

        using namespace Nuke::CrossPlatform;

        ParsedStat stat = TryReadStatFile(pid);
        return TicksToTimeSpan(stat.utime + stat.stime);
    }
}

#endif