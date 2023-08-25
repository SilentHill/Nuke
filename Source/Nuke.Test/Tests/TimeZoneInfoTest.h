
#pragma once

#include <gtest/gtest.h>
#include <System/DateTime.h>

namespace Nuke::Test::Tests
{
    using namespace Nuke::System;

    class TimeZoneInfoTest : public testing::Test
    {
    protected:
        TimeZoneInfoTest()
        {

        }

        ~TimeZoneInfoTest() override
        {

        }

        void SetUp() override
        {

        }

        void TearDown() override
        {

        }

    };


    TEST_F(TimeZoneInfoTest, TimeZoneInfoTest_Load)
    {
        auto utcNow = DateTime::UtcNow();
        auto localNow = DateTime::Now();
        TimeSpan span = localNow.Subtract(utcNow);

        EXPECT_EQ(span.TotalSeconds(), 28800);
    }

}