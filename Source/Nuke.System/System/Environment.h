
#pragma once

namespace Nuke::System
{
    class Environment
    {
    public: // 构造析构
        Environment() = delete;
        Environment(const Environment&) = delete;
        Environment(Environment&&) = delete;
        ~Environment();

    public: // 属性方法
        static int32_t ProcessId();
    };

}