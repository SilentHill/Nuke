
#include "_pch.h"

// 测试用例集
//#include "Tests/DateTimeTest.h"
//#include "Tests/TimerTest.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();
    getchar();
    return testResult;

}
