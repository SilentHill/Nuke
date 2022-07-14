
#include "_pch.h"

#include "Tests/DateTimeTest.h"
#include "Tests/TimerTest.h"

#include <Nuke.System.h>

int main(int argc, char** argv)
{
    using namespace Nuke::System::Net;

    //IPAddress a = IPAddress::Parse("255.255.255.255");
    IPAddress a(0xffffffff);
    bool equal = a == IPAddress::None;
    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();
    getchar();
    return testResult;

}
 