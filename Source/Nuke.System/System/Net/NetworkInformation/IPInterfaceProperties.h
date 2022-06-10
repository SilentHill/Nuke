
#pragma once

#include <System/Net/IPAddress.h>

namespace Nuke::System::Net::NetworkInformation
{
    class IPInterfaceProperties final
    {
    private: // 构造析构
        IPInterfaceProperties();
        IPInterfaceProperties(const IPInterfaceProperties&) = delete;
        IPInterfaceProperties(IPInterfaceProperties&&) = delete;
    public:
        ~IPInterfaceProperties();

    public: // 方法
        std::vector<IPAddress> GetDnsAddresses();
    };
}
