#include "pch/pch.h"
#include "airfield/ControllerAirfieldOwnershipHandler.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/Airfield.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "airfield/AirfieldOwnershipManager.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "mock/MockEuroScopeCControllerInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "massevent/MassEvent.h"
#include "initialaltitude/InitialAltitudeEventHandler.h"
#include "initialaltitude/InitialAltitudeGenerator.h"
#include "mock/MockSquawkEventHandler.h"
#include "flightplan/StoredFlightplan.h"
#include "message/UserMessager.h"
#include "timedevent/DeferredEventHandler.h"
#include "login/Login.h"
#include "controller/ControllerStatusEventHandlerCollection.h"

using UKControllerPlugin::Airfield::ControllerAirfieldOwnershipHandler;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::Airfield;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Airfield::AirfieldOwnershipManager;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::EventHandler::MassEvent;
using UKControllerPlugin::InitialAltitude::InitialAltitudeEventHandler;
using UKControllerPlugin::InitialAltitude::InitialAltitudeGenerator;
using UKControllerPluginTest::Squawk::MockSquawkEventHandler;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::TimedEvent::DeferredEventHandler;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;

using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Controller {

        class ControllerAirfieldOwnershipHandlerTest : public ::Test
        {
            public:
                ControllerAirfieldOwnershipHandlerTest()
                    : ownership(this->airfieldCollection, this->activeCallsigns),
                    login(plugin, ControllerStatusEventHandlerCollection()),
                    initialAltitudes(
                        new InitialAltitudeEventHandler(
                            this->generator,
                            this->activeCallsigns,
                            this->ownership,
                            this->login,
                            this->deferredEvents,
                            this->plugin
                        )
                    ),
                    userMessager(this->plugin),
                    massEvents(this->plugin, this->initialAltitudes, this->flightplans),
                    handler(
                        this->controllerCollection,
                        this->ownership,
                        this->activeCallsigns,
                        this->massEvents,
                        this->userMessager
                    )
                {

                }

                virtual void SetUp()
                {
                    // Add airfields to the collection
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<Airfield>(
                            new Airfield("EGKK", { "EGKK_DEL", "EGKK_GND", "EGKK_TWR", "EGKK_APP" })
                        )
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<Airfield>(
                            new Airfield("EGLL", { "EGLL_DEL", "EGLL_2_GND", "EGLL_S_TWR", "EGLL_N_APP" })
                        )
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<Airfield>(new Airfield("EGLC", { "LTC_SE_CTR", "LTC_S_CTR"}))
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<Airfield>(new Airfield("EGKB", { "LTC_SE_CTR", "LTC_S_CTR" }))
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<Airfield>(new Airfield("EGMC", { "LTC_SE_CTR", "LTC_S_CTR" }))
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<Airfield>(new Airfield("EGMD", { "LTC_SE_CTR", "LTC_S_CTR" }))
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<Airfield>(new Airfield("EGKA", { "LTC_SW_CTR", "LTC_S_CTR" }))
                    );

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
                    this->ownership.RefreshOwner("EGKK");

                    // Create a dummy initial altitude
                    this->generator.AddSid("EGKK", "ADMAG2X", 6000);
                    this->login.SetLoginTime(std::chrono::system_clock::now() - std::chrono::minutes(15));
                }

                AirfieldCollection airfieldCollection;
                ControllerPositionCollection controllerCollection;
                AirfieldOwnershipManager ownership;
                StoredFlightplanCollection flightplans;
                InitialAltitudeGenerator generator;
                std::shared_ptr<InitialAltitudeEventHandler> initialAltitudes;
                NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
                Login login;
                DeferredEventHandler deferredEvents;
                ActiveCallsignCollection activeCallsigns;
                UserMessager userMessager;
                MassEvent massEvents;
                ControllerAirfieldOwnershipHandler handler;

                // Controllers
                std::unique_ptr<ControllerPosition> kkTwr;
                std::unique_ptr<ControllerPosition> kkApp;
                std::unique_ptr<ControllerPosition> llTwr;
                std::unique_ptr<ControllerPosition> llApp;
        };

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ProcessCommandOwnedReturnsTrueOnCallsignMatch)
        {
            ON_CALL(this->plugin, ChatAreaMessage(_, _, _, _, _, _, _, _))
                .WillByDefault(Return());
            EXPECT_TRUE(this->handler.ProcessCommand(".ukcp owned EGKK_APP"));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ProcessCommandOwnedSendsAMessage)
        {
            ON_CALL(
                this->plugin,
                    ChatAreaMessage(
                    "UKCP_Query",
                    "UKCP",
                    "EGKK_TWR owns the following airfields: EGKK",
                    true,
                    true,
                    true,
                    true,
                    false
                ))
                .WillByDefault(Return());
            EXPECT_TRUE(this->handler.ProcessCommand(".ukcp owned EGKK_TWR"));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ProcessCommandOwnerReturnsTrueOnAirfieldMatch)
        {
            ON_CALL(this->plugin, ChatAreaMessage(_, _, _, _, _, _, _, _))
                .WillByDefault(Return());
            EXPECT_TRUE(this->handler.ProcessCommand(".ukcp owner EGKK"));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ProcessCommandOwnerSendsAMessageAirfieldMatch)
        {
            ON_CALL(
                this->plugin,
                ChatAreaMessage(
                    "UKCP_Query",
                    "UKCP",
                    "The airfield EGKK is owned by EGKK_TWR (Testy McTest)",
                    true,
                    true,
                    true,
                    true,
                    false
                ))
                .WillByDefault(Return());
            EXPECT_TRUE(this->handler.ProcessCommand(".ukcp owner EGKK"));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ProcessCommandOwnerReturnsFalseOnBadAerodromeFormat)
        {
            EXPECT_FALSE(this->handler.ProcessCommand(".ukcp owner EGK1"));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ProcessCommandReturnsFalseOnRandomCommand)
        {
            EXPECT_FALSE(this->handler.ProcessCommand("ilikepie"));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, SelfDisconnectEventFlushesAllCaches)
        {
            this->ownership.RefreshOwner("EGKK");
            this->ownership.RefreshOwner("EGLL");
            this->handler.SelfDisconnectEvent();

            EXPECT_FALSE(this->ownership.AirfieldHasOwner("EGKK"));
            EXPECT_FALSE(this->ownership.AirfieldHasOwner("EGLL"));
            EXPECT_EQ(0, this->activeCallsigns.GetNumberActiveCallsigns());
            EXPECT_EQ(0, this->activeCallsigns.GetNumberActivePositions());
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ControllerDisconnectRemovesActiveCallsign)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .Times(3)
                .WillRepeatedly(Return("EGKK_TWR"));

            this->handler.ControllerDisconnectEvent(euroscopeMock);

            EXPECT_FALSE(this->activeCallsigns.CallsignActive("EGKK_TWR"));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ControllerDisconnectResetsOwnerIfPositionNoLongerActive)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .Times(3)
                .WillRepeatedly(Return("EGKK_TWR"));

            this->handler.ControllerDisconnectEvent(euroscopeMock);
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGKK", this->activeCallsigns.GetCallsign("EGKK_APP")));
        }

        TEST_F(
            ControllerAirfieldOwnershipHandlerTest,
            ControllerDisconnectResetsOwnerToOtherCallsignIfPositionStillActive
        ) {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .Times(3)
                .WillRepeatedly(Return("EGKK_TWR"));

            // Add the spare controller
            ControllerPosition pos("EGKK_TWR", 199.998, "TWR", { "EGKK" });
            this->activeCallsigns.AddCallsign(ActiveCallsign("EGKK_1_TWR", "Another Guy", pos));

            this->handler.ControllerDisconnectEvent(euroscopeMock);
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGKK", this->activeCallsigns.GetCallsign("EGKK_1_TWR")));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ControllerDisconnectIngoresNonRecognisedControllers)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .Times(1)
                .WillOnce(Return("NOTAREALCALLSIGN"));

            this->handler.ControllerDisconnectEvent(euroscopeMock);
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ControllerUpdateEventRemovesCallsignIfFrequencyUnset)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .Times(4)
                .WillRepeatedly(Return("EGKK_TWR"));

            EXPECT_CALL(euroscopeMock, HasActiveFrequency())
                .Times(1)
                .WillOnce(Return(false));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_FALSE(this->activeCallsigns.CallsignActive("EGKK_TWR"));
            EXPECT_FALSE(this->activeCallsigns.PositionActive("EGKK_TWR"));
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGKK", this->activeCallsigns.GetCallsign("EGKK_APP")));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ControllerUpdateEventAddsActiveCallsign)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .Times(4)
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
                .Times(2)
                .WillRepeatedly(Return(true));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_TRUE(this->activeCallsigns.CallsignActive("EGKK_DEL"));
            EXPECT_TRUE(this->activeCallsigns.PositionActive("EGKK_DEL"));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ControllerUpdateEventUpdatesTopDown)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .Times(4)
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
                .Times(2)
                .WillRepeatedly(Return(true));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGKK", this->activeCallsigns.GetCallsign("EGKK_DEL")));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ControllerUpdateEventUpdatesTopDownMultiple)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .Times(4)
                .WillRepeatedly(Return("LTC_S_CTR"));

            EXPECT_CALL(euroscopeMock, HasActiveFrequency())
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(euroscopeMock, GetControllerName())
                .Times(1)
                .WillOnce(Return("Testy McTestington"));

            EXPECT_CALL(euroscopeMock, GetFrequency())
                .Times(1)
                .WillOnce(Return(134.120));

            EXPECT_CALL(euroscopeMock, IsCurrentUser())
                .Times(2)
                .WillRepeatedly(Return(true));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGKK", this->activeCallsigns.GetCallsign("EGKK_TWR")));
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGLL", this->activeCallsigns.GetCallsign("EGLL_S_TWR")));
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGLC", this->activeCallsigns.GetCallsign("LTC_S_CTR")));
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGKB", this->activeCallsigns.GetCallsign("LTC_S_CTR")));
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGMC", this->activeCallsigns.GetCallsign("LTC_S_CTR")));
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGMD", this->activeCallsigns.GetCallsign("LTC_S_CTR")));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ControllerUpdateEventHandlesHyphenatedCallsigns)
        {
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            EXPECT_CALL(euroscopeMock, GetCallsign())
                .Times(4)
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
                .Times(2)
                .WillRepeatedly(Return(true));

            this->handler.ControllerUpdateEvent(euroscopeMock);
            EXPECT_TRUE(this->activeCallsigns.CallsignActive("EGKK_1-DEL"));
            EXPECT_TRUE(this->activeCallsigns.PositionActive("EGKK_DEL"));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ControllerUpdateEventUpdatesFlightplansIfControllerIsUser)
        {

            // Do some dummy flightplan setups.
            NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
            NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
            ON_CALL(mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(mockFlightplan, GetDestination())
                .WillByDefault(Return("EDDM"));

            this->flightplans.UpdatePlan(StoredFlightplan(mockFlightplan));


            // Create the controller mock
            NiceMock<MockEuroScopeCControllerInterface> euroscopeMock;

            ON_CALL(euroscopeMock, GetCallsign())
                .WillByDefault(Return("EGKK_DEL"));

            ON_CALL(euroscopeMock, HasActiveFrequency())
                .WillByDefault(Return(true));

            ON_CALL(euroscopeMock, GetControllerName())
                .WillByDefault(Return("Testy McTestington"));

            ON_CALL(euroscopeMock, GetFrequency())
                .WillByDefault(Return(199.998));

            ON_CALL(euroscopeMock, IsCurrentUser())
                .WillByDefault(Return(true));


            // Create mock radar targets and flightplans, that simulate what ES returns when asked for a flightplan.
            std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockFlightplanReturn(
                new NiceMock<MockEuroScopeCFlightPlanInterface>
            );

            ON_CALL(*mockFlightplanReturn, GetDistanceFromOrigin())
                .WillByDefault(Return(1));

            ON_CALL(*mockFlightplanReturn, HasControllerClearedAltitude())
                .WillByDefault(Return(false));

            ON_CALL(*mockFlightplanReturn, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(*mockFlightplanReturn, IsSimulated())
                .WillByDefault(Return(false));

            ON_CALL(*mockFlightplanReturn, GetSidName())
                .WillByDefault(Return("ADMAG2X"));

            ON_CALL(*mockFlightplanReturn, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*mockFlightplanReturn, GetCruiseLevel())
                .WillByDefault(Return(6000));

            ON_CALL(*mockFlightplanReturn, GetCallsign())
                .WillByDefault(Return("BAW123"));

            std::shared_ptr<MockEuroScopeCRadarTargetInterface> mockRadarTargetReturn(
                new NiceMock<MockEuroScopeCRadarTargetInterface>
            );

            ON_CALL(*mockRadarTargetReturn, GetGroundSpeed())
                .WillByDefault(Return(5));

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(mockFlightplanReturn));

            ON_CALL(this->plugin, GetRadarTargetForCallsign("BAW123"))
                .WillByDefault(Return(mockRadarTargetReturn));

            // The important expectation - we're expecting the cleared altitude to be set and squawks assigned.
            EXPECT_CALL(*mockFlightplanReturn, SetClearedAltitude(6000))
                .Times(1);

            this->handler.ControllerUpdateEvent(euroscopeMock);
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
