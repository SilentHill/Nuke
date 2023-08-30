
#include "_pch.h"

#include "Tests/DateTimeTest.h"
#include "Tests/TimerTest.h"
#include "Tests/StringTest.h"

#include <Nuke.System.h>

#include <CrossPlatform/TimeZoneApi.h>

using namespace Nuke::System;
using namespace Nuke::System::Threading::Tasks;


Task<std::string> StartBatchJobAsync()
{
    auto task1 = TaskFactory::RunAsync([]()
    {
        return;
    });
    task1.Await();

    auto task2 = TaskFactory::RunAsync([]()
    {
        return 1;
    });
    auto value2 = task2.Await();

    auto task3 = TaskFactory::RunAsync([&]()
    {
        return std::to_string(value2);
    });


    return task3;
};


int main(int argc, char** argv)
{



    auto tzdbString = Nuke::CrossPlatform::TimeZoneApi::GetTimeZoneDataBaseJsonString();
    auto es = Convert::ToBase64String("HelloWorld", 0, 10);


    auto dv = Convert::FromBase64String(es);
    auto timeZoneIds = Nuke::CrossPlatform::TimeZoneApi::GetTimeZoneIds();

    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();
    getchar();
    return testResult;

}
