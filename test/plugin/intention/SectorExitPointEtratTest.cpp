#include "pch/pch.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "intention/SectorExitPointEtrat.h"

using UKControllerPlugin::IntentionCode::SectorExitPointEtrat;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        TEST(SectorExitPointEtrat, ItAppliesToAllControllers)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointEtrat exitPoint("ETRAT", "E", SectorExitPointEtrat::outSouth);

            EXPECT_TRUE(exitPoint.AppliesToController("BLA"));
        }

        TEST(SectorExitPointEtrat, GetIntentionCodeReturnsCorrectSecondaryCode)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointEtrat exitPoint("ETRAT", "E", SectorExitPointEtrat::outSouth);

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("E2"));
        }

        TEST(SectorExitPointEtrat, GetIntentionCodeReturnsCorrectSecondaryBoundary)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointEtrat exitPoint("ETRAT", "E", SectorExitPointEtrat::outSouth);

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 27000).compare("E2"));
        }

        TEST(SectorExitPointEtrat, GetIntentionCodeReturnsCorrectPrimaryCodeBoundary)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointEtrat exitPoint("ETRAT", "E", SectorExitPointEtrat::outSouth);

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 27001).compare("E"));
        }

        TEST(SectorExitPointEtrat, GetIntentionCodeReturnsCorrectPrimaryCode)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointEtrat exitPoint("ETRAT", "E", SectorExitPointEtrat::outSouth);

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("E"));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
