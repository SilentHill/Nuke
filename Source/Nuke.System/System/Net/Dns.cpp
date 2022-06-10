
#include "Dns.h"

#ifdef __linux__
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#elif _WIN32
#pragma warning(disable:5105)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <ios>
#include <iosfwd>
#include <sstream>
#include <CrossPlatform/SocketApi.h>
#include <CrossPlatform/NetworkApi.h>
#include <System/Net/NetworkInformation/NetworkInterface.h>

using namespace Nuke::CrossPlatform;

namespace Nuke::RFC::RFC1035
{
#pragma pack(1)

    /*  RFC1035 #4.1.1 头段格式

         0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                      ID                       |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                    QDCOUNT                    |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                    ANCOUNT                    |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                    NSCOUNT                    |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                    ARCOUNT                    |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

    */

    
    struct DnsMessageHeader
    {
        uint16_t ID;            // 16位唯一标识符，用于校验应答与请求的配对
        uint16_t QR     : 1;    // 0 表示请求; 1 表示应答
        uint16_t OpCode : 4;    // 0 标准查询; 1 反向查询; 2 服务器状态查询; 3-15保留值;
        uint16_t AA     : 1;    // 仅应答使用，指示DnsMessageQuestion中的名字服务器是否授权
        uint16_t TC     : 1;    // 截断，指出报文过长已被截断
        uint16_t RD     : 1;    // 期望递归，若设置为1则希望递归解析
        uint16_t RA     : 1;    // 支持递归，表示服务器是否支持递归
        uint16_t Z      : 3;    // 未使用，请永远置0
        uint16_t RCODE  : 4;    /*
                                    0 正确
                                    1 报文格式错误（服务器无法理解报文）
                                    2 服务器失败（服务器自己有问题）
                                    3 名字错误（只对授权域名解析服务器有意义，指出解析的域名不存在）
                                    4 DNS不支持该查询类型
                                    6~15 未使用
                                */
        uint16_t QDCOUNT;       // 请求段中的问题记录数
        uint16_t ANCOUNT;       // 应答段中的回答记录数
        uint16_t NSCOUNT;       // 授权段中的授权记录数
        uint16_t ARCOUNT;       // 附加段中的附加记录数
    };

    /*
        RFC1035 #4.1.2 问题段格式

          0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                                               |
        /                     QNAME                     /
        /                                               /
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                     QTYPE                     |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                     QCLASS                    |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    */
    enum QTYPE : uint16_t
    {
        A            = 1 , // 主机地址
        NS           = 2 , // 授权名称服务器
        MD           = 3 , // 邮件目的地（废弃，使用MX代替）
        MF           = 4 , // 邮件转发人（废弃，使用MX代替）
        CNAME        = 5 , // 真名
        SOA          = 6 , // 授权区起始
        MB           = 7 , // 邮件域名（实验特性）
        MG           = 8 , // 邮件组成员（实验特性）
        MR           = 9 , // 邮件重命名域名（实验特性）
        NULLRR       = 10, // 空RR（实验特性）
        WKS          = 11, // 服务描述
        PTR          = 12, // 域名指针
        HINFO        = 13, // 主机信息
        MINFO        = 14, // 邮箱或邮件列表信息
        MX           = 15, // 邮件交换
        TXT          = 16, // 文本字符串
    };
    struct DnsMessageQuestion
    {
        uint8_t*    QNAME;     // 不定长，Label序列，每个label包含1字节表示后续字符串长度，0长度或空字符为结束
        uint16_t    QTYPE;     // 查询类型，
        uint16_t    QCLASS;    // 2字节代表协议类，如IN代表Internet
    };

    /*
        RFC1035 #4.1.3 资源记录格式

          0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                                               |
        /                                               /
        /                      NAME                     /
        |                                               |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                      TYPE                     |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                     CLASS                     |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                      TTL                      |
        |                                               |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                   RDLENGTH                    |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
        /                     RDATA                     /
        /                                               /
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    */
    struct DnsMessageAnswer
    {
        uint8_t* NAME;          // 不定长，RDATA包含的域名
        uint16_t TYPE;          // RDATA的含义
        uint16_t CLASS;         // RDATA的类型
        int32_t TTL;            // RDATA可以缓存的时间, 0代表不允许缓存
        uint16_t RDLENGTH;      // RDATA的长度
        uint8_t* RDATA;         // 不定长，记录数
    };

    struct DnsMessageAuthority
    {

    };

    struct DnsMessageAdditional
    {

    };


#pragma pack()

    const uint16_t DnsDefaultPort = 53;     // RFC-1035 4.2 (RFC-793 / RFC-768)
    const uint32_t MaxDnsPackLength = 512;  // RFC-1035 4.2.1

    class DnsMessage
    {
    public:
        DnsMessage() 
            :buffer(MaxDnsPackLength)
        {
            
        }

        DnsMessageHeader& Header()
        {
            return *reinterpret_cast<DnsMessageHeader*>(buffer.data());
        }

        uint8_t* Data()
        {
            return buffer.data();
        }
        uint32_t DataLength()
        {
            return buffer.size();
        }

    private:
        std::vector<uint8_t> buffer;
    };

}

namespace Nuke::System::Net
{
    
    void Dns::Resolve(const std::string& hostName, const std::string& dnsIp)
    {
        NetworkApi::GetAdapterAddresses(Sockets::AddressFamily::Unspecified);

        using namespace RFC::RFC1035;
        static std::atomic_uint32_t dnsID { 0 };

        // 准备udp sock 与 53端口
        SocketApi::InitializeSocketEnvironment();
        auto udpSocket = SocketApi::CreateUdpSocket();
        sockaddr_in targetAddr;
        targetAddr.sin_family = AF_INET;
        targetAddr.sin_port = htons(DnsDefaultPort);
        targetAddr.sin_addr.s_addr = inet_addr(dnsIp.c_str());

        // 准备最大udp dns包
        uint8_t udpBuffer[MaxDnsPackLength] = { 0 };

        // 填充DnsMessageHeader
        DnsMessage dnsMessage;
        dnsMessage.Header().ID      = dnsID++;
        dnsMessage.Header().QR      = 0;        // 查询
        dnsMessage.Header().OpCode  = 0;        // 标准查询
        dnsMessage.Header().AA      = 0;        // 无授权回答
        dnsMessage.Header().TC      = 0;        // 不可截断
        dnsMessage.Header().RD      = 1;        // 期望递归
        dnsMessage.Header().RA      = 0;        // 不可用递归
        dnsMessage.Header().Z       = 0;        // 默认保留位必0
        dnsMessage.Header().RCODE   = 0;        // 无差错
        dnsMessage.Header().QDCOUNT = htons(1); // 问题数1
        dnsMessage.Header().ANCOUNT = 0;
        dnsMessage.Header().NSCOUNT = 0;
        dnsMessage.Header().ARCOUNT = 0;

        // 问题数

        auto sentLength = SocketApi::SendTo(udpSocket, dnsIp, DnsDefaultPort, 
            dnsMessage.Data(), dnsMessage.DataLength());

        

        uint8_t udpRecvBuffer[MaxDnsPackLength] = { 0 };
        auto recvresult = SocketApi::RecvFrom(udpSocket, DnsDefaultPort, udpRecvBuffer, MaxDnsPackLength);
        return;
    }
}