#include "pch/pch.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "intention/SectorExitPointShanwick.h"
#include "intention/IntentionCodeGenerator.h"

using UKControllerPlugin::IntentionCode::SectorExitPointShanwick;
using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using ::testing::StrictMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace IntentionCode {
        TEST(SectorExitPointShanwick, GetIntentionCodeReturnsCorrectDefaultCodeGomup)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointShanwick exitPoint("GOMUP", "N7", SectorExitPointShanwick::outWest);

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(1)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(1)
                .WillRepeatedly(Return("GOMUP"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 1, 37000).compare("N7"));
        }

        TEST(SectorExitPointShanwick, GetIntentionCodeReturnsCorrectDefaultCodeGomupIfRouteEmpty)
        {
            SectorExitPointShanwick exitPoint("GOMUP", "N7", SectorExitPointShanwick::outWest);
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            EXPECT_EQ(
                0,
                exitPoint.GetIntentionCode(
                    routeMock,
                    IntentionCodeGenerator::invalidExitPointIndex,
                    37000
                ).compare("N7")
            );
        }

        TEST(SectorExitPointShanwick, GetIntentionCodeReturnsCorrectDefaultCodeMimku)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointShanwick exitPoint("MIMKU", "N6", SectorExitPointShanwick::outWest);

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(1)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(1)
                .WillRepeatedly(Return("MIMKU"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 1, 37000).compare("N6"));
        }

        TEST(SectorExitPointShanwick, GetIntentionCodeReturnsCorrectDefaultCodeMimkuIfRouteInvalid)
        {
            SectorExitPointShanwick exitPoint("MIMKU", "N6", SectorExitPointShanwick::outWest);
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            EXPECT_EQ(
                0,
                exitPoint.GetIntentionCode(
                    routeMock,
                    IntentionCodeGenerator::invalidExitPointIndex,
                    37000
                ).compare("N6")
            );
        }

        TEST(SectorExitPointShanwick, GetIntentionCodeReturnsCorrectDefaultCodeNibog)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointShanwick exitPoint("NIBOG", "N5", SectorExitPointShanwick::outWest);

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(1)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(1)
                .WillRepeatedly(Return("NIBOG"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 1, 37000).compare("N5"));
        }

        TEST(SectorExitPointShanwick, GetIntentionCodeReturnsCorrectDefaultCodeNibogIfRouteInvalid)
        {
            SectorExitPointShanwick exitPoint("NIBOG", "N5", SectorExitPointShanwick::outWest);
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            EXPECT_EQ(
                0,
                exitPoint.GetIntentionCode(
                    routeMock,
                    IntentionCodeGenerator::invalidExitPointIndex,
                    3700
                ).compare("N5")
            );
        }

        TEST(SectorExitPointShanwick, GetIntentionCodeReturnsCorrectBelfastCodeGomup)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointShanwick exitPoint("GOMUP", "N7", SectorExitPointShanwick::outWest);

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(1)
                .WillRepeatedly(Return("BEL"));

            EXPECT_CALL(routeMock, GetPointName(2))
                .Times(1)
                .WillRepeatedly(Return("GOMUP"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 2, 37000).compare("U7"));
        }

        TEST(SectorExitPointShanwick, GetIntentionCodeReturnsCorrectBelfastCodeMimku)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointShanwick exitPoint("MIMKU", "N6", SectorExitPointShanwick::outWest);

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(1)
                .WillRepeatedly(Return("BEL"));

            EXPECT_CALL(routeMock, GetPointName(2))
                .Times(1)
                .WillRepeatedly(Return("MIMKU"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 2, 37000).compare("U6"));
        }

        TEST(SectorExitPointShanwick, GetIntentionCodeReturnsCorrectBelfastCodeNibog)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointShanwick exitPoint("NIBOG", "N5", SectorExitPointShanwick::outWest);

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(1)
                .WillRepeatedly(Return("BEL"));

            EXPECT_CALL(routeMock, GetPointName(2))
                .Times(1)
                .WillRepeatedly(Return("NIBOG"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 2, 37000).compare("U5"));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
