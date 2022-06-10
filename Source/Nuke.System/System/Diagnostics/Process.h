
#pragma once

#include "_pch.h"
#include <System/TimeSpan.h>

namespace Nuke::System::Diagnostics
{
    class Process
    {
    public: // 构造析构
        Process();
        Process(const Process&) = delete;
        Process operator=(const Process&) = delete;
        Process(Process&&) = default;
        ~Process();

    public: // 静态构造
        static Process& GetCurrentProcess();
        static std::vector<Process> GetProcesses();
        static Process GetProcessById(int32_t processId);

    public: // PID
        int32_t Id() const;

    public:
        TimeSpan TotalProcessorTime() const;


    private: // 私有
        class ProcessImpl;
        std::unique_ptr<ProcessImpl> impl;
    };
}