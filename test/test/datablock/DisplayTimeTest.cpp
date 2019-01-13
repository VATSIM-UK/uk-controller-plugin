#include "pch/pch.h"
#include "datablock/DisplayTime.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Datablock::DisplayTime;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPluginTest {
    namespace Datablock {

        TEST(DisplayTime, FromTimestampReturnsTimeString)
        {
            DisplayTime timeDisplay;
            std::string test = timeDisplay.FromTimestamp(1403549100);
            EXPECT_TRUE("18:45" == timeDisplay.FromTimestamp(1403549100));
        }

        TEST(DisplayTime, FromSystemTimeReturnsTimeString)
        {
            DisplayTime timeDisplay;
            std::string expectedTime = date::format("%H:%M", std::chrono::system_clock::now());
            EXPECT_TRUE(expectedTime == timeDisplay.FromSystemTime());
        }

        TEST(DisplayTime, FromTimePointReturnsTimeString)
        {
            DisplayTime timeDisplay;
            std::string expectedTime = "15:23";
            EXPECT_TRUE(expectedTime == timeDisplay.FromTimePoint(HelperFunctions::GetTimeFromNumberString("1523")));
        }
    }  // namespace Datablock
}  // namespace UKControllerPluginTest
