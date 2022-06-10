
#pragma once

namespace Nuke::System::Net::Sockets
{
	class TcpClient
	{
	public:
		TcpClient();
		~TcpClient(); 
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
