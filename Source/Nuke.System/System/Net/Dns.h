
#pragma once

namespace Nuke::System::Net
{
	class Dns
	{
	public: // 构造析构
		Dns() = delete;
		Dns(const Dns&) = delete;
		Dns(Dns&&) = delete;
		~Dns() = delete;

	public:
		static void Resolve(const std::string& hostName, const std::string& dnsIp);
	};
}
