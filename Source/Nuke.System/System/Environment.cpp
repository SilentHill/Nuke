
#include "_pch.h"
#include "Environment.h"
#include "CrossPlatform/ProcessApi.h"

namespace Nuke::System
{
    int32_t Environment::ProcessId()
    {
        return CrossPlatform::ProcessApi::GetCurrentProcessId();
    }
}