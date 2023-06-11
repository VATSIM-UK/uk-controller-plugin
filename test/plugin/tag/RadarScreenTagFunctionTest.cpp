#include "tag/RadarScreenTagFunction.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include <gmock/gmock-nice-strict.h>

using ::testing::StrictMock;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Tag::RadarScreenTagFunction;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;

namespace UKControllerPluginTest::Tag {

    TEST(RadarScreenTagFunction, EqualityOperatorReturnsTrueIfIdsSame)
    {
        RadarScreenTagFunction first = {
            1,
            "test",
            [](EuroscopeRadarLoopbackInterface&,
               EuroScopeCFlightPlanInterface&,
               EuroScopeCRadarTargetInterface&,
               std::string,
               const POINT&,
               const RECT&) {}};
        RadarScreenTagFunction second = {
            1,
            "test2",
            [](EuroscopeRadarLoopbackInterface&,
               EuroScopeCFlightPlanInterface&,
               EuroScopeCRadarTargetInterface&,
               std::string,
               const POINT&,
               const RECT&) {}};
        EXPECT_TRUE(first == second);
    }

    TEST(RadarScreenTagFunction, EqualityOperatorReturnsFalseIfDifferentIds)
    {
        RadarScreenTagFunction first = {
            1,
            "test",
            [](EuroscopeRadarLoopbackInterface&,
               EuroScopeCFlightPlanInterface&,
               EuroScopeCRadarTargetInterface&,
               std::string,
               const POINT&,
               const RECT&) {}};
        RadarScreenTagFunction second = {
            2,
            "test2",
            [](EuroscopeRadarLoopbackInterface&,
               EuroScopeCFlightPlanInterface&,
               EuroScopeCRadarTargetInterface&,
               std::string,
               const POINT&,
               const RECT&) {}};
        EXPECT_FALSE(first == second);
    }

    TEST(RadarScreenTagFunction, AssignmentOperatorSetsAllFields)
    {
        int testCount = 0;
        RadarScreenTagFunction first = {
            1,
            "test",
            [&testCount](
                EuroscopeRadarLoopbackInterface&,
                EuroScopeCFlightPlanInterface&,
                EuroScopeCRadarTargetInterface&,
                std::string,
                const POINT&,
                const RECT&) { testCount++; }};
        RadarScreenTagFunction second = first;

        StrictMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        StrictMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;

        EXPECT_EQ(first.functionId, second.functionId);
        EXPECT_TRUE(first.description == second.description);
        first.function(mockRadarScreen, mockFlightplan, mockRadarTarget, "", POINT(), {});
        EXPECT_EQ(1, testCount);
        second.function(mockRadarScreen, mockFlightplan, mockRadarTarget, "", POINT(), {});
        EXPECT_EQ(2, testCount);
    }
} // namespace UKControllerPluginTest::Tag
