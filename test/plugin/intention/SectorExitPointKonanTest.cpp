#include "intention/SectorExitPointKonan.h"

using ::testing::Return;
using ::testing::StrictMock;
using UKControllerPlugin::IntentionCode::SectorExitPointKonan;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        TEST(SectorExitPointKonan, ItAppliesToAllControllers)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointKonan exitPoint("KONAN", "D", SectorExitPointKonan::outSouth);

            EXPECT_TRUE(exitPoint.AppliesToController("BLA"));
        }

        TEST(SectorExitPointKonan, GetIntentionCodeReturnsCorrectCodeIfViaKoksyAtLowLevel)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            EXPECT_CALL(routeMock, GetPointsNumber()).Times(1).WillOnce(Return(1));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(1).WillOnce(Return("KOK"));

            SectorExitPointKonan exitPoint("KONAN", "D", SectorExitPointKonan::outSouth);

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 24000).compare("D1"));
        }

        TEST(SectorExitPointKonan, GetIntentionCodeReturnsCorrectCodeIfViaKoksyAtHighLevel)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            EXPECT_CALL(routeMock, GetPointsNumber()).Times(1).WillOnce(Return(1));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(1).WillOnce(Return("KOK"));

            SectorExitPointKonan exitPoint("KONAN", "D", SectorExitPointKonan::outSouth);

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("D"));
        }

        TEST(SectorExitPointKonan, GetIntentionCodeReturnsCorrectCodeIfNotViaKoksy)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            EXPECT_CALL(routeMock, GetPointsNumber()).Times(1).WillOnce(Return(2));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(1).WillOnce(Return("NOTKOK"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(1).WillOnce(Return("STILLNOTKOK"));

            SectorExitPointKonan exitPoint("KONAN", "D", SectorExitPointKonan::outSouth);

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("D"));
        }
    } // namespace IntentionCode
} // namespace UKControllerPluginTest
