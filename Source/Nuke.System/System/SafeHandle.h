
#pragma once

#include <type_traits>

namespace Nuke::System
{
    class ISafeHandle
    {
    public: // 构造析构
        virtual ~ISafeHandle();

    public: // 属性
        bool IsClosed();

    protected:  // 可覆写
        virtual bool IsInvalid();
        virtual bool ReleaseHandle();
    };
    
    //template <NativeHandle>
    class SafeHandle
    {
    public:
        using NativeHandle = void*;
        

    };

}