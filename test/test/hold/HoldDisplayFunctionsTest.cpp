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

        TEST(HoldDisplayFunctionsTest, ItReturnsCorrectDisplayLevelUnder100)
        {
            std::wstring expected = L"000";
            EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetLevelDisplayString(50));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsCorrectDisplayLevelUnder1000)
        {
            std::wstring expected = L"005";
            EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetLevelDisplayString(500));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsCorrectDisplayLevelUnder10000)
        {
            std::wstring expected = L"051";
            EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetLevelDisplayString(5100));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsCorrectDisplayLevelAbove10000)
        {
            std::wstring expected = L"351";
            EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetLevelDisplayString(35100));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsTheNearestLevelWhenWellBelowNoClimb)
        {
            EXPECT_EQ(8000, UKControllerPlugin::Hold::GetOccupiedLevel(7501, 0));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsTheNearestLevelWhenSlightlyBelowNoClimb)
        {
            EXPECT_EQ(8000, UKControllerPlugin::Hold::GetOccupiedLevel(7898, 0));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsTheNearestLevelWhenSlightlyAboveNoClimb)
        {
            EXPECT_EQ(8000, UKControllerPlugin::Hold::GetOccupiedLevel(8213, 0));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsTheNearestLevelWhenWellAboveNoClimb)
        {
            EXPECT_EQ(8000, UKControllerPlugin::Hold::GetOccupiedLevel(8499, 0));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsNearestLevelWhenClimbingButNotBeyondThreshold)
        {
            EXPECT_EQ(8000, UKControllerPlugin::Hold::GetOccupiedLevel(8399, 301));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsNextLevelWhenClimbingBeyondThreshold)
        {
            EXPECT_EQ(9000, UKControllerPlugin::Hold::GetOccupiedLevel(8401, 301));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsNextLevelWhenClimbingOnThreshold)
        {
            EXPECT_EQ(9000, UKControllerPlugin::Hold::GetOccupiedLevel(8400, 301));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsNearestLevelWhenDescendingButNotBeyondThreshold)
        {
            EXPECT_EQ(8000, UKControllerPlugin::Hold::GetOccupiedLevel(7601, -301));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsNextLevelWhenDescendingOnThreshold)
        {
            EXPECT_EQ(7000, UKControllerPlugin::Hold::GetOccupiedLevel(7600, -301));
        }

        TEST(HoldDisplayFunctionsTest, ItReturnsNextLevelWhenDescendingBeyondThreshold)
        {
            EXPECT_EQ(7000, UKControllerPlugin::Hold::GetOccupiedLevel(7599, -301));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
