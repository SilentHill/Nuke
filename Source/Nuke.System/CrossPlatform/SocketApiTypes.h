
#pragma once

#include "Result.h"

namespace Nuke::CrossPlatform::SocketApi
{
#ifdef __linux__
    using SocketHandle = int;
    const SocketHandle InvalidSocketHandle = 0;
#elif _WIN32
    using SocketHandle = uintptr_t;
    const SocketHandle InvalidSocketHandle = (uintptr_t)(~0);
#endif

    enum class SocketOptionLevel : int32_t
    {
        Socket = 0xffff,
        IP = 0,
        IPv6 = 41,
        Tcp = 6,
        Udp = 17,
    };

    enum class SocketOptionName : int32_t
    {
#pragma region SocketOptionLevel.Socket
        Debug = 0x0001,
        AcceptConnection = 0x0002,
        ReuseAddress = 0x0004,
        KeepAlive = 0x0008,
        DontRoute = 0x0010,
        Broadcast = 0x0020,
        UseLoopback = 0x0040,
        Linger = 0x0080,
        OutOfBandInline = 0x0100,
        DontLinger = ~Linger,
        ExclusiveAddressUse = ~ReuseAddress,
        SendBuffer = 0x1001,
        ReceiveBuffer = 0x1002,
        SendLowWater = 0x1003,
        ReceiveLowWater = 0x1004,
        SendTimeout = 0x1005,
        ReceiveTimeout = 0x1006,
        Error = 0x1007,
        Type = 0x1008,
        ReuseUnicastPort = 0x3007,
        MaxConnections = 0x7fffffff,
#pragma endregion

#pragma region SocketOptionLevel.IP
        IPOptions = 1,
        HeaderIncluded = 2,
        TypeOfService = 3,
        IpTimeToLive = 4,
        MulticastInterface = 9,
        MulticastTimeToLive = 10,
        MulticastLoopback = 11,
        AddMembership = 12,
        DropMembership = 13,
        DontFragment = 14,
        AddSourceMembership = 15,
        DropSourceMembership = 16,
        BlockSource = 17,
        UnblockSource = 18,
        PacketInformation = 19,
#pragma endregion

#pragma region SocketOptionLevel.IPv6
        HopLimit = 21,

        IPProtectionLevel = 23,

        IPv6Only = 27,
#pragma endregion

#pragma region SocketOptionLevel.Tcp
        NoDelay = 1,
        BsdUrgent = 2,
        Expedited = 2,
        TcpKeepAliveRetryCount = 16,
        TcpKeepAliveTime = 3,
        TcpKeepAliveInterval = 17,
#pragma endregion

#pragma region SocketOptionlevel.Udp
        NoChecksum = 1,
        ChecksumCoverage = 20,
        UpdateAcceptContext = 0x700B,
        UpdateConnectContext = 0x7010,
#pragma endregion
    };

}