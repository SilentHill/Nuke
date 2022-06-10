
#include "../_pch.h"
#include "NetworkApi.h"

#ifdef __linux__
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <arpa/nameser.h>
    #include <resolv.h>
#elif _WIN32
    #include <WinSock2.h>
    #include <Windows.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "iphlpapi.lib")
#endif

namespace Nuke::CrossPlatform::NetworkApi
{
#ifdef __linux__
    std::vector<std::string> GetCurrentDnsServers_Linux()
    {
        res_init();
        std::vector<std::string> dnsServers;
        auto dnsServerList = _res.nsaddr_list;
        for (int i = 0; i < _res.nscount; ++i)
        {
            dnsServers.push_back(inet_ntoa(dnsServerList[i].sin_addr));
        }
        return dnsServers;
    }
#elif _WIN32
    std::vector<std::string> GetCurrentDnsServers_Windows()
    {
        // 获取需要的缓冲长度
        FIXED_INFO stackFixedInfo;
        unsigned long fixedInfoNeedSize = 0;
        ::GetNetworkParams(&stackFixedInfo, &fixedInfoNeedSize);
        FIXED_INFO* fixedInfos = reinterpret_cast<FIXED_INFO*>(malloc(fixedInfoNeedSize));
        if (fixedInfos == nullptr)
        {
            return {};
        }
        uint32_t apiResult = ::GetNetworkParams(fixedInfos, &fixedInfoNeedSize);
        if (apiResult == NO_ERROR)
        {
            std::vector<std::string> dnsServers;
            dnsServers.push_back(fixedInfos->DnsServerList.IpAddress.String);
            IP_ADDR_STRING* ipAddrString = fixedInfos->DnsServerList.Next;
            while (ipAddrString != nullptr)
            {
                dnsServers.push_back(ipAddrString->IpAddress.String);
                ipAddrString = ipAddrString->Next;
            }
            return dnsServers;
        }
        else
        {
            return {};
        }

        if (fixedInfos)
        {
            free(fixedInfos);
        }
        return {};
    }

#endif

    
    std::vector<std::string> GetCurrentDnsServers()
    {
#ifdef __linux__
        return GetCurrentDnsServers_Linux();
#elif _WIN32
        return GetCurrentDnsServers_Windows();
#endif
    }
    void GetAdapterAddresses(AddressFamily addressFamily)
    {
#ifdef __linux__
        //getifaddrs
        return;
#elif _WIN32
        enum GetAdaptersAddressesFlags
        {
            SkipUnicast = 0x0001,
            SkipAnycast = 0x0002,
            SkipMulticast = 0x0004,
            SkipDnsServer = 0x0008,
            IncludePrefix = 0x0010,
            SkipFriendlyName = 0x0020,
            IncludeWins = 0x0040,
            IncludeGateways = 0x0080,
            IncludeAllInterfaces = 0x0100,
            IncludeAllCompartments = 0x0200,
            IncludeTunnelBindingOrder = 0x0400,
        };

        // 整点缓冲
        const size_t workingBufferSize = 16 * 1024; // 16KB
        unsigned long outLength = workingBufferSize;
        IP_ADAPTER_ADDRESSES* adapaterAddresses = static_cast<IP_ADAPTER_ADDRESSES*>(malloc(workingBufferSize));

        ::GetAdaptersAddresses(static_cast<unsigned long>(addressFamily), IncludeGateways|IncludeWins, nullptr, adapaterAddresses, &outLength);

        IP_ADAPTER_ADDRESSES* currentAdapter = adapaterAddresses;
        while (currentAdapter)
        {
            currentAdapter = currentAdapter->Next;
        }

        free(adapaterAddresses);
        return;
#endif
    }
}