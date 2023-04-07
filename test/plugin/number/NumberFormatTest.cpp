#include "number/NumberFormat.h"

using UKControllerPlugin::Number::To1Dp;
using UKControllerPlugin::Number::To1DpWide;

namespace UKControllerPluginTest::Number {
    class NumberFormatTest : public testing::Test
    {
    };

    TEST_F(NumberFormatTest, ItFormatsToOneDecimalPlace)
    {
        EXPECT_EQ("1.1", To1Dp(1.1));
    }

    TEST_F(NumberFormatTest, ItFormatsToOneDecimalPlaceExtraZeros)
    {
        EXPECT_EQ("1.1", To1Dp(1.100000000000));
    }

    TEST_F(NumberFormatTest, ItFormatsToOneDecimalPlaceRoundDown)
    {
        EXPECT_EQ("1.1", To1Dp(1.149999999));
    }

    TEST_F(NumberFormatTest, ItFormatsToOneDecimalPlaceRoundUp)
    {
        EXPECT_EQ("1.1", To1Dp(1.0500000000));
    }

    TEST_F(NumberFormatTest, ItFormatsToOneDecimalPlaceWide)
    {
        EXPECT_EQ(L"1.1", To1DpWide(1.1));
    }

    TEST_F(NumberFormatTest, ItFormatsToOneDecimalPlaceExtraZerosWide)
    {
        EXPECT_EQ(L"1.1", To1DpWide(1.100000000000));
    }

    TEST_F(NumberFormatTest, ItFormatsToOneDecimalPlaceRoundDownWide)
    {
        EXPECT_EQ(L"1.1", To1DpWide(1.149999999));
    }

    TEST_F(NumberFormatTest, ItFormatsToOneDecimalPlaceRoundUpWide)
    {
        EXPECT_EQ(L"1.1", To1DpWide(1.0500000000));
    }
} // namespace UKControllerPluginTest::Number
