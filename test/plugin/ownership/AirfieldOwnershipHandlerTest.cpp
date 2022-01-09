#include "ownership/AirfieldOwnershipHandler.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionHierarchy.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "message/UserMessager.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "login/Login.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Ownership::AirfieldOwnershipHandler;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest::Ownership {

    class ControllerAirfieldOwnershipHandlerTest : public ::Test
    {
        public:
        ControllerAirfieldOwnershipHandlerTest()
            : serviceProviders(std::make_shared<AirfieldServiceProviderCollection>()),
              ownership(serviceProviders, this->airfieldCollection, this->activeCallsigns),
              login(plugin, ControllerStatusEventHandlerCollection()), userMessager(this->plugin),
              handler(this->ownership, this->userMessager)
        {
        }

        void SetUp() override
        {
            // Add the controllers
            this->controllerCollection.AddPosition(std::make_shared<ControllerPosition>(
                1, "EGKK_DEL", 199.998, std::vector<std::string>{"EGKK"}, true, false));
            this->controllerCollection.AddPosition(std::make_shared<ControllerPosition>(
                2, "EGKK_TWR", 199.999, std::vector<std::string>{"EGKK"}, true, false));
            this->controllerCollection.AddPosition(std::make_shared<ControllerPosition>(
                3, "EGKK_APP", 199.990, std::vector<std::string>{"EGKK"}, true, false));
            this->controllerCollection.AddPosition(std::make_shared<ControllerPosition>(
                4, "EGLL_S_TWR", 199.998, std::vector<std::string>{"EGLL"}, true, false));
            this->controllerCollection.AddPosition(std::make_shared<ControllerPosition>(
                5, "EGLL_N_APP", 199.998, std::vector<std::string>{"EGLL"}, true, false));
            this->controllerCollection.AddPosition(std::make_shared<ControllerPosition>(
                6,
                "LTC_S_CTR",
                134.120,
                std::vector<std::string>{"EGLL", "EGKK", "EGLC", "EGKA", "EGKB", "EGMC", "EGMD"},
                true,
                false));

            // Add the active callsigns
            this->activeCallsigns.AddCallsign(
                ActiveCallsign("EGKK_TWR", "Testy McTest", *this->controllerCollection.FetchPositionById(2), false));
            this->activeCallsigns.AddCallsign(
                ActiveCallsign("EGKK_APP", "Testy McTest", *this->controllerCollection.FetchPositionById(3), false));
            this->activeCallsigns.AddCallsign(
                ActiveCallsign("EGLL_S_TWR", "Testy McTest", *this->controllerCollection.FetchPositionById(4), false));
            this->activeCallsigns.AddCallsign(
                ActiveCallsign("EGLL_N_APP", "Testy McTest", *this->controllerCollection.FetchPositionById(5), false));
            this->ownership.RefreshOwner("EGKK");

            // Create a dummy initial altitude
            this->login.SetLoginTime(std::chrono::system_clock::now() - std::chrono::minutes(15));

            // Add airfields to the collection
            auto gatwickHierarchy = std::make_unique<ControllerPositionHierarchy>();
            gatwickHierarchy->AddPosition(this->controllerCollection.FetchPositionById(1));
            gatwickHierarchy->AddPosition(this->controllerCollection.FetchPositionById(2));
            gatwickHierarchy->AddPosition(this->controllerCollection.FetchPositionById(3));
            airfieldCollection.AddAirfield(std::make_shared<AirfieldModel>(1, "EGKK", std::move(gatwickHierarchy)));

            auto heathrowHierarchy = std::make_unique<ControllerPositionHierarchy>();
            heathrowHierarchy->AddPosition(this->controllerCollection.FetchPositionById(4));
            heathrowHierarchy->AddPosition(this->controllerCollection.FetchPositionById(5));
            airfieldCollection.AddAirfield(std::make_shared<AirfieldModel>(2, "EGLL", std::move(heathrowHierarchy)));

            airfieldCollection.AddAirfield(std::make_shared<AirfieldModel>(3, "EGLC", DefaultHierarchy()));
            airfieldCollection.AddAirfield(std::make_shared<AirfieldModel>(4, "EGKB", DefaultHierarchy()));
            airfieldCollection.AddAirfield(std::make_shared<AirfieldModel>(5, "EGMC", DefaultHierarchy()));
            airfieldCollection.AddAirfield(std::make_shared<AirfieldModel>(6, "EGMD", DefaultHierarchy()));
            airfieldCollection.AddAirfield(std::make_shared<AirfieldModel>(7, "EGKA", DefaultHierarchy()));
        }

        [[nodiscard]] auto DefaultHierarchy() const -> std::unique_ptr<ControllerPositionHierarchy>
        {
            auto hierarchy = std::make_unique<ControllerPositionHierarchy>();
            hierarchy->AddPosition(this->controllerCollection.FetchPositionById(6));
            return std::move(hierarchy);
        }

        AirfieldCollection airfieldCollection;
        ControllerPositionCollection controllerCollection;
        ActiveCallsignCollection activeCallsigns;
        std::shared_ptr<AirfieldServiceProviderCollection> serviceProviders;
        AirfieldOwnershipManager ownership;
        StoredFlightplanCollection flightplans;
        NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
        Login login;
        UserMessager userMessager;
        AirfieldOwnershipHandler handler;
    };

    TEST_F(ControllerAirfieldOwnershipHandlerTest, ProcessCommandOwnedReturnsTrueOnCallsignMatch)
    {
        ON_CALL(this->plugin, ChatAreaMessage(_, _, _, _, _, _, _, _)).WillByDefault(Return());
        EXPECT_TRUE(this->handler.ProcessCommand(".ukcp owned EGKK_APP"));
    }

    TEST_F(ControllerAirfieldOwnershipHandlerTest, ProcessCommandOwnedSendsAMessage)
    {
        ON_CALL(
            this->plugin,
            ChatAreaMessage(
                "UKCP_Query", "UKCP", "EGKK_TWR owns the following airfields: EGKK", true, true, true, true, false))
            .WillByDefault(Return());
        EXPECT_TRUE(this->handler.ProcessCommand(".ukcp owned EGKK_TWR"));
    }

    TEST_F(ControllerAirfieldOwnershipHandlerTest, ProcessCommandOwnerReturnsTrueOnAirfieldMatch)
    {
        ON_CALL(this->plugin, ChatAreaMessage(_, _, _, _, _, _, _, _)).WillByDefault(Return());
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
                false))
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
        EXPECT_FALSE(this->serviceProviders->AirfieldHasDeliveryProvider("EGKK"));
        EXPECT_FALSE(this->serviceProviders->AirfieldHasDeliveryProvider("EGLL"));
    }

    TEST_F(ControllerAirfieldOwnershipHandlerTest, ActiveCallsignDisconnectRefreshesOwnership)
    {
        ActiveCallsign gatwick = this->activeCallsigns.GetCallsign("EGKK_TWR");
        this->activeCallsigns.RemoveCallsign(this->activeCallsigns.GetCallsign("EGKK_TWR"));
        this->handler.ActiveCallsignRemoved(gatwick);
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGKK_APP"),
            *this->serviceProviders->DeliveryProviderForAirfield("EGKK")->controller);
    }

    TEST_F(
        ControllerAirfieldOwnershipHandlerTest, ActiveCallsignDisconnectResetsOwnerToOtherCallsignIfPositionStillActive)
    {
        // Add the spare controller
        ControllerPosition pos(1, "EGKK_TWR", 199.998, {"EGKK"}, true, false);
        this->activeCallsigns.AddCallsign(ActiveCallsign("EGKK_1_TWR", "Another Guy", pos, false));

        ActiveCallsign gatwick = this->activeCallsigns.GetCallsign("EGKK_TWR");
        this->activeCallsigns.RemoveCallsign(this->activeCallsigns.GetCallsign("EGKK_TWR"));
        this->handler.ActiveCallsignRemoved(gatwick);
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGKK_1_TWR"),
            *this->serviceProviders->DeliveryProviderForAirfield("EGKK")->controller);
    }

    TEST_F(ControllerAirfieldOwnershipHandlerTest, NewActiveCallsignEventRefreshesTopDown)
    {
        this->activeCallsigns.AddCallsign(
            ActiveCallsign("EGKK_DEL", "Test", *this->controllerCollection.FetchPositionByCallsign("EGKK_DEL"), false));
        this->handler.ActiveCallsignAdded(this->activeCallsigns.GetCallsign("EGKK_DEL"));
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGKK_DEL"),
            *this->serviceProviders->DeliveryProviderForAirfield("EGKK")->controller);
    }

    TEST_F(ControllerAirfieldOwnershipHandlerTest, ControllerUpdateEventUpdatesTopDownMultiple)
    {
        this->activeCallsigns.AddCallsign(ActiveCallsign(
            "LTC_S_CTR", "Test", *this->controllerCollection.FetchPositionByCallsign("LTC_S_CTR"), false));
        this->handler.ActiveCallsignAdded(this->activeCallsigns.GetCallsign("LTC_S_CTR"));
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGKK_TWR"),
            *this->serviceProviders->DeliveryProviderForAirfield("EGKK")->controller);
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("EGLL_S_TWR"),
            *this->serviceProviders->DeliveryProviderForAirfield("EGLL")->controller);
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("LTC_S_CTR"),
            *this->serviceProviders->DeliveryProviderForAirfield("EGLC")->controller);
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("LTC_S_CTR"),
            *this->serviceProviders->DeliveryProviderForAirfield("EGKB")->controller);
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("LTC_S_CTR"),
            *this->serviceProviders->DeliveryProviderForAirfield("EGMC")->controller);
        EXPECT_EQ(
            this->activeCallsigns.GetCallsign("LTC_S_CTR"),
            *this->serviceProviders->DeliveryProviderForAirfield("EGMD")->controller);
    }
} // namespace UKControllerPluginTest::Ownership
