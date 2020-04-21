#include "pch/pch.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "intention/SectorExitPointScottish.h"

using UKControllerPlugin::IntentionCode::SectorExitPointScottish;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        TEST(SectorExitPointScottish, ItAppliesToScottishAc)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointScottish exitPoint("LAKEY", "H7", SectorExitPointScottish::outSouth);

            EXPECT_TRUE(exitPoint.AppliesToController("SCO_CTR"));
        }

        TEST(SectorExitPointScottish, ItAppliesToScottishTc)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointScottish exitPoint("LAKEY", "H7", SectorExitPointScottish::outSouth);

            EXPECT_TRUE(exitPoint.AppliesToController("STC_A_CTR"));
        }

        TEST(SectorExitPointScottish, ItAppliesToScottishMil)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointScottish exitPoint("LAKEY", "H7", SectorExitPointScottish::outSouth);

            EXPECT_TRUE(exitPoint.AppliesToController("EGQX_APP"));
        }

        TEST(SectorExitPointScottish, ItAppliesToScottishAirfields)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointScottish exitPoint("LAKEY", "H7", SectorExitPointScottish::outSouth);

            EXPECT_TRUE(exitPoint.AppliesToController("EGPH_TWR"));
        }

        TEST(SectorExitPointScottish, ItAppliesToIrishAirfields)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointScottish exitPoint("LAKEY", "H7", SectorExitPointScottish::outSouth);

            EXPECT_TRUE(exitPoint.AppliesToController("EGAA_TWR"));
        }

        TEST(SectorExitPointScottish, GetIntentionCodeReturnsCorrectCode)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointScottish exitPoint("LAKEY", "H7", SectorExitPointScottish::outSouth);

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("H7"));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
