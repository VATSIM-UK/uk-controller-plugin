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

        TEST(SectorExitPointLelna, ItAppliesToAllControllers)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointLelna exitPoint("LELNA", "H2", SectorExitPointLelna::outSouth);

            EXPECT_TRUE(exitPoint.AppliesToController("BLA"));
        }

        TEST(SectorExitPointLelna, GetIntentionCodeReturnsCorrectDefaultCodeLelna)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointLelna exitPoint("LELNA", "H2", SectorExitPointLelna::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(1)
                .WillRepeatedly(Return(2));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(3)
                .WillRepeatedly(Return("LELNA"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(4)
                .WillRepeatedly(Return("LFMN"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H2"));
        }

        TEST(SectorExitPointLelna, GetIntentionCodeReturnsCorrectSecondaryCodeDinard)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointLelna exitPoint("LELNA", "H2", SectorExitPointLelna::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(1)
                .WillRepeatedly(Return(3));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(3)
                .WillRepeatedly(Return("LELNA"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(1)
                .WillRepeatedly(Return("DIN"));

            EXPECT_CALL(routeMock, GetPointName(2))
                .Times(1)
                .WillRepeatedly(Return("LEMD"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H2"));
        }

        TEST(SectorExitPointLelna, GetIntentionCodeReturnsCorrectSecondaryCodeArree)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointLelna exitPoint("LELNA", "H2", SectorExitPointLelna::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(1)
                .WillRepeatedly(Return(3));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(3)
                .WillRepeatedly(Return("LELNA"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(3)
                .WillRepeatedly(Return("ARE"));

            EXPECT_CALL(routeMock, GetPointName(2))
                .Times(1)
                .WillRepeatedly(Return("LEMD"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H3"));
        }

        TEST(SectorExitPointLelna, GetIntentionCodeReturnsCorrectSecondaryCodeDomok)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointLelna exitPoint("LELNA", "H2", SectorExitPointLelna::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                 .Times(1)
                .WillRepeatedly(Return(3));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(3)
                .WillRepeatedly(Return("LELNA"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(2)
                .WillRepeatedly(Return("DOMOK"));

            EXPECT_CALL(routeMock, GetPointName(2))
                .Times(1)
                .WillRepeatedly(Return("LEMD"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H3"));
        }

        TEST(SectorExitPointLelna, GetIntentionCodeReturnsCorrectIntentionCodeForBrestFirArrivals)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointLelna exitPoint("LELNA", "H2", SectorExitPointLelna::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(1)
                .WillRepeatedly(Return(3));

            EXPECT_CALL(routeMock, GetPointName(2))
                .Times(1)
                .WillRepeatedly(Return("LFRO"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("H5"));
        }
		
        TEST(SectorExitPointLelna, GetIntentionCodeReturnsCorrectIntentionCodeForCherbourgArrivals)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointLelna exitPoint("LELNA", "H5", SectorExitPointLelna::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(1)
                .WillRepeatedly(Return(3));

            EXPECT_CALL(routeMock, GetPointName(2))
                .Times(1)
                .WillRepeatedly(Return("LFRC"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("JC"));
        }

    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
