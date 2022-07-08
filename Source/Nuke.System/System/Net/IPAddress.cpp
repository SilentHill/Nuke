
#include "_pch.h"
#include "IPAddress.h"

namespace Nuke::System::Net
{
    using namespace Nuke::System::Net::Sockets;

    namespace IPAddressParserStatics
    {
        const int32_t IPv4AddressBytes = 4;
        const int32_t IPv6AddressBytes = 16;
        const int32_t IPv6AddressShorts = IPv6AddressBytes / 2;
        const int32_t NumberOfLabels = IPv6AddressBytes / 2;
    }

    class IPAddress::IPAddressImpl
    {
    private:
        friend class IPAddress;
        uint32_t _addressOrScopeId;
        int32_t _hashCode;
        std::string _toString;
        std::vector<uint16_t> _numbers;

    private:
        uint32_t GetPrivateAddress()
        {
            return _addressOrScopeId;
        }

        void SetPrivateAddress(uint32_t value)
        {
            _toString = "";
            _hashCode = 0;
            _addressOrScopeId = value;
        }

        uint32_t GetPrivateScopeID()
        {
            return _addressOrScopeId;
        }

        void SetPrivateScopeID(uint32_t value)
        {
            _toString = "";
            _hashCode = 0;
            _addressOrScopeId = value;
        }

        bool IsIPv4()
        {
            return _numbers.size() == 0;
        }

        bool IsIPv6()
        {
            return _numbers.size() > 0;
        }
    };

    // IPAddress接口转发
    IPAddress::IPAddress(uint8_t* address, int32_t addressLength)
    {
        impl = std::make_unique<IPAddress::IPAddressImpl>();

        if (addressLength == IPAddressParserStatics::IPv4AddressBytes)
        {
            impl->SetPrivateAddress(*(uint32_t*)address);
        }
        else if (addressLength == IPAddressParserStatics::IPv6AddressBytes)
        {
            impl->_numbers.resize(IPAddressParserStatics::NumberOfLabels);

            for (int i = 0; i < IPAddressParserStatics::NumberOfLabels; i++)
            {
                impl->_numbers[i] = (uint16_t)(address[i * 2] * 256 + address[i * 2 + 1]);
            }
        }
        else
        {
            throw std::invalid_argument("bad ip address");
        }

    }

    IPAddress::~IPAddress()
    {

    }




    enum AddressFamily IPAddress::AddressFamily()
    {
        return impl->IsIPv4() ? AddressFamily::InterNetwork : AddressFamily::InterNetworkV6;
    }

    bool IPAddress::IsIPv4MappedToIPv6()
    {
        if (impl->IsIPv4())
        {
            return false;
        }
        for (int i = 0; i < 5; i++)
        {
            if (impl->_numbers[i] != 0)
            {
                return false;
            }
        }
        return (impl->_numbers[5] == 0xFFFF);
    }

    bool IPAddress::IsIPv6LinkLocal()
    {
        return impl->IsIPv6() && ((impl->_numbers[0] & 0xFFC0) == 0xFE80);
    }

    bool IPAddress::IsIPv6Multicast()
    {
        return impl->IsIPv6() && ((impl->_numbers[0] & 0xFF00) == 0xFF00);
    }

    bool IPAddress::IsIPv6SiteLocal()
    {
        return impl->IsIPv6() && ((impl->_numbers[0] & 0xFFC0) == 0xFEC0);
    }

    bool IPAddress::IsIPv6Teredo()
    {
        return impl->IsIPv6() && (impl->_numbers[0] == 0x2001) && (impl->_numbers[1] == 0);
    }

    int64_t IPAddress::ScopeId()
    {
        if (impl->IsIPv4())
        {
            throw std::invalid_argument("socket operation not support.");
        }

        return impl->GetPrivateScopeID();
    }

    void IPAddress::SetScopeId(int64_t value)
    {
        if (impl->IsIPv4())
        {
            throw std::invalid_argument("socket operation not support.");
        }
        if (value < 0 || value > 0x00000000FFFFFFFF)
        {
            throw std::out_of_range("scope id is out of range");
        }

        impl->SetPrivateScopeID((uint32_t)value);
    }

    std::vector<uint8_t> GetAddressBytes()
    {
        //if (IsIPv6())
        //{
        //    byte[] bytes = new byte[IPAddressParserStatics.IPv6AddressBytes];
        //    WriteIPv6Bytes(bytes);
        //    return bytes;
        //}
        //else
        //{
        //    byte[] bytes = new byte[IPAddressParserStatics.IPv4AddressBytes];
        //    WriteIPv4Bytes(bytes);
        //    return bytes;
        //}
        return {};
    }
}