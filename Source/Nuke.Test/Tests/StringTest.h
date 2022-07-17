
#pragma once

#include <gtest/gtest.h>
#include <System/String.h>

namespace Nuke::Test::Tests
{
    using namespace Nuke::System;

    class StringTest : public testing::Test
    {
    protected:
        StringTest()
        {

        }

        ~StringTest() override
        {

        }

        void SetUp() override
        {

        }

        void TearDown() override
        {

        }

    };


    TEST_F(StringTest, StringCompareTest)
    {
        String a("aaaa");
        String b("aaaa");
        using namespace Nuke::System;
        auto compareResult = String::Compare(a, b);
        EXPECT_EQ(compareResult, 0);
        a = "aaab";
        b = "aaaa";
        compareResult = String::Compare(a, b);
        EXPECT_EQ(compareResult, 1);

        a = "aaaa";
        b = "aaca";
        compareResult = String::Compare(a, b);
        EXPECT_EQ(compareResult, -1);

        a = "aaaa";
        b = "aaAa";
        compareResult = String::Compare(a, b, String::StringComparison::CurrentCultureIgnoreCase);
        EXPECT_EQ(compareResult, 0);

        a = "aaaa";
        b = "aaAa";
        compareResult = String::Compare(a, b);
        EXPECT_EQ(compareResult, 1);

        //a = "aaaa";
        //b = "aaAa";
        //compareResult = String::Compare(a, b, String::StringComparison::CurrentCulture);
        //EXPECT_EQ(compareResult, -1);
    }

}