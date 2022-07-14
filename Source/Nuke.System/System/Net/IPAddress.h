
#pragma once

#include "Sockets/AddressFamily.h"

namespace Nuke::System::Net
{
    /// <summary>
    /// Internet 协议 (IP) 地址
    /// </summary>
    class IPAddress
    {
    public: // 构造析构

        IPAddress(int64_t newAddress);
        IPAddress(std::span<std::byte> address);
        IPAddress(std::span<std::byte> address, int64_t scopeid);
        IPAddress(const IPAddress& ipaddress);
        ~IPAddress();

    public: // 静态构造
        static IPAddress Parse(std::string_view ipStringView);

    public: // 静态预置对象
        static const IPAddress Any;             // 提供一个IPv4 IP地址，指示Socket必须侦听所有网络接口上的客户端活动。
        static const IPAddress Loopback;        // 提供IPv4环回地址。
        static const IPAddress Broadcast;       // 提供IPv4广播地址。
        static const IPAddress None;            // 提供指示不应使用任何网络接口的IPv4地址
        
        static const IPAddress IPv6Any;         // 提供一个IPv6 IP地址，指示Socket必须侦听所有网络接口上的客户端活动。
        static const IPAddress IPv6Loopback;    // 提供IPv6环回地址。
        static const IPAddress IPv6None;        // 提供指示不应使用任何网络接口的IPv6地址

    public:
        std::string ToString();

    public: // 操作符重载
        bool operator==(const IPAddress& ipAddress);
        

    public: // 属性
        Sockets::AddressFamily AddressFamily();          // 获取 IP 地址的地址族。

        bool IsIPv4MappedToIPv6();

        bool IsIPv6LinkLocal();

        bool IsIPv6Multicast();

        bool IsIPv6SiteLocal();

        bool IsIPv6Teredo();

        int64_t ScopeId();

        void SetScopeId(int64_t value);

    public: // 方法
        std::vector<uint8_t> GetAddressBytes();

        static int64_t HostToNetworkOrder(int64_t host);

        static int HostToNetworkOrder(int32_t host);

        static short HostToNetworkOrder(int16_t host);

        static int64_t NetworkToHostOrder(int64_t network);

        static int NetworkToHostOrder(int32_t network);

        static short NetworkToHostOrder(int16_t network);

    private: // 私有
        class IPAddressImpl;
        std::unique_ptr<IPAddressImpl> impl;
        
    };

}
