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
    }  // namespace Hold
}  // namespace UKControllerPluginTest
