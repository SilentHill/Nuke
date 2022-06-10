
#include "Process.h"

#include <CrossPlatform/ProcessApi.h>
#include <System/Environment.h>

namespace Nuke::System::Diagnostics
{
    class Process::ProcessImpl
    {
    public:
        ProcessImpl() = default;
        ~ProcessImpl() = default;
        ProcessImpl(int32_t processId)
        {
            _processId = processId;
            _haveProcessId = true;
        }
    private:
        friend class Process;
        TimeSpan TotalProcessorTime()
        {
            using namespace CrossPlatform;
            return ProcessApi::GetTotalProcessorTime(_processId);
        }
        bool _haveProcessId;
        int32_t _processId;
    };

    Process::Process()
    {
        impl = std::make_unique<ProcessImpl>();
    }

    Process::~Process()
    {

    }


    Process& Process::GetCurrentProcess()
    {
        static Process currentProcess = Process::GetProcessById(Environment::ProcessId());
        return currentProcess;
    }

    std::vector<Process> Process::GetProcesses()
    {
        using namespace CrossPlatform;
        std::vector<ProcessApi::ProcessInfo> processInfos = ProcessApi::GetProcessInfos();
        std::vector<Process> processes;

        for (const auto& processInfo : processInfos)
        {
            processes.push_back(Process::GetProcessById(processInfo.ProcessId));

        }
        return processes;
    }

    Process Process::GetProcessById(int32_t processId)
    {
        Process process;
        process.impl->_processId = processId;
        process.impl->_haveProcessId = true;
        return std::move(process);
    }

    int32_t Process::Id() const
    {
        return impl->_processId;
    }

    TimeSpan Process::TotalProcessorTime() const
    {
        return impl->TotalProcessorTime();
    }
}