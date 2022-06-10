
#include "../_pch.h"
#include "SocketApi.h"

#ifdef __linux__
    #include <sys/socket.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <poll.h>
    
#elif _WIN32
    #pragma warning(disable:5105)
    #include <winsock2.h>
    #include <ws2tcpip.h>
#endif

#include "Result.h"

namespace Nuke::CrossPlatform::SocketApi
{


    std::vector<std::string> DnsResolve(const std::string& hostName)
    {
        // ����addrinfo
        addrinfo hints = { 0 };
        hints.ai_flags = AI_NUMERICSERV;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_addrlen = 0;
        hints.ai_addr = nullptr;
        hints.ai_canonname = nullptr;
        hints.ai_next = nullptr;
        addrinfo* result = nullptr;
        auto get_addr_info_result = getaddrinfo((const char*)hostName.c_str(), "", &hints, &result);

        while (result)
        {

        };

        freeaddrinfo(result);

        return {};
    }

    enum class SocketStatus
    {
        Success = 0,
        Error = -1
    };

    void InitializeSocketEnvironment()
    {
#ifdef __linux__
        return;

#elif _WIN32
        WSADATA wsaData;
        auto wsa_start_up_result = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (wsa_start_up_result == 0)
        {
            return;
        }
        else
        {
            return;
        }
#endif
    }

    SocketHandle CreateTcpSocket()
    {
        return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }

