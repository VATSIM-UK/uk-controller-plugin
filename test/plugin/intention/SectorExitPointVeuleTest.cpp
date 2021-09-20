#include "intention/SectorExitPointVeule.h"
#include "intention/IntentionCodeGenerator.h"

using ::testing::Return;
using ::testing::StrictMock;
using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPlugin::IntentionCode::SectorExitPointVeule;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        TEST(SectorExitPointVeule, ItAppliesToAllControllers)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_TRUE(exitPoint.AppliesToController("BLA"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectDefaultCodeVeule)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(3).WillRepeatedly(Return(2));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(5).WillRepeatedly(Return("LFMN"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 31000).compare("V"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeTepri)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(1).WillRepeatedly(Return("TEPRI"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 31000).compare("V1"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeTepriLow)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(1).WillRepeatedly(Return("TEPRI"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 27000).compare("X1"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeTepriLowBoundary)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(1).WillRepeatedly(Return("TEPRI"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 29000).compare("X1"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodePekim)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(2).WillRepeatedly(Return("PEKIM"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 31000).compare("V2"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodePekimLow)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(2).WillRepeatedly(Return("PEKIM"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 27000).compare("X2"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodePekimLowBoundary)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(2).WillRepeatedly(Return("PEKIM"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 29000).compare("X2"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeOdebu)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(3).WillRepeatedly(Return("ODEBU"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 31000).compare("V3"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeOdebuLow)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(3).WillRepeatedly(Return("ODEBU"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 27000).compare("X3"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeOdebuLowBoundary)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(3).WillRepeatedly(Return("ODEBU"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 29000).compare("X3"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeKotap)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(4).WillRepeatedly(Return("KOTAP"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 31000).compare("V5"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeKotapLow)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(4).WillRepeatedly(Return("KOTAP"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 27000).compare("X5"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeKotapLowBoundary)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(4).WillRepeatedly(Return("KOTAP"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 29000).compare("X5"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeDekod)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(5).WillRepeatedly(Return("DEKOD"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 31000).compare("V6"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeDekodLow)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(5).WillRepeatedly(Return("DEKOD"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 27000).compare("X6"));
        }

        TEST(SectorExitPointVeule, GetIntentionCodeReturnsCorrectSecondaryCodeDekodLowBoundary)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> routeMock;
            SectorExitPointVeule exitPoint("VEULE", "V", SectorExitPointVeule::outSouth);

            EXPECT_CALL(routeMock, GetPointsNumber()).Times(2).WillRepeatedly(Return(5));

            EXPECT_CALL(routeMock, GetPointName(0)).Times(5).WillRepeatedly(Return("VEULE"));

            EXPECT_CALL(routeMock, GetPointName(1)).Times(5).WillRepeatedly(Return("DEKOD"));

            EXPECT_EQ(0, exitPoint.GetIntentionCode(routeMock, 0, 29000).compare("X6"));
        }
    } // namespace IntentionCode
} // namespace UKControllerPluginTest
