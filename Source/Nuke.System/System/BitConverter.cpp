
#include "_pch.h"
#include "BitConverter.h"

namespace Nuke::System
{
#if BIGENDIAN
    const bool BitConverter::IsLittleEndian = false;
#else
    const bool BitConverter::IsLittleEndian = true;
#endif

}