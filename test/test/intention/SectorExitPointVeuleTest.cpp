#include "pch/pch.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "intention/SectorExitPointVeule.h"
#include "intention/IntentionCodeGenerator.h"

using UKControllerPlugin::IntentionCode::SectorExitPointVeule;
using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using ::testing::StrictMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace IntentionCode {
        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectDefaultCodeVeule)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(3)
                .WillRepeatedly(Return(2));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(5)
                .WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(5)
                .WillRepeatedly(Return("LFMN"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("V"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeTepri)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(5)
                .WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(1)
                .WillRepeatedly(Return("TEPRI"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("V1"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodePekim)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(5)
                .WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(2)
                .WillRepeatedly(Return("PEKIM"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("V2"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeOdebuLow)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(5)
                .WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(3)
                .WillRepeatedly(Return("ODEBU"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 25000).compare("V4"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeOdebuLowBoundary)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(5)
                .WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(3)
                .WillRepeatedly(Return("ODEBU"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 27000).compare("V4"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeOdebuHighBoundary)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(5)
                .WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(3)
                .WillRepeatedly(Return("ODEBU"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 27001).compare("V3"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeOdebuHigh)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(5)
                .WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(3)
                .WillRepeatedly(Return("ODEBU"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("V3"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeKotap)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(5)
                .WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(4)
                .WillRepeatedly(Return("KOTAP"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("V5"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeDekod)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber())
                .Times(2)
                .WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0))
                .Times(5)
                .WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1))
                .Times(5)
                .WillRepeatedly(Return("DEKOD"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 37000).compare("V6"));
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
