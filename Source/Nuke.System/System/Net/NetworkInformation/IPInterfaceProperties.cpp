
#include "_pch.h"
#include "IPInterfaceProperties.h"
#include <CrossPlatform/NetworkApi.h>

namespace Nuke::System::Net::NetworkInformation
{
    using namespace Nuke::CrossPlatform;

    std::vector<IPAddress> IPInterfaceProperties::GetDnsAddresses()
    {
        auto dnsServerStrings = NetworkApi::GetCurrentDnsServers();
        std::vector<IPAddress> dnsAddresses;
        for (const auto& dnsServerString : dnsServerStrings)
        {

        }
        return {};
    }
}