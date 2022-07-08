
#pragma once

#include "System/Net/IPEndPoint.h"

namespace Nuke::System::Net::Sockets
{
	/// <summary>
	/// Provides client connections for TCP network services.
	/// </summary>
	class TcpClient final
	{
	public: // 构造析构
		TcpClient();
		TcpClient(const TcpClient&) = delete;
		TcpClient(TcpClient&&) = delete;
		~TcpClient();

	public:
		TcpClient(const IPEndPoint& ipEndPoint);
		TcpClient(const AddressFamily& addressFamily);
		TcpClient(const std::string& hostname, int32_t port);

	public:
		void Connect(const std::string& address, int32_t port);
		void Close();
		bool Connected();
		int32_t Available();
		void Write(uint8_t* buffer, int32_t offset, int32_t size);
		int32_t Read(uint8_t* buffer, int offset, int size);
	private:
		class TcpClientImpl;
		std::unique_ptr<TcpClientImpl> impl;
	};
}
