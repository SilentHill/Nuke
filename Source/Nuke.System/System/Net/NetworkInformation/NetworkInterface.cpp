
#include "_pch.h"
#include "NetworkInterface.h"
#include <CrossPlatform/NetworkApi.h>

namespace Nuke::System::Net::NetworkInformation
{
    namespace NetworkFiles
    {
        // linux特定文件路径
        static const std::string InterfaceListingFile = "/proc/net/dev";
        static const std::string SnmpV4StatsFile = "/proc/net/snmp";
        static const std::string SnmpV6StatsFile = "/proc/net/snmp6";
        static const std::string Ipv4ConfigFolder = "/proc/sys/net/ipv4/conf";
        static const std::string Ipv6ConfigFolder = "/proc/sys/net/ipv6/conf";
        static const std::string Ipv4GlobalForwardingFile = "/proc/sys/net/ipv4/ip_forward";
        static const std::string Ipv4RouteFile = "/proc/net/route";
        static const std::string Ipv6RouteFile = "/proc/net/ipv6_route";
        static const std::string SockstatFile = "/proc/net/sockstat";
        static const std::string Sockstat6File = "/proc/net/sockstat6";
        static const std::string SysClassNetFolder = "/sys/class/net";
        static const std::string EtcResolvConfFile = "/etc/resolv.conf";
        static const std::string Tcp4ConnectionsFile = "/proc/net/tcp";
        static const std::string Tcp6ConnectionsFile = "/proc/net/tcp6";
        static const std::string Udp4ConnectionsFile = "/proc/net/udp";
        static const std::string Udp6ConnectionsFile = "/proc/net/udp6";
        static const std::string DHClientLeasesFile = "/var/lib/dhcp/dhclient.leases";
        static const std::string DHClientInterfaceLeasesFile = "/var/lib/dhcp/dhclient.{0}.leases";
        static const std::string DHClientSecondaryInterfaceLeasesFile = "/var/lib/dhcp/dhclient6.{0}.leases";
        static const std::string SmbConfFile = "/etc/samba/smb.conf";

        // 独立文件
        static const std::string AllNetworkInterfaceFileName = "all";
        static const std::string DefaultNetworkInterfaceFileName = "default";
        static const std::string FlagsFileName = "flags";
        static const std::string ForwardingFileName = "forwarding";
        static const std::string MtuFileName = "mtu";
        static const std::string OperstateFileName = "operstate";
        static const std::string SpeedFileName = "speed";
        static const std::string TransmitQueueLengthFileName = "tx_queue_len";
    };

    class NetworkInterface::NetworkInterfaceImpl
    {

    };


    NetworkInterface::NetworkInterface()
    {
        impl = std::make_unique<NetworkInterface::NetworkInterfaceImpl>();
    }

    std::vector<NetworkInterface> NetworkInterface::GetAllNetworkInterfaces()
    {
        return {};
    }

    NetworkInterface::~NetworkInterface()
    {

    }

}