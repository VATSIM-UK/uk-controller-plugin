#include "pch/pch.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "intention/SectorExitPointLelna.h"
#include "intention/IntentionCodeGenerator.h"

using UKControllerPlugin::IntentionCode::SectorExitPointLelna;
using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using ::testing::Return;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace IntentionCode {
        TEST(SectorExitPointLelna, GetIntentionCodeReturnsCorrectDefaultCodeLelna)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointLelna exitPoint("LELNA", "H2", SectorExitPointLelna::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(3)
                .WillRepeatedly(Return(2));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(3)
                .WillRepeatedly(Return("LELNA"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(3)
                .WillRepeatedly(Return("LFMN"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H2"));
        }

        TEST(SectorExitPointLelna, GetIntentionCodeReturnsCorrectSecondaryCodeDinard)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointLelna exitPoint("LELNA", "H2", SectorExitPointLelna::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(3));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(3)
                .WillRepeatedly(Return("LELNA"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(1)
                .WillRepeatedly(Return("DIN"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H2"));
        }

        TEST(SectorExitPointLelna, GetIntentionCodeReturnsCorrectSecondaryCodeArree)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointLelna exitPoint("LELNA", "H2", SectorExitPointLelna::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(3));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(3)
                .WillRepeatedly(Return("LELNA"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(3)
                .WillRepeatedly(Return("ARE"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H3"));
        }

        TEST(SectorExitPointLelna, GetIntentionCodeReturnsCorrectSecondaryCodeDomok)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointLelna exitPoint("LELNA", "H2", SectorExitPointLelna::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(3));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(3)
                .WillRepeatedly(Return("LELNA"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(2)
                .WillRepeatedly(Return("DOMOK"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H3"));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
