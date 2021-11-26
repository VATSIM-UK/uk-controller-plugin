#include "prenote/PrenoteService.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"
#include "controller/ActiveCallsignCollection.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ControllerPosition.h"
#include "message/UserMessager.h"
#include "prenote/AbstractPrenote.h"
#include "prenote/PublishedPrenote.h"
#include "controller/ControllerPositionHierarchy.h"
#include "airfield/AirfieldModel.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"
#include "flightrule/FlightRuleCollection.h"
#include "prenote/PublishedPrenoteMapper.h"
#include "prenote/PublishedPrenoteCollection.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::FlightRules::FlightRuleCollection;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPlugin::Prenote::AbstractPrenote;
using UKControllerPlugin::Prenote::PrenoteService;
using UKControllerPlugin::Prenote::PublishedPrenote;
using UKControllerPlugin::Prenote::PublishedPrenoteCollection;
using UKControllerPlugin::Prenote::PublishedPrenoteMapper;
using UKControllerPlugin::Sid::SidCollection;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest::Prenote {
    class PrenoteServiceTest : public Test
    {
        public:
        PrenoteServiceTest()
            : hierarchyPointer(std::make_unique<ControllerPositionHierarchy>()), hierarchy(*hierarchyPointer),
              mapper(prenotes, airfields, sids, flightRules)
        {
            // Add controllers
            this->controllerUser = std::shared_ptr<ControllerPosition>(
                new ControllerPosition(1, "EGKK_GND", 121.800, {"EGKK"}, true, false));
            this->controllerOther = std::shared_ptr<ControllerPosition>(
                new ControllerPosition(2, "EGKK_APP", 126.820, {"EGKK"}, true, false));
            this->controllerNoLondon = std::shared_ptr<ControllerPosition>(
                new ControllerPosition(3, "LON_S_CTR", 129.420, {"EGKK"}, true, false));
            this->activeCallsigns.AddUserCallsign(
                ActiveCallsign("EGKK_GND", "Testy McTestface", *this->controllerUser, true));
            this->activeCallsigns.AddCallsign(
                ActiveCallsign("EGKK_APP", "Testy McTestface II", *this->controllerOther, false));

            this->activeCallsigns.AddCallsign(
                ActiveCallsign("LON_S_CTR", "Testy McTestface III", *this->controllerNoLondon, false));

            this->airfieldOwnership.SetProvidersForAirfield(
                "EGKK",
                std::vector<std::shared_ptr<ServiceProvision>>{std::make_shared<ServiceProvision>(
                    ServiceType::Delivery,
                    std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>(
                        this->activeCallsigns.GetUserCallsign()))});

            this->messager = std::make_unique<UserMessager>(this->mockPlugin);
            this->service = std::make_unique<PrenoteService>(
                this->mapper, this->airfieldOwnership, this->activeCallsigns, *this->messager);

            this->prenotes.Add(std::make_shared<PublishedPrenote>(1, "test", std::move(this->hierarchyPointer)));

            this->sids.AddSid(
                std::make_shared<StandardInstrumentDeparture>("EGKK", "ADMAG2X", 1, 2, 3, std::set<int>{1}));
            this->sids.AddSid(
                std::make_shared<StandardInstrumentDeparture>("EGKK", "LAM4M", 1, 2, 3, std::set<int>{1, 2}));

            ON_CALL(this->mockFlightplan, GetOrigin).WillByDefault(Return("EGKK"));
            ON_CALL(this->mockFlightplan, GetCallsign).WillByDefault(Return("BAW123"));
            ON_CALL(this->mockFlightplan, GetSidName).WillByDefault(Return("ADMAG2X"));
        }

        FlightRuleCollection flightRules;
        AirfieldCollection airfields;
        SidCollection sids;
        AirfieldServiceProviderCollection airfieldOwnership;
        PublishedPrenoteCollection prenotes;
        std::unique_ptr<ControllerPositionHierarchy> hierarchyPointer;
        ControllerPositionHierarchy& hierarchy;
        PublishedPrenoteMapper mapper;
        std::shared_ptr<PublishedPrenote> prenote;
        std::unique_ptr<PrenoteService> service;
        ActiveCallsignCollection activeCallsigns;
        std::shared_ptr<ControllerPosition> controllerUser;
        std::shared_ptr<ControllerPosition> controllerOther;
        std::shared_ptr<ControllerPosition> controllerNoLondon;
        std::unique_ptr<UserMessager> messager;
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
    };

    TEST_F(PrenoteServiceTest, CancelPrenoteHandlesNonPrenotedAircraft)
    {
        EXPECT_NO_THROW(this->service->CancelPrenote(this->mockFlightplan));
    }

    TEST_F(PrenoteServiceTest, SendPrenotesDoesNothingIfAirfieldNotOwnedByUser)
    {
        // Flush ownership so nobody owns Gatwick.
        this->airfieldOwnership.Flush();

        EXPECT_NO_THROW(this->service->SendPrenotes(this->mockFlightplan));
    }

    TEST_F(PrenoteServiceTest, SendPrenotesDoesNothingIfNobodyToPrenote)
    {
        // The target controller is offline.
        this->hierarchy.AddPosition(this->controllerOther);
        this->activeCallsigns.RemoveCallsign(
            ActiveCallsign("EGKK_APP", "Testy McTestface II", *this->controllerOther, false));

        EXPECT_NO_THROW(this->service->SendPrenotes(this->mockFlightplan));
    }

    TEST_F(PrenoteServiceTest, SendPrenotesDoesNothingIfPrenotingSelf)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_))
            .Times(0);

        // Add self to the prenote hierarchy
        this->hierarchy.AddPosition(this->controllerUser);

        EXPECT_NO_THROW(this->service->SendPrenotes(this->mockFlightplan));
    }

    TEST_F(PrenoteServiceTest, SendPrenotesSendsApplicablePrenotes)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                "PRENOTE",
                "UKCP",
                "Prenote to EGKK_APP required for BAW123 (EGKK/ADMAG2X)",
                true,
                true,
                true,
                true,
                true))
            .Times(1);

        // Add self to the prenote hierarchy
        this->hierarchy.AddPosition(this->controllerOther);

        this->service->SendPrenotes(this->mockFlightplan);
    }

    TEST_F(PrenoteServiceTest, SendPrenotesStopsAfterFirstApplicableController)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                "PRENOTE",
                "UKCP",
                "Prenote to EGKK_APP required for BAW123 (EGKK/ADMAG2X)",
                true,
                true,
                true,
                true,
                true))
            .Times(1);

        // Add self to the prenote hierarchy
        this->hierarchy.AddPosition(this->controllerOther);
        this->hierarchy.AddPosition(this->controllerNoLondon);

        this->service->SendPrenotes(this->mockFlightplan);
    }

    TEST_F(PrenoteServiceTest, ControllerFlightPlanDataDoesNotPrenoteIfAlreadyDone)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                "PRENOTE",
                "UKCP",
                "Prenote to EGKK_APP required for BAW123 (EGKK/ADMAG2X)",
                true,
                true,
                true,
                true,
                true))
            .Times(1);

        // Add self to the prenote hierarchy
        this->hierarchy.AddPosition(this->controllerOther);
        this->hierarchy.AddPosition(this->controllerNoLondon);

        this->service->SendPrenotes(this->mockFlightplan);
        this->service->SendPrenotes(this->mockFlightplan);
    }

    TEST_F(PrenoteServiceTest, SendPrenotesSendsMultipleApplicablePrenotes)
    {
        auto hierarchyTwo = std::make_unique<ControllerPositionHierarchy>();
        hierarchyTwo->AddPosition(this->controllerNoLondon);
        this->prenotes.Add(std::make_shared<PublishedPrenote>(2, "test", std::move(hierarchyTwo)));

        ON_CALL(this->mockFlightplan, GetSidName).WillByDefault(Return("LAM4M"));

        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                "PRENOTE",
                "UKCP",
                "Prenote to EGKK_APP required for BAW123 (EGKK/LAM4M)",
                true,
                true,
                true,
                true,
                true))
            .Times(1);

        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                "PRENOTE",
                "UKCP",
                "Prenote to LON_S_CTR required for BAW123 (EGKK/LAM4M)",
                true,
                true,
                true,
                true,
                true))
            .Times(1);

        this->hierarchy.AddPosition(this->controllerOther);
        this->hierarchy.AddPosition(this->controllerNoLondon);

        this->service->SendPrenotes(this->mockFlightplan);
    }

    TEST_F(PrenoteServiceTest, SendPrenotesSendsMultipleApplicablePrenotesAndDoesntDuplicateControllers)
    {
        auto hierarchyTwo = std::make_unique<ControllerPositionHierarchy>();
        hierarchyTwo->AddPosition(this->controllerOther);
        hierarchyTwo->AddPosition(this->controllerNoLondon);
        this->prenotes.Add(std::make_shared<PublishedPrenote>(2, "test", std::move(hierarchyTwo)));

        ON_CALL(this->mockFlightplan, GetSidName).WillByDefault(Return("LAM4M"));

        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                "PRENOTE",
                "UKCP",
                "Prenote to EGKK_APP required for BAW123 (EGKK/LAM4M)",
                true,
                true,
                true,
                true,
                true))
            .Times(1);

        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                "PRENOTE",
                "UKCP",
                "Prenote to LON_S_CTR required for BAW123 (EGKK/LAM4M)",
                true,
                true,
                true,
                true,
                true))
            .Times(0);

        this->hierarchy.AddPosition(this->controllerOther);
        this->hierarchy.AddPosition(this->controllerNoLondon);

        this->service->SendPrenotes(this->mockFlightplan);
    }

    TEST_F(PrenoteServiceTest, CancelPrenoteClearsPrenoteMarker)
    {
        EXPECT_CALL(
            mockPlugin,
            ChatAreaMessage(
                "PRENOTE",
                "UKCP",
                "Prenote to EGKK_APP required for BAW123 (EGKK/ADMAG2X)",
                true,
                true,
                true,
                true,
                true))
            .Times(2);

        // Add self to the prenote hierarchy
        this->hierarchy.AddPosition(this->controllerOther);
        this->hierarchy.AddPosition(this->controllerNoLondon);

        this->service->SendPrenotes(this->mockFlightplan);
        this->service->CancelPrenote(this->mockFlightplan);
        this->service->SendPrenotes(this->mockFlightplan);
    }

    TEST_F(PrenoteServiceTest, CancelPrenoteHandlesNonExistantPrenotes)
    {
        ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

        EXPECT_NO_THROW(this->service->CancelPrenote(this->mockFlightplan));
    }
} // namespace UKControllerPluginTest::Prenote
