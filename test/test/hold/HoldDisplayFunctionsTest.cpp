#include "pch/pch.h"
#include "hold/HoldDisplayFunctions.h"
#include "hold/HoldElementDimensions.h"

using UKControllerPlugin::Hold::HoldElementDimensions;

namespace UKControllerPluginTest {
    namespace Hold {

        TEST(HoldDisplayFunctionsTest, ConvertToTcharConvertsString)
        {
            EXPECT_EQ(0, _tcscmp(L"thisisatest", UKControllerPlugin::Hold::ConvertToTchar("thisisatest")));
        }

        TEST(HoldDisplayFunctionsTest, ConvertToTcharConvertsInt)
        {
            EXPECT_EQ(0, _tcscmp(L"12345", UKControllerPlugin::Hold::ConvertToTchar(12345)));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculateProfileSelectorPosition)
        {
            RECT parent = {
                0,
                0,
                600,
                400
            };

            HoldElementDimensions expected = {
                200,
                350,
                200,
                300
            };

            EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetProfileSelectorDimensions(parent));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesGroupXPosition0)
        {
            EXPECT_EQ(10, UKControllerPlugin::Hold::GetHoldGroupX(0));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesGroupXPosition1)
        {
            EXPECT_EQ(250, UKControllerPlugin::Hold::GetHoldGroupX(1));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesGroupXPosition2)
        {
            EXPECT_EQ(490, UKControllerPlugin::Hold::GetHoldGroupX(2));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesGroupXPosition3)
        {
            EXPECT_EQ(10, UKControllerPlugin::Hold::GetHoldGroupX(3));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesGroupYPosition0)
        {
            EXPECT_EQ(10, UKControllerPlugin::Hold::GetHoldGroupY(0));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesGroupYPosition1)
        {
            EXPECT_EQ(10, UKControllerPlugin::Hold::GetHoldGroupY(1));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesGroupYPosition2)
        {
            EXPECT_EQ(10, UKControllerPlugin::Hold::GetHoldGroupY(2));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesGroupYPosition3)
        {
            EXPECT_EQ(410, UKControllerPlugin::Hold::GetHoldGroupY(3));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesIdentifierDimensions)
        {
            HoldElementDimensions expected = {
                10,
                10,
                210,
                20
            };
            EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetHoldIdentifierDimensions(10, 10));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesInboundDimensions)
        {
            HoldElementDimensions expected = {
                10,
                30,
                210,
                20
            };
            EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetHoldInboundDimensions(10, 10));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesMinumumDimensions)
        {
            HoldElementDimensions expected = {
                10,
                50,
                210,
                20
            };
            EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetHoldMinimumDimensions(10, 10));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesMaximumDimensions)
        {
            HoldElementDimensions expected = {
                10,
                70,
                210,
                20
            };
            EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetHoldMaximumDimensions(10, 10));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesTurnDimensions)
        {
            HoldElementDimensions expected = {
                10,
                90,
                210,
                20
            };
            EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetHoldTurnDimensions(10, 10));
        }

        TEST(HoldDisplayFunctionsTest, ItCalculatesTableDimensions)
        {
            HoldElementDimensions expected = {
                10,
                110,
                210,
                250
            };
            EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetHoldTableDimensions(10, 10));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
