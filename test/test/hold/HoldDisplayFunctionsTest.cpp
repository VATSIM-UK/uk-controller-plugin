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

    }  // namespace Hold
}  // namespace UKControllerPluginTest
