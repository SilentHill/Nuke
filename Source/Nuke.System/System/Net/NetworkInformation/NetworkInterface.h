
#pragma once

#include <System/Net/NetworkInformation/NetworkInterfaceType.h>

namespace Nuke::System::Net::NetworkInformation
{
    // 提供网络接口的配置和统计信息。
    class NetworkInterface final
    {
    private: // 构造析构
        NetworkInterface();
        NetworkInterface(const NetworkInterface&) = delete;
        NetworkInterface(NetworkInterface&&) = delete;
    public:
        ~NetworkInterface();

    public: // 属性
        // 获取接口的描述
        std::string GetDescription();

        // 获取网络适配器的标识符
        std::string GetId();

        // 获取网络适配器的名称
        std::string GetName();

        // 获取接口类型
        NetworkInterfaceType GetNetworkInterfaceType();

    public: // 方法

        // 返回描述本地计算机上的网络接口的对象。
        static std::vector<NetworkInterface> GetAllNetworkInterfaces();

    private: // 私有
        class NetworkInterfaceImpl;
        std::unique_ptr<NetworkInterfaceImpl> impl;

    };
}
