#include "euroscope/RadarScreenCallbackFunction.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Euroscope::RadarScreenCallbackFunction;

namespace UKControllerPluginTest::Euroscope {

    TEST(RadarScreenCallbackFunction, EqualityOperatorReturnsTrueIfIdsSame)
    {
        RadarScreenCallbackFunction first = {
            1, "test", [](int, EuroscopeRadarLoopbackInterface&, const std::string&, const POINT&, const RECT&) {}};
        RadarScreenCallbackFunction second = {
            1, "test2", [](int, EuroscopeRadarLoopbackInterface&, const std::string&, const POINT&, const RECT&) {}};
        EXPECT_TRUE(first == second);
    }

    TEST(RadarScreenCallbackFunction, EqualityOperatorReturnsFalseIfDifferentIds)
    {
        RadarScreenCallbackFunction first = {
            1, "test", [](int, EuroscopeRadarLoopbackInterface&, const std::string&, const POINT&, const RECT&) {}};
        RadarScreenCallbackFunction second = {
            2, "test2", [](int, EuroscopeRadarLoopbackInterface&, const std::string&, const POINT&, const RECT&) {}};
        EXPECT_FALSE(first == second);
    }

    TEST(RadarScreenCallbackFunction, AssignmentOperatorSetsAllFields)
    {
        testing::NiceMock<UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
        int testCount = 0;
        RadarScreenCallbackFunction first = {
            1,
            "test",
            [&testCount](int, EuroscopeRadarLoopbackInterface&, const std::string&, const POINT&, const RECT&) {
                testCount++;
            }};
        RadarScreenCallbackFunction second = first;

        EXPECT_EQ(first.functionId, second.functionId);
        EXPECT_TRUE(first.description == second.description);
        first.function(0, mockRadarScreen, "test", {}, {});
        EXPECT_EQ(1, testCount);
        second.function(0, mockRadarScreen, "test", {}, {});
        EXPECT_EQ(2, testCount);
    }
} // namespace UKControllerPluginTest::Euroscope
