
#include "_pch.h"
#include "IPAddress.h"

#include <System/BitConverter.h>
#include <System/Buffers/Binary/BinaryPrimitives.h>

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
    class IPv4AddressHelper
    {
        static const int64_t Invalid = -1;
        static const int64_t MaxIPv4Value = UINT32_MAX; // the native parser cannot handle MaxIPv4Value, only MaxIPv4Value - 1
        static const int32_t Octal = 8;
        static const int32_t Decimal = 10;
        static const int32_t Hex = 16;

        static const int32_t NumberOfLabels = 4;

        // Only called from the IPv6Helper, only parse the canonical format
        static int32_t ParseHostNumber(ReadOnlySpan<char> str, int32_t start, int32_t end)
        {
            std::byte numbers[NumberOfLabels];

            for (int32_t i = 0; i < NumberOfLabels; ++i)
            {
                int32_t b = 0;
                char ch;

                for (; (start < end) && (ch = str[start]) != '.' && ch != ':'; ++start)
                {
                    b = (b * 10) + ch - '0';
                }

                numbers[i] = (byte)b;
                ++start;
            }

            return BinaryPrimitives::ReadInt32BigEndian(numbers);
        }

        static bool IsValid(char* name, int32_t start, int32_t& end, bool allowIPv6, bool notImplicitFile, bool unknownScheme)
        {
            // IPv6 can only have canonical IPv4 embedded. Unknown schemes will not attempt parsing of non-canonical IPv4 addresses.
            if (allowIPv6 || unknownScheme)
            {
                return IsValidCanonical(name, start, ref end, allowIPv6, notImplicitFile);
            }
            else
            {
                return ParseNonCanonical(name, start, ref end, notImplicitFile) != Invalid;
            }
        }

        static bool IsValidCanonical(char* name, int32_t start, int32_t& end, bool allowIPv6, bool notImplicitFile)
        {
            int32_t dots = 0;
            int32_t number = 0;
            bool haveNumber = false;
            bool firstCharIsZero = false;

            while (start < end)
            {
                char ch = name[start];
                if (allowIPv6)
                {
                    // for ipv4 inside ipv6 the terminator is either ScopeId, prefix or ipv6 terminator
                    if (ch == ']' || ch == '/' || ch == '%')
                        break;
                }
                else if (ch == '/' || ch == '\\' || (notImplicitFile && (ch == ':' || ch == '?' || ch == '#')))
                {
                    break;
                }

                if (char.IsAsciiDigit(ch))
                {
                    if (!haveNumber && (ch == '0'))
                    {
                        if ((start + 1 < end) && name[start + 1] == '0')
                        {
                            // 00 is not allowed as a prefix.
                            return false;
                        }

                        firstCharIsZero = true;
                    }

                    haveNumber = true;
                    number = number * 10 + (name[start] - '0');
                    if (number > 255)
                    {
                        return false;
                    }
                }
                else if (ch == '.')
                {
                    if (!haveNumber || (number > 0 && firstCharIsZero))
                    {
                        // 0 is not allowed to prefix a number.
                        return false;
                    }
                    ++dots;
                    haveNumber = false;
                    number = 0;
                    firstCharIsZero = false;
                }
                else
                {
                    return false;
                }
                ++start;
            }
            bool res = (dots == 3) && haveNumber;
            if (res)
            {
                end = start;
            }
            return res;
        }

        static int64_t ParseNonCanonical(char* name, int32_t start, int32_t& end, bool notImplicitFile)
        {
            int32_t numberBase = Decimal;
            char ch;
            int64_t parts[4];
            int64_t currentValue = 0;
            bool atLeastOneChar = false;

            // Parse one dotted section at a time
            int32_t dotCount = 0; // Limit 3
            int32_t current = start;
            for (; current < end; current++)
            {
                ch = name[current];
                currentValue = 0;

                // Figure out what base this section is in
                numberBase = Decimal;
                if (ch == '0')
                {
                    numberBase = Octal;
                    current++;
                    atLeastOneChar = true;
                    if (current < end)
                    {
                        ch = name[current];
                        if (ch == 'x' || ch == 'X')
                        {
                            numberBase = Hex;
                            current++;
                            atLeastOneChar = false;
                        }
                    }
                }

                // Parse this section
                for (; current < end; current++)
                {
                    ch = name[current];
                    int32_t digitValue;

                    if ((numberBase == Decimal || numberBase == Hex) && char.IsAsciiDigit(ch))
                    {
                        digitValue = ch - '0';
                    }
                    else if (numberBase == Octal && '0' <= ch && ch <= '7')
                    {
                        digitValue = ch - '0';
                    }
                    else if (numberBase == Hex && 'a' <= ch && ch <= 'f')
                    {
                        digitValue = ch + 10 - 'a';
                    }
                    else if (numberBase == Hex && 'A' <= ch && ch <= 'F')
                    {
                        digitValue = ch + 10 - 'A';
                    }
                    else
                    {
                        break; // Invalid/terminator
                    }

                    currentValue = (currentValue * numberBase) + digitValue;

                    if (currentValue > MaxIPv4Value) // Overflow
                    {
                        return Invalid;
                    }

                    atLeastOneChar = true;
                }

                if (current < end && name[current] == '.')
                {
                    if (dotCount >= 3 // Max of 3 dots and 4 segments
                        || !atLeastOneChar // No empty segmets: 1...1
                                           // Only the last segment can be more than 255 (if there are less than 3 dots)
                        || currentValue > 0xFF)
                    {
                        return Invalid;
                    }
                    parts[dotCount] = currentValue;
                    dotCount++;
                    atLeastOneChar = false;
                    continue;
                }
                // We don't get here unless We find an invalid character or a terminator
                break;
            }

            // Terminators
            if (!atLeastOneChar)
            {
                return Invalid;  // Empty trailing segment: 1.1.1.
            }
            else if (current >= end)
            {
                // end of string, allowed
            }
            else if ((ch = name[current]) == '/' || ch == '\\' || (notImplicitFile && (ch == ':' || ch == '?' || ch == '#')))
            {
                end = current;
            }
            else
            {
                // not a valid terminating character
                return Invalid;
            }

            parts[dotCount] = currentValue;

            // Parsed, reassemble and check for overflows
            switch (dotCount)
            {
            case 0: // 0xFFFFFFFF
                if (parts[0] > MaxIPv4Value)
                {
                    return Invalid;
                }
                return parts[0];
            case 1: // 0xFF.0xFFFFFF
                if (parts[1] > 0xffffff)
                {
                    return Invalid;
                }
                return (parts[0] << 24) | (parts[1] & 0xffffff);
            case 2: // 0xFF.0xFF.0xFFFF
                if (parts[2] > 0xffff)
                {
                    return Invalid;
                }
                return (parts[0] << 24) | ((parts[1] & 0xff) << 16) | (parts[2] & 0xffff);
            case 3: // 0xFF.0xFF.0xFF.0xFF
                if (parts[3] > 0xff)
                {
                    return Invalid;
                }
                return (parts[0] << 24) | ((parts[1] & 0xff) << 16) | ((parts[2] & 0xff) << 8) | (parts[3] & 0xff);
            default:
                return Invalid;
            }
        }
    };

    class IPAddressParser
    {
    private:
        friend class IPAddress;

        static const int32_t MaxIPv4StringLength = 15; // 4 numbers separated by 3 periods, with up to 3 digits per number

        static IPAddress* Parse(std::string_view ipSpan, bool tryParse)
        {
            int64_t address;
            if (ipSpan.find(':') != std::string_view::npos)
            {
                uint16_t numbers[IPAddressParserStatics::IPv6AddressShorts];
                uint32_t scope;
                if (Ipv6StringToAddress(ipSpan, numbers, IPAddressParserStatics::IPv6AddressShorts, scope))
                {
                    return new IPAddress(numbers, scope);
                }
            }
            else if (Ipv4StringToAddress(ipSpan, address))
            {
                return new IPAddress(address);
            }

            if (tryParse)
            {
                return nullptr;
            }

            throw std::invalid_argument("bad address");
        }

        static std::string IPv4AddressToString(uint32_t address)
        {
            char addressString[MaxIPv4StringLength];
            int32_t charsWritten = IPv4AddressToStringHelper(address, addressString);
            return std::string(addressString, charsWritten);
        }

        static void IPv4AddressToString(uint32_t address, std::string& destination)
        {
            char addressString[MaxIPv4StringLength];
            int32_t charsWritten = IPv4AddressToStringHelper(address, addressString);
            destination.append(addressString, charsWritten);
        }

        static bool IPv4AddressToString(uint32_t address, std::span<char> formatted, int32_t& charsWritten)
        {
            if (formatted.size() < MaxIPv4StringLength)
            {
                charsWritten = 0;
                return false;
            }

            charsWritten = IPv4AddressToStringHelper(address, formatted);


            return true;
        }

        static int32_t IPv4AddressToStringHelper(uint32_t address, char* addressString)
        {
            int32_t offset = 0;
            address = (uint32_t)IPAddress::NetworkToHostOrder((int32_t)address);

            FormatIPv4AddressNumber((int32_t)((address >> 24) & 0xFF), addressString, offset);
            addressString[offset++] = '.';
            FormatIPv4AddressNumber((int32_t)((address >> 16) & 0xFF), addressString, offset);
            addressString[offset++] = '.';
            FormatIPv4AddressNumber((int32_t)((address >> 8) & 0xFF), addressString, offset);
            addressString[offset++] = '.';
            FormatIPv4AddressNumber((int32_t)(address & 0xFF), addressString, offset);

            return offset;
        }

        static std::string IPv6AddressToString(uint16_t[] address, uint32_t scopeId)
        {
            Debug.Assert(address != null);
            Debug.Assert(address.Length == IPAddressParserStatics.IPv6AddressShorts);

            StringBuilder buffer = IPv6AddressToStringHelper(address, scopeId);

            return StringBuilderCache.GetStringAndRelease(buffer);
        }

        static bool IPv6AddressToString(uint16_t[] address, uint32_t scopeId, std::span<char> destination, out int32_t charsWritten)
        {
            Debug.Assert(address != null);
            Debug.Assert(address.Length == IPAddressParserStatics.IPv6AddressShorts);

            StringBuilder buffer = IPv6AddressToStringHelper(address, scopeId);

            if (destination.Length < buffer.Length)
            {
                StringBuilderCache.Release(buffer);
                charsWritten = 0;
                return false;
            }

            buffer.CopyTo(0, destination, buffer.Length);
            charsWritten = buffer.Length;

            StringBuilderCache.Release(buffer);

            return true;
        }

        static StringBuilder IPv6AddressToStringHelper(uint16_t[] address, uint32_t scopeId)
        {
            const int32_t INET6_ADDRSTRLEN = 65;
            StringBuilder buffer = StringBuilderCache.Acquire(INET6_ADDRSTRLEN);

            if (IPv6AddressHelper.ShouldHaveIpv4Embedded(address))
            {
                // We need to treat the last 2 ushorts as a 4-byte IPv4 address,
                // so output the first 6 ushorts normally, followed by the IPv4 address.
                AppendSections(address, 0, 6, buffer);
                if (buffer[buffer.Length - 1] != ':')
                {
                    buffer.Append(':');
                }
                IPv4AddressToString(ExtractIPv4Address(address), buffer);
            }
            else
            {
                // No IPv4 address.  Output all 8 sections as part of the IPv6 address
                // with normal formatting rules.
                AppendSections(address, 0, 8, buffer);
            }

            // If there's a scope ID, append it.
            if (scopeId != 0)
            {
                buffer.Append('%').Append(scopeId);
            }

            return buffer;
        }

        static void FormatIPv4AddressNumber(int32_t number, char* addressString, int32_t& offset)
        {
            // Math.DivRem has no overload for byte, assert here for safety
            Debug.Assert(number < 256);

            offset += number > 99 ? 3 : number > 9 ? 2 : 1;

            int32_t i = offset;
            do
            {
                int32_t rem;
                number = Math.DivRem(number, 10, rem);
                addressString[--i] = (char)('0' + rem);
            } while (number != 0);
        }

        static bool Ipv4StringToAddress(std::span<char> ipSpan, int64_t& address)
        {
            int32_t end = ipSpan.size();
            int64_t tmpAddr;


            tmpAddr = IPv4AddressHelper::ParseNonCanonical(ipSpan, 0, ref end, notImplicitFile: true);


            if (tmpAddr != IPv4AddressHelper.Invalid && end == ipSpan.Length)
            {
                // IPv4AddressHelper.ParseNonCanonical returns the bytes in host order.
                // Convert to network order and return success.
                address = (uint32_t)IPAddress.HostToNetworkOrder(unchecked((int32_t)tmpAddr));
                return true;
            }
            else
            {
                // Failed to parse the address.
                address = 0;
                return false;
            }
        }

        static bool Ipv6StringToAddress(std::string_view ipSpan, std::span<uint16_t> numbers, int32_t numbersLength, uint32_t& scope)
        {
            int32_t end = ipSpan.size();

            bool isValid = false;
            fixed(char* ipStringPtr = &MemoryMarshal.GetReference(ipSpan))
            {
                isValid = IPv6AddressHelper.IsValidStrict(ipStringPtr, 0, ref end);
            }
            if (isValid || (end != ipSpan.Length))
            {
                std::string scopeId = null;
                IPv6AddressHelper.Parse(ipSpan, numbers, 0, scopeId);

                if (scopeId ? .Length > 1)
                {
                    if (uint32_t.TryParse(scopeId.AsSpan(1), NumberStyles.None, CultureInfo.InvariantCulture, out scope))
                    {
                        return true; // scopeId is a numeric value
                    }
                    uint32_t interfaceIndex = InterfaceInfoPal.InterfaceNameToIndex(scopeId);
                    if (interfaceIndex > 0)
                    {
                        scope = interfaceIndex;
                        return true; // scopeId is a known interface name
                    }
                    // scopeId is an unknown interface name
                }
                // scopeId is not presented
                scope = 0;
                return true;
            }

            scope = 0;
            return false;
        }

        /// <summary>
        /// Appends each of the numbers in address in indexed range [fromInclusive, toExclusive),
        /// while also replacing the longest sequence of 0s found in that range with "::", as int64_t
        /// as the sequence is more than one 0.
        /// </summary>
        static void AppendSections(uint16_t[] address, int32_t fromInclusive, int32_t toExclusive, StringBuilder buffer)
        {
            // Find the longest sequence of zeros to be combined into a "::"
            std::span<uint16_t> addressSpan = new std::span<uint16_t>(address, fromInclusive, toExclusive - fromInclusive);
            (int32_t zeroStart, int32_t zeroEnd) = IPv6AddressHelper.FindCompressionRange(addressSpan);
            bool needsColon = false;

            // Output all of the numbers before the zero sequence
            for (int32_t i = fromInclusive; i < zeroStart; i++)
            {
                if (needsColon)
                {
                    buffer.Append(':');
                }
                needsColon = true;
                AppendHex(address[i], buffer);
            }

            // Output the zero sequence if there is one
            if (zeroStart >= 0)
            {
                buffer.Append("::");
                needsColon = false;
                fromInclusive = zeroEnd;
            }

            // Output everything after the zero sequence
            for (int32_t i = fromInclusive; i < toExclusive; i++)
            {
                if (needsColon)
                {
                    buffer.Append(':');
                }
                needsColon = true;
                AppendHex(address[i], buffer);
            }
        }

        /// <summary>Appends a number as hexadecimal (without the leading "0x") to the StringBuilder.</summary>
        static void AppendHex(uint16_t value, StringBuilder buffer)
        {
            if ((value & 0xF000) != 0)
                buffer.Append(HexConverter.ToCharLower(value >> 12));

            if ((value & 0xFF00) != 0)
                buffer.Append(HexConverter.ToCharLower(value >> 8));

            if ((value & 0xFFF0) != 0)
                buffer.Append(HexConverter.ToCharLower(value >> 4));

            buffer.Append(HexConverter.ToCharLower(value));
        }

        /// <summary>Extracts the IPv4 address from the end of the IPv6 address byte array.</summary>
        static uint32_t ExtractIPv4Address(uint16_t[] address)
        {
            uint32_t ipv4address = (uint32_t)address[6] << 16 | (uint32_t)address[7];
            return (uint32_t)IPAddress::HostToNetworkOrder(unchecked((int32_t)ipv4address));
        }
    };

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
            int32_t j = 0;
            for (int32_t i = 0; i < IPAddressParserStatics::NumberOfLabels; i++)
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

        for (int32_t i = 0; i < IPAddressParserStatics::NumberOfLabels; i++)
        {
            impl->_numbers[i] = (uint16_t)(address[i * 2] * 256 + address[i * 2 + 1]);
        }

        impl->SetPrivateScopeId((uint32_t)scopeid);
    }

    IPAddress::IPAddress(std::span<std::byte> address, int64_t scopeid)
    {
        if (address.size() != IPAddressParserStatics::IPv6AddressBytes)
        {
            throw std::invalid_argument("not ipv6 address length");
        }

        if (scopeid < 0 || scopeid > 0x00000000FFFFFFFF)
        {
            throw std::out_of_range("scopeid out of range");
        }

        impl->_numbers = std::make_unique<uint16_t[]>(IPAddressParserStatics::NumberOfLabels);

        for (int32_t i = 0; i < IPAddressParserStatics::NumberOfLabels; i++)
        {
            impl->_numbers[i] = (uint16_t)(address[i * 2] * 256 + address[i * 2 + 1]);
        }

        impl->SetPrivateScopeId((uint32_t)scopeid);
    }

    IPAddress::IPAddress(const IPAddress& ipaddress)
    {

    }
    std::string IPAddress::ToString()
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
        for (int32_t i = 0; i < 5; i++)
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

    using namespace Nuke::System::Buffers::Binary;
    int64_t IPAddress::HostToNetworkOrder(int64_t host)
    {
        return BitConverter::IsLittleEndian ? BinaryPrimitives::ReverseEndianness(host) : host;
    }

    int32_t IPAddress::HostToNetworkOrder(int32_t host)
    {
        return BitConverter::IsLittleEndian ? BinaryPrimitives::ReverseEndianness(host) : host;
    }

    int16_t IPAddress::HostToNetworkOrder(int16_t host)
    {
        return BitConverter::IsLittleEndian ? BinaryPrimitives::ReverseEndianness(host) : host;
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