#include "pch/pch.h"
#include "tag/TagFunction.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace Function {

        TEST(TagFunction, EqualityOperatorReturnsTrueIfIdsSame)
        {
            TagFunction first = {
                1,
                "test",
                [](EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {}
            };
            TagFunction second = {
                1,
                "test2",
                [](EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {}
            };
            EXPECT_TRUE(first == second);
        }

        TEST(TagFunction, EqualityOperatorReturnsFalseIfDifferentIds)
        {
            TagFunction first = {
                1,
                "test",
                [](EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {}
            };
            TagFunction second = {
                2,
                "test2",
                [](EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {}
            };
            EXPECT_FALSE(first == second);
        }

        TEST(TagFunction, AssignmentOperatorSetsAllFields)
        {
            int testCount = 0;
            TagFunction first = {
                1,
                "test",
                [&testCount]
                (EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {testCount++; }
            };
            TagFunction second = first;

            StrictMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;

            EXPECT_EQ(first.functionId, second.functionId);
            EXPECT_TRUE(first.description == second.description);
            first.function(mockFlightplan, mockRadarTarget, "", POINT());
            EXPECT_EQ(1, testCount);
            second.function(mockFlightplan, mockRadarTarget, "", POINT());
            EXPECT_EQ(2, testCount);
        }
    }  // namespace Function
}  // namespace UKControllerPluginTest
