
#pragma once

#include <System/Net/Sockets/AddressFamily.h>

namespace Nuke::CrossPlatform::NetworkApi
{
    using namespace Nuke::System::Net::Sockets;

    std::vector<std::string> GetCurrentDnsServers();

    void GetAdapterAddresses(AddressFamily addressFamily);
}