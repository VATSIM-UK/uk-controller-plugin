#include "hold/CompareHolds.h"
#include "hold/HoldDisplayFunctions.h"

using UKControllerPlugin::Hold::CompareHolds;
using UKControllerPlugin::Hold::HoldingData;

namespace UKControllerPluginTest::Hold {

    TEST(HoldDisplayFunctionsTest, ConvertToTcharConvertsString)
    {
        EXPECT_EQ(L"thisisatest", UKControllerPlugin::Hold::ConvertToTchar("thisisatest"));
    }

    TEST(HoldDisplayFunctionsTest, ConverFromTcharConvertsToString)
    {
        wchar_t input[12] = L"thisisatest";                                              // NOLINT
        EXPECT_TRUE("thisisatest" == UKControllerPlugin::Hold::ConvertFromTchar(input)); // NOLINT
    }

    TEST(HoldDisplayFunctionsTest, ConvertToTcharConvertsInt)
    {
        EXPECT_EQ(L"12345", UKControllerPlugin::Hold::ConvertToTchar(12345));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsCorrectDisplayLevelWithRoundingUp)
    {
        std::wstring expected = L"001";
        EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetLevelDisplayString(50));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsCorrectDisplayLevelUnder100RoundDown)
    {
        std::wstring expected = L"000";
        EXPECT_TRUE(expected == UKControllerPlugin::Hold::GetLevelDisplayString(49));
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

    TEST(HoldDisplayFunctionsTest, ItReturnsRowToDisplayInHoldBasedOnAltitudeMax)
    {
        EXPECT_EQ(0, UKControllerPlugin::Hold::GetDisplayRow(15000, 15000));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsRowToDisplayInHoldBasedOnAltitudeNotMaxBoundary)
    {
        EXPECT_EQ(1, UKControllerPlugin::Hold::GetDisplayRow(15000, 14000));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsRowToDisplayInHoldBasedOnAltitudeNotMax)
    {
        EXPECT_EQ(2, UKControllerPlugin::Hold::GetDisplayRow(15000, 13000));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsTimeInHoldLessThan10Minutes)
    {
        std::chrono::system_clock::time_point testTime = std::chrono::system_clock::now() - std::chrono::minutes(5);
        EXPECT_TRUE(L"5m" == UKControllerPlugin::Hold::GetTimeInHoldDisplayString(testTime));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsTimeInHoldGreaterThan10Minutes)
    {
        std::chrono::system_clock::time_point testTime = std::chrono::system_clock::now() - std::chrono::minutes(13);
        EXPECT_TRUE(L"13m" == UKControllerPlugin::Hold::GetTimeInHoldDisplayString(testTime));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsTimeInHoldSeconds)
    {
        std::chrono::system_clock::time_point testTime = std::chrono::system_clock::now() - std::chrono::seconds(13);
        EXPECT_TRUE(L"0m" == UKControllerPlugin::Hold::GetTimeInHoldDisplayString(testTime));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsVerticalSpeedDirectionPositive)
    {
        EXPECT_EQ(1, UKControllerPlugin::Hold::GetVerticalSpeedDirection(500));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsVerticalSpeedDirectionPositiveBoundary)
    {
        EXPECT_EQ(1, UKControllerPlugin::Hold::GetVerticalSpeedDirection(300));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsVerticalSpeedDirectionNoneBoundary)
    {
        EXPECT_EQ(0, UKControllerPlugin::Hold::GetVerticalSpeedDirection(299));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsVerticalSpeedDirectionNone)
    {
        EXPECT_EQ(0, UKControllerPlugin::Hold::GetVerticalSpeedDirection(0));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsVerticalSpeedDirectionNoneNegativeBoundary)
    {
        EXPECT_EQ(0, UKControllerPlugin::Hold::GetVerticalSpeedDirection(-299));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsVerticalSpeedDirectionNegativeBoundary)
    {
        EXPECT_EQ(-1, UKControllerPlugin::Hold::GetVerticalSpeedDirection(-300));
    }

    TEST(HoldDisplayFunctionsTest, ItReturnsVerticalSpeedDirectionNegative)
    {
        EXPECT_EQ(-1, UKControllerPlugin::Hold::GetVerticalSpeedDirection(-500));
    }
} // namespace UKControllerPluginTest::Hold
