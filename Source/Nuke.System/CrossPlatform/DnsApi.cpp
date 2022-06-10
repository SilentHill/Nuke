
#include "../_pch.h"
#include "DnsApi.h"

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

namespace Nuke::CrossPlatform::DnsApi
{

}