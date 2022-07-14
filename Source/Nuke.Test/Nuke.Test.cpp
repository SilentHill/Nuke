
#include "_pch.h"

#include "Tests/DateTimeTest.h"
#include "Tests/TimerTest.h"

#include <Nuke.System.h>

int main(int argc, char** argv)
{
    using namespace Nuke::System::Net;

    IPAddress ipv4 = IPAddress::Parse("127.0.0.1");
    IPAddress ipv6 = IPAddress::Parse("fe80::252c:20ac:f36b:c811%14");

    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();
    getchar();
    return testResult;

}
 