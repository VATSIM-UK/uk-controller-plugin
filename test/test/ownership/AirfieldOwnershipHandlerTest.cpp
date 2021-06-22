#include "pch/pch.h"
#include "ownership/AirfieldOwnershipHandler.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "mock/MockEuroScopeCControllerInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "initialaltitude/InitialAltitudeEventHandler.h"
#include "mock/MockSquawkEventHandler.h"
#include "flightplan/StoredFlightplan.h"
#include "message/UserMessager.h"
#include "timedevent/DeferredEventHandler.h"
#include "login/Login.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

using UKControllerPlugin::Sid::SidCollection;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;
using UKControllerPlugin::Ownership::AirfieldOwnershipHandler;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::InitialAltitude::InitialAltitudeEventHandler;
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
    namespace Ownership {

        class ControllerAirfieldOwnershipHandlerTest : public ::Test
        {
            public:
                ControllerAirfieldOwnershipHandlerTest()
                    : ownership(this->airfieldCollection, this->activeCallsigns),
                    login(plugin, ControllerStatusEventHandlerCollection()),
                    initialAltitudes(
                        new InitialAltitudeEventHandler(
                            this->sids,
                            this->activeCallsigns,
                            this->ownership,
                            this->login,
                            this->deferredEvents,
                            this->plugin,
                            this->flightplans
                        )
                    ),
                    userMessager(this->plugin),
                    handler(
                        this->ownership,
                        this->userMessager
                    )
                {

                }

                virtual void SetUp()
                {
                    // Add airfields to the collection
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<AirfieldModel>(
                            new AirfieldModel("EGKK", { "EGKK_DEL", "EGKK_GND", "EGKK_TWR", "EGKK_APP" })
                        )
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<AirfieldModel>(
                            new AirfieldModel("EGLL", { "EGLL_DEL", "EGLL_2_GND", "EGLL_S_TWR", "EGLL_N_APP" })
                        )
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<AirfieldModel>(new AirfieldModel("EGLC", { "LTC_SE_CTR", "LTC_S_CTR"}))
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<AirfieldModel>(new AirfieldModel("EGKB", { "LTC_SE_CTR", "LTC_S_CTR" }))
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<AirfieldModel>(new AirfieldModel("EGMC", { "LTC_SE_CTR", "LTC_S_CTR" }))
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<AirfieldModel>(new AirfieldModel("EGMD", { "LTC_SE_CTR", "LTC_S_CTR" }))
                    );
                    airfieldCollection.AddAirfield(
                        std::unique_ptr<AirfieldModel>(new AirfieldModel("EGKA", { "LTC_SW_CTR", "LTC_S_CTR" }))
                    );

                    // Add the controllers
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition(1, "EGKK_DEL", 199.998, {"EGKK"}, true, false)
                        )
                    );
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition(2, "EGKK_TWR", 199.999, {"EGKK"}, true, false)
                        )
                    );
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition(3, "EGKK_APP", 199.990, {"EGKK"}, true, false)
                        )
                    );
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition(4, "EGLL_S_TWR", 199.998, {"EGLL"}, true, false)
                        )
                    );
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition(5, "EGLL_N_APP", 199.998, {"EGLL"}, true, false)
                        )
                    );
                    this->controllerCollection.AddPosition(
                        std::unique_ptr<ControllerPosition>(
                            new ControllerPosition(
                                6,
                                "LTC_S_CTR",
                                134.120,
                                {"EGLL", "EGKK", "EGLC", "EGKA", "EGKB", "EGMC", "EGMD"},
                                true,
                                false
                            )
                        )
                    );

                    // Add the active callsigns
                    this->kkTwr = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition(2, "EGKK_TWR", 199.999, {"EGKK"}, true, false)
                    );
                    this->kkApp = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition(3, "EGKK_APP", 199.990, {"EGKK"}, true, false)
                    );
                    this->llTwr = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition(4, "EGLL_S_TWR", 199.998, {"EGLL"}, true, false)
                    );
                    this->llApp = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition(5, "EGLL_N_APP", 199.998, {"EGLL"}, true, false)
                    );

                    this->activeCallsigns.AddCallsign(ActiveCallsign("EGKK_TWR", "Testy McTest", *kkTwr));
                    this->activeCallsigns.AddCallsign(ActiveCallsign("EGKK_APP", "Testy McTest", *kkApp));
                    this->activeCallsigns.AddCallsign(ActiveCallsign("EGLL_S_TWR", "Testy McTest", *llTwr));
                    this->activeCallsigns.AddCallsign(ActiveCallsign("EGLL_N_APP", "Testy McTest", *llApp));
                    this->ownership.RefreshOwner("EGKK");

                    // Create a dummy initial altitude
                    sids.AddSid(std::make_shared<StandardInstrumentDeparture>("EGKK", "ADMAG2X", 6000, 0));
                    this->login.SetLoginTime(std::chrono::system_clock::now() - std::chrono::minutes(15));
                }

                AirfieldCollection airfieldCollection;
                ControllerPositionCollection controllerCollection;
                AirfieldOwnershipManager ownership;
                StoredFlightplanCollection flightplans;
                SidCollection sids;
                std::shared_ptr<InitialAltitudeEventHandler> initialAltitudes;
                NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
                Login login;
                DeferredEventHandler deferredEvents;
                ActiveCallsignCollection activeCallsigns;
                UserMessager userMessager;
                AirfieldOwnershipHandler handler;

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

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ActiveCallsignFlushFlushesOwners)
        {
            this->ownership.RefreshOwner("EGKK");
            this->ownership.RefreshOwner("EGLL");

            this->handler.CallsignsFlushed();
            EXPECT_FALSE(this->ownership.AirfieldHasOwner("EGKK"));
            EXPECT_FALSE(this->ownership.AirfieldHasOwner("EGLL"));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ActiveCallsignDisconnectRefreshesOwnership)
        {
            ActiveCallsign gatwick = this->activeCallsigns.GetCallsign("EGKK_TWR");
            this->activeCallsigns.RemoveCallsign(this->activeCallsigns.GetCallsign("EGKK_TWR"));
            this->handler.ActiveCallsignRemoved(gatwick, false);
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGKK", this->activeCallsigns.GetCallsign("EGKK_APP")));
        }

        TEST_F(
            ControllerAirfieldOwnershipHandlerTest,
            ActiveCallsignDisconnectResetsOwnerToOtherCallsignIfPositionStillActive
        ) {
            // Add the spare controller
            ControllerPosition pos(1, "EGKK_TWR", 199.998, {"EGKK"}, true, false);
            this->activeCallsigns.AddCallsign(ActiveCallsign("EGKK_1_TWR", "Another Guy", pos));

            ActiveCallsign gatwick = this->activeCallsigns.GetCallsign("EGKK_TWR");
            this->activeCallsigns.RemoveCallsign(this->activeCallsigns.GetCallsign("EGKK_TWR"));
            this->handler.ActiveCallsignRemoved(gatwick, false);
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGKK", this->activeCallsigns.GetCallsign("EGKK_1_TWR")));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, NewActiveCallsignEventRefreshesTopDown)
        {
            this->activeCallsigns.AddCallsign(
                ActiveCallsign("EGKK_DEL", "Test", this->controllerCollection.FetchPositionByCallsign("EGKK_DEL"))
            );
            this->handler.ActiveCallsignAdded(this->activeCallsigns.GetCallsign("EGKK_DEL"), true);
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGKK", this->activeCallsigns.GetCallsign("EGKK_DEL")));
        }

        TEST_F(ControllerAirfieldOwnershipHandlerTest, ControllerUpdateEventUpdatesTopDownMultiple)
        {
            this->activeCallsigns.AddCallsign(
                ActiveCallsign("LTC_S_CTR", "Test", this->controllerCollection.FetchPositionByCallsign("LTC_S_CTR"))
            );
            this->handler.ActiveCallsignAdded(this->activeCallsigns.GetCallsign("LTC_S_CTR"), true);
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGKK", this->activeCallsigns.GetCallsign("EGKK_TWR")));
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGLL", this->activeCallsigns.GetCallsign("EGLL_S_TWR")));
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGLC", this->activeCallsigns.GetCallsign("LTC_S_CTR")));
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGKB", this->activeCallsigns.GetCallsign("LTC_S_CTR")));
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGMC", this->activeCallsigns.GetCallsign("LTC_S_CTR")));
            EXPECT_TRUE(this->ownership.AirfieldOwnedBy("EGMD", this->activeCallsigns.GetCallsign("LTC_S_CTR")));
        }
    }  // namespace Ownership
}  // namespace UKControllerPluginTest
