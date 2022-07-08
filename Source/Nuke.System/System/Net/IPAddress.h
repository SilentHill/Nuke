
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
        IPAddress(uint8_t* address, int32_t addressLength);
        ~IPAddress();

    public: // 静态边界字段
        static const IPAddress& Any;             // 提供一个IPv4 IP地址，指示Socket必须侦听所有网络接口上的客户端活动。
        static const IPAddress& Loopback;        // 提供IPv4环回地址。
        static const IPAddress& Broadcast;       // 提供IPv4广播地址。
        static const IPAddress& None;            // 提供指示不应使用任何网络接口的IPv4地址
        
        static const IPAddress& IPv6Any;         // 提供一个IPv6 IP地址，指示Socket必须侦听所有网络接口上的客户端活动。
        static const IPAddress& IPv6Loopback;    // 提供IPv6环回地址。
        static const IPAddress& IPv6None;        // 提供指示不应使用任何网络接口的IPv6地址

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

    private: // 私有
        class IPAddressImpl;
        std::unique_ptr<IPAddressImpl> impl;
        
    };

}
