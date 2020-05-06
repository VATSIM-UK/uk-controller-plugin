#include "pch/pch.h"
#include "controller/ActiveCallsignMonitor.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "mock/MockEuroScopeCControllerInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "flightplan/StoredFlightplan.h"
#include "timedevent/DeferredEventHandler.h"
#include "login/Login.h"
#include "controller/ControllerStatusEventHandlerCollection.h"

using UKControllerPlugin::Controller::ActiveCallsignMonitor;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::TimedEvent::DeferredEventHandler;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;

using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Controller {

        class ActiveCallsignMonitorTest : public ::Test
        {
            public:
                ActiveCallsignMonitorTest()
                    : login(plugin, ControllerStatusEventHandlerCollection()),
                    handler(
                        this->controllerCollection,
                        this->activeCallsigns
                    )
                {

                }

                virtual void SetUp()
                {
                    // Add the controllers
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition("EGKK_DEL", 199.998, "DEL", { "EGKK" })
                        )
                    );
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition("EGKK_TWR", 199.999, "TWR", { "EGKK" })
                        )
                    );
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition("EGKK_APP", 199.990, "APP", { "EGKK" })
                        )
                    );
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition("EGLL_S_TWR", 199.998, "TWR", { "EGLL" })
                        )
                    );
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition("EGLL_N_APP", 199.998, "APP", { "EGLL" })
                        )
                    );
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition(
                                "LTC_S_CTR",
                                134.120,
                                "CTR",
                                { "EGLL", "EGKK", "EGLC", "EGKA", "EGKB", "EGMC", "EGMD" }
                            )
                        )
                    );

                    // Add the active callsigns
                    this->kkTwr = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("EGKK_TWR", 199.999, "TWR", { "EGKK" })
                    );
                    this->kkApp = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("EGKK_APP", 199.990, "APP", { "EGKK" })
                    );
                    this->llTwr = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("EGLL_S_TWR", 199.998, "TWR", { "EGLL" })
                    );
                    this->llApp = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("EGLL_N_APP", 199.998, "APP", { "EGLL" })
                    );

                    this->activeCallsigns.AddCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *kkTwr));
                    this->activeCallsigns.AddCallsign(ActiveCallsign("EGKK_APP", "Testy McTest", *kkApp));
                    this->activeCallsigns.AddCallsign(ActiveCallsign("EGLL_S_TWR", "Testy McTest", *llTwr));
                    this->activeCallsigns.AddCallsign(ActiveCallsign("EGLL_N_APP", "Testy McTest", *llApp));

                    this->login.SetLoginTime(std::chrono::system_clock::now() - std::chrono::minutes(15));
                }

                ControllerPositionCollection controllerCollection;
                StoredFlightplanCollection flightplans;
                NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
                Login login;
                DeferredEventHandler deferredEvents;
                ActiveCallsignCollection activeCallsigns;
                ActiveCallsignMonitor handler;

                // Controllers
                std::unique_ptr<ControllerPosition> kkTwr;
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

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .WillRepeatedly(Return("EGKK_TWR"));

            this->handler.ControllerDisconnectEvent(euroscopeMock);

            EXPECT_FALSE(this->activeCallsigns.CallsignActive("EGKK_TWR"));
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerDisconnectIngoresNonRecognisedControllers)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .WillOnce(Return("NOTAREALCALLSIGN"));

            this->handler.ControllerDisconnectEvent(euroscopeMock);
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerUpdateEventRemovesCallsignIfFrequencyUnset)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .WillRepeatedly(Return("EGKK_TWR"));

            EXPECT_CALL(euroscopeMock, HasActiveFrequency())
                .Times(1)
                .WillOnce(Return(false));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_FALSE(this->activeCallsigns.CallsignActive("EGKK_TWR"));
            EXPECT_FALSE(this->activeCallsigns.PositionActive("EGKK_TWR"));
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerUpdateEventAddsActiveCallsign)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .WillRepeatedly(Return("EGKK_DEL"));

            EXPECT_CALL(euroscopeMock, HasActiveFrequency())
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(euroscopeMock, GetControllerName())
                .Times(1)
                .WillOnce(Return("Testy McTestington"));

            EXPECT_CALL(euroscopeMock, GetFrequency())
                .Times(1)
                .WillOnce(Return(199.998));

            EXPECT_CALL(euroscopeMock, IsCurrentUser())
                .Times(1)
                .WillRepeatedly(Return(true));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_TRUE(this->activeCallsigns.CallsignActive("EGKK_DEL"));
            EXPECT_TRUE(this->activeCallsigns.PositionActive("EGKK_DEL"));
        }

        TEST_F(ActiveCallsignMonitorTest, ControllerUpdateEventHandlesHyphenatedCallsigns)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .WillRepeatedly(Return("EGKK_1-DEL"));

            EXPECT_CALL(euroscopeMock, HasActiveFrequency())
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(euroscopeMock, GetControllerName())
                .Times(1)
                .WillOnce(Return("Testy McTestington"));

            EXPECT_CALL(euroscopeMock, GetFrequency())
                .Times(1)
                .WillOnce(Return(199.998));

            EXPECT_CALL(euroscopeMock, IsCurrentUser())
                .Times(1)
                .WillRepeatedly(Return(true));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_TRUE(this->activeCallsigns.CallsignActive("EGKK_1-DEL"));
            EXPECT_TRUE(this->activeCallsigns.PositionActive("EGKK_DEL"));
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
