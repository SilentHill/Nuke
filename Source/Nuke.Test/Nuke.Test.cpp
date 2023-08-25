
#include "_pch.h"

#include "Tests/DateTimeTest.h"
#include "Tests/TimerTest.h"
#include "Tests/StringTest.h"

#include <Nuke.System.h>

#include <CrossPlatform/TimeZoneApi.h>

int main(int argc, char** argv)
{
    using namespace Nuke::System::Net;

    Nuke::CrossPlatform::TimeZoneApi::GetAllTimeZones();

    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();
    getchar();
    return testResult;

}
 