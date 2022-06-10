
#pragma once

#include <gtest/gtest.h>
#include <System/Timers/Timer.h>

namespace Nuke::Test::Tests
{
    using namespace Nuke::System;

    class TimerTest : public testing::Test
    {
    protected:
        TimerTest()
        {

        }

        ~TimerTest() override
        {

        }

        void SetUp() override
        {

        }

        void TearDown() override
        {

        }

    };


    TEST_F(TimerTest, TimerRaise)
    {
        using namespace Timers;
        Timer timer;
        timer.SetInterval(1000);
        std::atomic_int raiseCount{ 0 };
        timer.Elapsed.AddHandler([&raiseCount](Timer::ElapsedEventArgs elapsedEventArgs)
            {
                std::cout << "Timer Elapsed" << std::endl;
                raiseCount++;
            });

        timer.Start();
        std::this_thread::sleep_for(std::chrono::milliseconds(3100));
        timer.Stop();

        EXPECT_EQ(raiseCount, 3);
    }

}

