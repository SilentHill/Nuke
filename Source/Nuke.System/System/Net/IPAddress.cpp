﻿
#include "_pch.h"
#include "IPAddress.h"

namespace Nuke::System::Net
{
    using namespace Nuke::System::Net::Sockets;

    namespace IPAddressParserStatics
    {
        constexpr int32_t IPv4AddressBytes = 4;
        constexpr int32_t IPv6AddressBytes = 16;
        constexpr int32_t IPv6AddressShorts = IPv6AddressBytes / 2;
        constexpr int32_t NumberOfLabels = IPv6AddressBytes / 2;
    }

    class IPAddress::IPAddressImpl
    {
    private:
        friend class IPAddress;
        uint32_t _addressOrScopeId;
        int32_t _hashCode;
        std::string _toString;
        std::unique_ptr<uint16_t[]> _numbers;

    private:
        uint32_t PrivateAddress()
        {
            return _addressOrScopeId;
        }

        void SetPrivateAddress(uint32_t value)
        {
            _toString = "";
            _hashCode = 0;
            _addressOrScopeId = value;
        }

        uint32_t PrivateScopeId()
        {
            return _addressOrScopeId;
        }

        void SetPrivateScopeId(uint32_t value)
        {
            _toString = "";
            _hashCode = 0;
            _addressOrScopeId = value;
        }

        bool IsIPv4()
        {
            return _numbers == nullptr;
        }

        bool IsIPv6()
        {
            return _numbers != nullptr;
        }

        void WriteIPv6Bytes(std::span<uint8_t> destination)
        {
            int j = 0;
            for (int i = 0; i < IPAddressParserStatics::NumberOfLabels; i++)
            {
                destination[j++] = (uint8_t)((_numbers[i] >> 8) & 0xFF);
                destination[j++] = (uint8_t)((_numbers[i]) & 0xFF);
            }
        }

        void WriteIPv4Bytes(std::span<uint8_t> destination)
        {
            uint32_t address = PrivateAddress();
            (*(uint32_t*)destination.data()) = address;
        }
    };

    IPAddress::IPAddress(uint8_t* address, int32_t addressLength)
    {
        impl = std::make_unique<IPAddress::IPAddressImpl>();

        if (addressLength == IPAddressParserStatics::IPv4AddressBytes)
        {
            impl->SetPrivateAddress(*(uint32_t*)address);
        }
        else if (addressLength == IPAddressParserStatics::IPv6AddressBytes)
        {
            impl->_numbers = std::make_unique<uint16_t[]>(IPAddressParserStatics::NumberOfLabels);

            for (int32_t i = 0; i < IPAddressParserStatics::NumberOfLabels; i++)
            {
                impl->_numbers[i] = (uint16_t)(address[i * 2] * 256 + address[i * 2 + 1]);
            }
        }
        else
        {
            throw std::invalid_argument("bad ip address");
        }

    }

    IPAddress::IPAddress(int64_t newAddress)
    {
        if (newAddress < 0 || newAddress > 0x00000000FFFFFFFF)
        {
            throw std::out_of_range("newAddress is out of range");
        }

        impl->SetPrivateAddress((uint32_t)newAddress);
    }

    IPAddress::~IPAddress()
    {

    }


    IPAddress::IPAddress(uint8_t* address, int32_t addressLength, int64_t scopeid)
    {
        if (addressLength != IPAddressParserStatics::IPv6AddressBytes)
        {
            throw std::invalid_argument("not ipv6 address length");
        }

        if (scopeid < 0 || scopeid > 0x00000000FFFFFFFF)
        {
            throw std::out_of_range("scopeid out of range");
        }

        impl->_numbers = std::make_unique<uint16_t[]>(IPAddressParserStatics::NumberOfLabels);

        for (int i = 0; i < IPAddressParserStatics::NumberOfLabels; i++)
        {
            impl->_numbers[i] = (ushort)(address[i * 2] * 256 + address[i * 2 + 1]);
        }

        impl->SetPrivateScopeId((uint32_t)scopeid);
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

        return impl->PrivateScopeId();
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

        impl->SetPrivateScopeId((uint32_t)value);
    }

    std::vector<uint8_t> IPAddress::GetAddressBytes()
    {
        if (impl->IsIPv6())
        {
            std::vector<uint8_t> bytes(IPAddressParserStatics::IPv6AddressBytes);
            impl->WriteIPv6Bytes(bytes);
            return bytes;
        }
        else
        {
            std::vector<uint8_t> bytes(IPAddressParserStatics::IPv4AddressBytes);
            impl->WriteIPv4Bytes(bytes);
            return bytes;
        }
        return {};
    }

    int64_t IPAddress::HostToNetworkOrder(int64_t host)
    {
        //return BitConverter.IsLittleEndian ? BinaryPrimitives.ReverseEndianness(host) : host;
    }

    int32_t IPAddress::HostToNetworkOrder(int32_t host)
    {
        //return BitConverter.IsLittleEndian ? BinaryPrimitives.ReverseEndianness(host) : host;
    }

    int16_t IPAddress::HostToNetworkOrder(int16_t host)
    {
        //return BitConverter.IsLittleEndian ? BinaryPrimitives.ReverseEndianness(host) : host;
    }

    int64_t IPAddress::NetworkToHostOrder(int64_t network)
    {
        return HostToNetworkOrder(network);
    }

    int32_t IPAddress::NetworkToHostOrder(int32_t network)
    {
        return HostToNetworkOrder(network);
    }

    int16_t IPAddress::NetworkToHostOrder(int16_t network)
    {
        return HostToNetworkOrder(network);
    }
}