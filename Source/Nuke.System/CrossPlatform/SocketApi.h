
#pragma once

#include "Result.h"
#include "SocketApiTypes.h"

namespace Nuke::CrossPlatform::SocketApi
{

	void InitializeSocketEnvironment();

	SocketHandle CreateTcpSocket();

    SocketHandle CreateUdpSocket();

	InvokeResult CloseSocket(SocketHandle socketHandle);

	struct SocketHandleDeleter
	{
		void operator()(SocketHandle socketHandle)
		{
			CloseSocket(socketHandle);
		}
		using pointer = SocketHandle;
	};

	InvokeResult Connect(SocketHandle socketHandle, const std::string& address, int32_t port);

    int32_t SendTo(SocketHandle socketHandle, const std::string& address, uint32_t port, uint8_t* buffer, int32_t length);

	int32_t Send(SocketHandle socketHandle, uint8_t* buffer, int32_t length);

    int32_t Recv(SocketHandle socketHandle, uint8_t* buffer, int32_t length);

    int32_t RecvFrom(SocketHandle socketHandle, uint32_t port, uint8_t* buffer, int32_t length);

	bool Connected(SocketHandle socketHandle);

    InvokeResult GetSockOpt(SocketHandle socketHandle, SocketOptionLevel socketOptionLevel, SocketOptionName socketOptionName, void* optionValue, uint32_t* optionLength);

	InvokeResult GetLastInvokeResult();

}