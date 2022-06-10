
#include "_pch.h"
#include <CrossPlatform/SocketApi.h>
#include "TcpClient.h"

namespace Nuke::System::Net::Sockets
{
    using namespace CrossPlatform;

    class TcpClient::TcpClientImpl
    {
    public:
        TcpClientImpl()
        {
            static std::once_flag winsock_init_flag;
            std::call_once(winsock_init_flag, []()
            {
                SocketApi::InitializeSocketEnvironment();
            });
            _socketHandle = SocketApi::InvalidSocketHandle;
        }
        SocketApi::SocketHandle _socketHandle;
    };

    TcpClient::TcpClient()
    {
        impl = std::make_unique<TcpClient::TcpClientImpl>();
    }

    TcpClient::~TcpClient()
    {
        Close();
    }

    void TcpClient::Connect(const std::string& address, int32_t port)
    {
        impl->_socketHandle = SocketApi::CreateTcpSocket();
        SocketApi::Connect(impl->_socketHandle, address, port);
    }

    void TcpClient::Write(uint8_t* buffer, int32_t offset, int32_t size)
    {
        SocketApi::Send(impl->_socketHandle, buffer + offset, size);
    }

    int32_t TcpClient::Read(uint8_t* buffer, int offset, int size)
    {
        return SocketApi::Recv(impl->_socketHandle, buffer + offset, size);
    }


    bool TcpClient::Connected()
    {
        return SocketApi::Connected(impl->_socketHandle);
    }


    int32_t TcpClient::Available()
    {
        return 0;
    }

    void TcpClient::Close()
    {
        SocketApi::CloseSocket(impl->_socketHandle);
    }


}