    SocketHandle CreateUdpSocket()
    {
        return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    std::string addrinfo_to_ip_string(const addrinfo& ai)
    {
        char string_addr[16];
        sockaddr_in* ipv4 = reinterpret_cast<sockaddr_in*>(ai.ai_addr);
        in_addr* ipv4_sin_addr = &(ipv4->sin_addr);
        inet_ntop(AF_INET, ipv4_sin_addr, string_addr, 16);
        return string_addr;
    }

    InvokeResult Connect(SocketHandle socketHandle, const std::string& address, int32_t port)
    {
        // ����addrinfo
        addrinfo hints = { 0 };
        hints.ai_flags = AI_NUMERICSERV;
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_addrlen = 0;
        hints.ai_addr = nullptr;
        hints.ai_canonname = nullptr;
        hints.ai_next = nullptr;

        addrinfo* result = nullptr;
        auto get_addr_info_result = getaddrinfo((const char*)address.c_str(), std::to_string(port).c_str(), &hints, &result);
        SocketStatus connect_result = static_cast<SocketStatus>(connect(socketHandle, result->ai_addr, (int)result->ai_addrlen));

        freeaddrinfo(result);
        if (connect_result == SocketStatus::Error)
        {
            CloseSocket(socketHandle);
            socketHandle = InvalidSocketHandle;
            return GetLastInvokeResult();
        }
        return InvokeResult::Success;
    }
    int32_t SendTo(SocketHandle socketHandle, const std::string& address, uint32_t port, uint8_t* buffer, int32_t length)
    {
        auto toBeSentByteCount = length;
        auto totalSentBytesCount = 0;
        sockaddr_in recvAddr;
        recvAddr.sin_family = AF_INET;
        recvAddr.sin_port = htons(port);
        recvAddr.sin_addr.s_addr = inet_addr(address.c_str());
        do
        {
            auto singleCallSentBytesSize = sendto(socketHandle,  
                reinterpret_cast<const char*>(buffer + (length - toBeSentByteCount)), 
                length, 0,
                reinterpret_cast<sockaddr*>(&recvAddr), sizeof(sockaddr_in));
            if (singleCallSentBytesSize == -1)
            {
                auto result = GetLastInvokeResult();
                return totalSentBytesCount;
            }
            else
            {
                totalSentBytesCount += singleCallSentBytesSize;
                toBeSentByteCount = toBeSentByteCount - singleCallSentBytesSize;
                if (toBeSentByteCount == 0)
                {
                    break;
                }
                else if (toBeSentByteCount > 0)
                {
                    continue;
                }
            }
        } while (true);
        return totalSentBytesCount;
    }
    int32_t Send(SocketHandle socketHandle, uint8_t* buffer, int32_t length)
    {
        auto toBeSentByteCount = length;
        auto totalSentBytesCount = 0;
        do
        {
            auto singleCallSentBytesSize = send(socketHandle, reinterpret_cast<const char*>(buffer + (length - toBeSentByteCount)), length, 0);
            if (singleCallSentBytesSize == -1)
            {
                return totalSentBytesCount;
            }
            else
            {
                totalSentBytesCount += singleCallSentBytesSize;
                toBeSentByteCount = toBeSentByteCount - singleCallSentBytesSize;
                if (toBeSentByteCount == 0)
                {
                    break;
                }
                else if (toBeSentByteCount > 0)
                {
                    continue;
                }
            }
        } while (true);
        return totalSentBytesCount;
    }
    int32_t RecvFrom(SocketHandle socketHandle, uint32_t port, uint8_t* buffer, int32_t size)
    {
        auto toBeReceivedByteCount = size;
        auto totalReceivedByteCount = 0;
        sockaddr_in recvAddr;
        recvAddr.sin_family = AF_INET;
        recvAddr.sin_port = htons(port);
        recvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        
        auto bind_result = bind(socketHandle, (sockaddr*)&recvAddr, sizeof(recvAddr));

        auto result = SocketApi::GetLastInvokeResult();
        int32_t recvLength = sizeof(recvAddr);
        do
        {
            auto singleCallReceivedByteCount = recvfrom(
                socketHandle, reinterpret_cast<char*>(buffer + (size - toBeReceivedByteCount)),
                size, 0,
                (sockaddr*)&recvAddr , (socklen_t*) & recvLength);
            if (singleCallReceivedByteCount == -1)
            {
                return totalReceivedByteCount;
            }
            else
            {
                totalReceivedByteCount += singleCallReceivedByteCount;
                toBeReceivedByteCount = toBeReceivedByteCount - singleCallReceivedByteCount;
                if (toBeReceivedByteCount == 0)
                {
                    break;
                }
                else if (toBeReceivedByteCount > 0)
                {
                    continue;
                }
            }
        } while (true);
        return totalReceivedByteCount;
    }
    int32_t Recv(SocketHandle socketHandle, uint8_t* buffer, int32_t size)
    {
        auto toBeReceivedByteCount = size;
        auto totalReceivedByteCount = 0;
        do
        {
            auto singleCallReceivedByteCount = recv(socketHandle, reinterpret_cast<char*>(buffer + (size - toBeReceivedByteCount)), size, 0);
            if (singleCallReceivedByteCount == -1)
            {
                return totalReceivedByteCount;
            }
            else
            {
                totalReceivedByteCount += singleCallReceivedByteCount;
                toBeReceivedByteCount = toBeReceivedByteCount - singleCallReceivedByteCount;
                if (toBeReceivedByteCount == 0)
                {
                    break;
                }
                else if (toBeReceivedByteCount > 0)
                {
                    continue;
                }
            }
        } while (true);
        return totalReceivedByteCount;
    }

    bool Connected(SocketHandle socketHandle)
    {
#ifdef __linux__

        pollfd pfd = { 0 };
        pfd.fd = socketHandle;
        pfd.events = POLLOUT;
        if (poll(&pfd, 1, -1) == -1)
        {
            return false;
        }
        else
        {
            if ((pfd.revents & POLLOUT) == 0)
            {
                return false;
            }
        }
        return true;
#elif _WIN32

        WSAPOLLFD wsaPollFd = { 0 };
        wsaPollFd.fd = socketHandle;
        wsaPollFd.events = POLLOUT;
        auto pollResult = ::WSAPoll(&wsaPollFd, 1, 10);
        if ((wsaPollFd.revents & POLLOUT) == 0)
        {
            return false;
        }
        return true;
#endif
}

    InvokeResult GetSockOpt(SocketHandle socketHandle, SocketOptionLevel socketOptionLevel, SocketOptionName socketOptionName, void* optionValue, uint32_t* optionLength)
    {
        return static_cast<InvokeResult>(getsockopt(socketHandle,
            static_cast<int32_t>(socketOptionLevel),
            static_cast<int32_t>(socketOptionName),
            (char*)optionValue,
            reinterpret_cast<socklen_t*>(optionLength)));
    }

    InvokeResult GetLastInvokeResult()
    {

#ifdef __linux__
        return static_cast<InvokeResult>(errno);
#elif _WIN32
        return static_cast<InvokeResult>(WSAGetLastError());
#endif

    }

    InvokeResult CloseSocket(SocketHandle socketHandle)
    {
#ifdef __linux__
        return static_cast<InvokeResult>(close(socketHandle));
#elif _WIN32
        return static_cast<InvokeResult>(closesocket(socketHandle));
#endif
        return InvokeResult();
    }
}