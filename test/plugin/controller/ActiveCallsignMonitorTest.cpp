#include "controller/ActiveCallsignMonitor.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "login/Login.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ActiveCallsignMonitor;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Controller {

        class ActiveCallsignMonitorTest : public ::Test
        {
            public:
            ActiveCallsignMonitorTest()
                : login(plugin, ControllerStatusEventHandlerCollection()),
                  handler(this->controllerCollection, this->activeCallsigns)
            {
            }

            virtual void SetUp()
            {
                // Add the controllers
                this->controllerCollection.AddPosition(std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(1, "EGKK_DEL", 199.998, {"EGKK"}, true, false)));
                this->controllerCollection.AddPosition(std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(2, "EGKK_TWR", 199.999, {"EGKK"}, true, false)));
                this->controllerCollection.AddPosition(std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(3, "EGKK_2_TWR", 199.997, {"EGKK"}, true, false)));
                this->controllerCollection.AddPosition(std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(4, "EGKK_APP", 199.990, {"EGKK"}, true, false)));
                this->controllerCollection.AddPosition(std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(5, "EGLL_S_TWR", 199.998, {"EGLL"}, true, false)));
                this->controllerCollection.AddPosition(std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(6, "EGLL_N_APP", 199.998, {"EGLL"}, true, false)));
                this->controllerCollection.AddPosition(std::unique_ptr<ControllerPosition>(new ControllerPosition(
                    5, "LTC_S_CTR", 134.120, {"EGLL", "EGKK", "EGLC", "EGKA", "EGKB", "EGMC", "EGMD"}, true, false)));

                // Add the active callsigns
                this->kkTwr = std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(2, "EGKK_TWR", 199.999, {"EGKK"}, true, false));
                this->kkTwr2 = std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(3, "EGKK_2_TWR", 199.997, {"EGKK"}, true, false));
                this->kkApp = std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(4, "EGKK_APP", 199.990, {"EGKK"}, true, false));
                this->llTwr = std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(5, "EGLL_S_TWR", 199.998, {"EGLL"}, true, false));
                this->llApp = std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(6, "EGLL_N_APP", 199.998, {"EGLL"}, true, false));

                this->activeCallsigns.AddCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *kkTwr, false));
                this->activeCallsigns.AddCallsign(ActiveCallsign("EGKK_APP", "Testy McTest", *kkApp, false));
                this->activeCallsigns.AddCallsign(ActiveCallsign("EGLL_S_TWR", "Testy McTest", *llTwr, false));
                this->activeCallsigns.AddCallsign(ActiveCallsign("EGLL_N_APP", "Testy McTest", *llApp, false));

                this->login.SetLoginTime(std::chrono::system_clock::now() - std::chrono::minutes(15));
            }

            ControllerPositionCollection controllerCollection;
            StoredFlightplanCollection flightplans;
            NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
            Login login;
            ActiveCallsignCollection activeCallsigns;
            ActiveCallsignMonitor handler;

            // Controllers
            std::unique_ptr<ControllerPosition> kkTwr;
            std::unique_ptr<ControllerPosition> kkTwr2;
            std::unique_ptr<ControllerPosition> kkApp;
            std::unique_ptr<ControllerPosition> llTwr;
            std::unique_ptr<ControllerPosition> llApp;
        };

        TEST_F(ActiveCallsignMonitorTest, SelfDisconnectEventFlushesAllCaches)
        {
            this->handler.SelfDisconnectEvent();
            EXPECT_EQ(0, this->activeCallsigns.GetNumberActiveCallsigns());
            EXPECT_EQ(0, this->activeCallsigns.GetNumberActivePositions());
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerDisconnectRemovesActiveCallsign)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign()).WillRepeatedly(Return("EGKK_TWR"));

            this->handler.ControllerDisconnectEvent(euroscopeMock);

            EXPECT_FALSE(this->activeCallsigns.CallsignActive("EGKK_TWR"));
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerDisconnectIngoresNonRecognisedControllers)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign()).WillOnce(Return("NOTAREALCALLSIGN"));

            this->handler.ControllerDisconnectEvent(euroscopeMock);
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerUpdateEventRemovesCallsignIfFrequencyUnset)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign()).WillRepeatedly(Return("EGKK_TWR"));

            EXPECT_CALL(euroscopeMock, HasActiveFrequency()).Times(1).WillOnce(Return(false));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_FALSE(this->activeCallsigns.CallsignActive("EGKK_TWR"));
            EXPECT_FALSE(this->activeCallsigns.PositionActive("EGKK_TWR"));
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerUpdateEventHandlesFrequencyChanges)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign()).WillRepeatedly(Return("EGKK_TWR"));

            EXPECT_CALL(euroscopeMock, GetFrequency()).WillRepeatedly(Return(199.997));

            EXPECT_CALL(euroscopeMock, HasActiveFrequency()).Times(1).WillOnce(Return(true));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_TRUE(this->activeCallsigns.CallsignActive("EGKK_TWR"));
            EXPECT_EQ(*this->kkTwr2, this->activeCallsigns.GetCallsign("EGKK_TWR").GetNormalisedPosition());
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerUpdateEventDoesNothingOnFrequencyChanges)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign()).WillRepeatedly(Return("EGKK_TWR"));

            EXPECT_CALL(euroscopeMock, GetFrequency()).WillRepeatedly(Return(199.999));

            EXPECT_CALL(euroscopeMock, HasActiveFrequency()).Times(1).WillOnce(Return(true));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_TRUE(this->activeCallsigns.CallsignActive("EGKK_TWR"));
            EXPECT_EQ(*this->kkTwr, this->activeCallsigns.GetCallsign("EGKK_TWR").GetNormalisedPosition());
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerUpdateEventAddsUserActiveCallsign)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign()).WillRepeatedly(Return("EGKK_DEL"));

            EXPECT_CALL(euroscopeMock, HasActiveFrequency()).Times(1).WillOnce(Return(true));

            EXPECT_CALL(euroscopeMock, GetControllerName()).Times(1).WillOnce(Return("Testy McTestington"));

            EXPECT_CALL(euroscopeMock, GetFrequency()).Times(1).WillOnce(Return(199.998));

            EXPECT_CALL(euroscopeMock, IsCurrentUser()).Times(1).WillRepeatedly(Return(true));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_TRUE(this->activeCallsigns.CallsignActive("EGKK_DEL"));
            EXPECT_TRUE(this->activeCallsigns.PositionActive("EGKK_DEL"));
            EXPECT_TRUE(this->activeCallsigns.UserHasCallsign());
            auto callsign = this->activeCallsigns.GetCallsign("EGKK_DEL");
            EXPECT_EQ("EGKK_DEL", callsign.GetCallsign());
            EXPECT_EQ("Testy McTestington", callsign.GetControllerName());
            EXPECT_EQ(
                *this->controllerCollection.FetchPositionByCallsign("EGKK_DEL"), callsign.GetNormalisedPosition());
            EXPECT_TRUE(callsign.GetIsUser());
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerUpdateEventAddsNonUserActiveCallsign)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign()).WillRepeatedly(Return("EGKK_DEL"));

            EXPECT_CALL(euroscopeMock, HasActiveFrequency()).Times(1).WillOnce(Return(true));

            EXPECT_CALL(euroscopeMock, GetControllerName()).Times(1).WillOnce(Return("Testy McTestington"));

            EXPECT_CALL(euroscopeMock, GetFrequency()).Times(1).WillOnce(Return(199.998));

            EXPECT_CALL(euroscopeMock, IsCurrentUser()).Times(1).WillRepeatedly(Return(false));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_TRUE(this->activeCallsigns.CallsignActive("EGKK_DEL"));
            EXPECT_TRUE(this->activeCallsigns.PositionActive("EGKK_DEL"));
            EXPECT_FALSE(this->activeCallsigns.UserHasCallsign());
            auto callsign = this->activeCallsigns.GetCallsign("EGKK_DEL");
            EXPECT_EQ("EGKK_DEL", callsign.GetCallsign());
            EXPECT_EQ("Testy McTestington", callsign.GetControllerName());
            EXPECT_EQ(
                *this->controllerCollection.FetchPositionByCallsign("EGKK_DEL"), callsign.GetNormalisedPosition());
            EXPECT_FALSE(callsign.GetIsUser());
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerUpdateEventHandlesHyphenatedCallsigns)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign()).WillRepeatedly(Return("EGKK_1-DEL"));

            EXPECT_CALL(euroscopeMock, HasActiveFrequency()).Times(1).WillOnce(Return(true));

            EXPECT_CALL(euroscopeMock, GetControllerName()).Times(1).WillOnce(Return("Testy McTestington"));

            EXPECT_CALL(euroscopeMock, GetFrequency()).Times(1).WillOnce(Return(199.998));

            EXPECT_CALL(euroscopeMock, IsCurrentUser()).Times(1).WillRepeatedly(Return(true));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_TRUE(this->activeCallsigns.CallsignActive("EGKK_1-DEL"));
            EXPECT_TRUE(this->activeCallsigns.PositionActive("EGKK_DEL"));
        }
    } // namespace Controller
} // namespace UKControllerPluginTest
