
#pragma once

#include <System/Net/Sockets/AddressFamily.h>
#include <System/Net/IPAddress.h>

namespace Nuke::System::Net
{
    /// <summary>
    /// Initializes a new instance of the IPEndPoint class.
    /// </summary>
    class IPEndPoint final
    {
    public:
        IPEndPoint(int64_t address, int32_t port);
        IPEndPoint(const IPAddress& address, int32_t port);
    };
}
 