
#include "_pch.h"

#include "Tests/DateTimeTest.h"
#include "Tests/TimerTest.h"

#include <Nuke.System.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();
    getchar();
    return testResult;

}
