
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
    public:
        IPAddressImpl(uint8_t* address, int32_t addressLength)
        {
            if (addressLength == IPAddressParserStatics::IPv4AddressBytes)
            {
                SetPrivateAddress(*(uint32_t*)address);
            }
            else if (addressLength == IPAddressParserStatics::IPv6AddressBytes)
            {
                _numbers.resize(IPAddressParserStatics::NumberOfLabels);

                for (int i = 0; i < IPAddressParserStatics::NumberOfLabels; i++)
                {
                    _numbers[i] = (uint16_t)(address[i * 2] * 256 + address[i * 2 + 1]);
                }
            }
            else
            {
                throw std::invalid_argument("bad ip address");
            }
        }

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

        enum AddressFamily AddressFamily()
        {
            return IsIPv4() ? AddressFamily::InterNetwork : AddressFamily::InterNetworkV6;
        }

        bool IsIPv4MappedToIPv6()
        {
            if (IsIPv4())
            {
                return false;
            }
            for (int i = 0; i < 5; i++)
            {
                if (_numbers[i] != 0)
                {
                    return false;
                }
            }
            return (_numbers[5] == 0xFFFF);
        }

        bool IsIPv6LinkLocal()
        {
            return IsIPv6() && ((_numbers[0] & 0xFFC0) == 0xFE80);
        }

        bool IsIPv6Multicast()
        {
            return IsIPv6() && ((_numbers[0] & 0xFF00) == 0xFF00);
        }

        bool IsIPv6SiteLocal()
        {
            return IsIPv6() && ((_numbers[0] & 0xFFC0) == 0xFEC0);
        }

        bool IsIPv6Teredo()
        {
            return IsIPv6() && (_numbers[0] == 0x2001) && (_numbers[1] == 0);
        }

        int64_t ScopeId()
        {
            if (IsIPv4())
            {
                throw std::invalid_argument("socket operation not support.");
            }

            return GetPrivateScopeID();
        }
        void SetScopeId(int64_t value)
        {
            if (IsIPv4())
            {
                throw std::invalid_argument("socket operation not support.");
            }
            if (value < 0 || value > 0x00000000FFFFFFFF)
            {
                throw std::out_of_range("scope id is out of range");
            }

            SetPrivateScopeID((uint32_t)value);
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

        uint32_t _addressOrScopeId;
        int32_t _hashCode;
        std::string _toString;
        std::vector<uint16_t> _numbers;
    };

    // IPAddress接口转发
    IPAddress::IPAddress(uint8_t* address, int32_t addressLength)
    {
        impl = std::make_unique<IPAddress::IPAddressImpl>(address, addressLength);
    }

    IPAddress::~IPAddress()
    {

    }

    AddressFamily IPAddress::AddressFamily()
    {
        return impl->AddressFamily();
    }

    bool IPAddress::IsIPv4MappedToIPv6()
    {
        return impl->IsIPv4MappedToIPv6();
    }

    bool IPAddress::IsIPv6LinkLocal()
    {
        return impl->IsIPv6LinkLocal();
    }

    bool IPAddress::IsIPv6Multicast()
    {
        return impl->IsIPv6Multicast();
    }

    bool IPAddress::IsIPv6SiteLocal()
    {
        return impl->IsIPv6SiteLocal();
    }

    bool IPAddress::IsIPv6Teredo()
    {
        return impl->IsIPv6Teredo();
    }

    int64_t IPAddress::ScopeId()
    {
        return impl->ScopeId();
    }
    void IPAddress::SetScopeId(int64_t value)
    {
        return impl->SetScopeId(value);
    }

    std::vector<uint8_t> IPAddress::GetAddressBytes()
    {
        return impl->GetAddressBytes();
    }
}