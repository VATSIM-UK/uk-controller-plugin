#include "prenote/PrenoteService.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"
#include "controller/ActiveCallsignCollection.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ControllerPosition.h"
#include "message/UserMessager.h"
#include "prenote/AbstractPrenote.h"
#include "controller/ControllerPositionHierarchy.h"
#include "airfield/AirfieldModel.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPlugin::Prenote::AbstractPrenote;
using UKControllerPlugin::Prenote::PrenoteService;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest {
    namespace Prenote {

        class EventHandlerPrenote : public AbstractPrenote
        {
            public:
            EventHandlerPrenote(
                std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchy> controllers,
                bool applicable)
                : AbstractPrenote(std::move(controllers)), applicable(applicable){

                                                           };

            bool
            IsApplicable(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan) const override
            {
                return this->applicable;
            }

            std::string GetSummaryString(void) const override
            {
                return "summary";
            }

            private:
            const bool applicable;
        };

        class PrenoteServiceTest : public Test
        {
            public:
            void SetUp(void)
            {
                this->airfieldOwnership = std::make_unique<AirfieldServiceProviderCollection>();

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

                this->hierarchy = std::make_unique<ControllerPositionHierarchy>();

                this->airfieldOwnership->SetProvidersForAirfield(
                    "EGKK",
                    std::vector<std::shared_ptr<ServiceProvision>>{std::make_shared<ServiceProvision>(
                        ServiceType::Delivery,
                        std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>(
                            this->activeCallsigns.GetUserCallsign()))});

                this->messager = std::make_unique<UserMessager>(this->mockPlugin);
                this->service =
                    std::make_unique<PrenoteService>(*this->airfieldOwnership, this->activeCallsigns, *this->messager);
            };

            std::unique_ptr<PrenoteService> service;
            std::unique_ptr<AirfieldServiceProviderCollection> airfieldOwnership;
            ActiveCallsignCollection activeCallsigns;
            std::shared_ptr<ControllerPosition> controllerUser;
            std::shared_ptr<ControllerPosition> controllerOther;
            std::shared_ptr<ControllerPosition> controllerNoLondon;
            std::unique_ptr<UserMessager> messager;
            std::unique_ptr<ControllerPositionHierarchy> hierarchy;
            NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
        };

        TEST_F(PrenoteServiceTest, CancelPrenoteHandlesNonPrenotedAircraft)
        {
            ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            EXPECT_NO_THROW(this->service->CancelPrenote(this->mockFlightplan));
        }

        TEST_F(PrenoteServiceTest, SendPrenotesDoesNothingIfAirfieldNotOwnedByUser)
        {
            ON_CALL(this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            // Flush ownership so nobody owns Gatwick.
            this->airfieldOwnership->Flush();

            EXPECT_NO_THROW(this->service->SendPrenotes(this->mockFlightplan));
        }

        TEST_F(PrenoteServiceTest, CountPrenotesReturnsNumberOfPrenotesStored)
        {
            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(this->controllerOther);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), false));

            EXPECT_EQ(1, this->service->CountPrenotes());
        }

        TEST_F(PrenoteServiceTest, SendPrenotesSkipsNonApplicablePrenotes)
        {
            ON_CALL(this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(this->controllerOther);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), false));

            EXPECT_NO_THROW(this->service->SendPrenotes(this->mockFlightplan));
        }

        TEST_F(PrenoteServiceTest, SendPrenotesDoesNothingIfNobodyToPrenote)
        {
            ON_CALL(this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            // The target controller is offline.
            this->hierarchy->AddPosition(this->controllerOther);
            this->activeCallsigns.RemoveCallsign(
                ActiveCallsign("EGKK_APP", "Testy McTestface II", *this->controllerOther, false));
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            EXPECT_NO_THROW(this->service->SendPrenotes(this->mockFlightplan));
        }

        TEST_F(PrenoteServiceTest, SendPrenotesDoesNothingIfPrenotingSelf)
        {
            ON_CALL(this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(this->controllerUser);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            EXPECT_NO_THROW(this->service->SendPrenotes(this->mockFlightplan));
        }

        TEST_F(PrenoteServiceTest, SendPrenotesSendsApplicablePrenotes)
        {
            ON_CALL(this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            EXPECT_CALL(
                mockPlugin,
                ChatAreaMessage(
                    "Prenote",
                    "UKCP",
                    "Prenote to EGKK_APP required for BAW123 (summary)",
                    true,
                    true,
                    true,
                    true,
                    true))
                .Times(1);

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(this->controllerOther);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            this->service->SendPrenotes(this->mockFlightplan);
        }

        TEST_F(PrenoteServiceTest, SendPrenotesStopsAfterFirstApplicableController)
        {
            ON_CALL(this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            EXPECT_CALL(
                mockPlugin,
                ChatAreaMessage(
                    "Prenote",
                    "UKCP",
                    "Prenote to EGKK_APP required for BAW123 (summary)",
                    true,
                    true,
                    true,
                    true,
                    true))
                .Times(1);

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(this->controllerOther);
            this->hierarchy->AddPosition(this->controllerNoLondon);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            this->service->SendPrenotes(this->mockFlightplan);
        }

        TEST_F(PrenoteServiceTest, ControllerFlightPlanDataDoesNotPrenoteIfAlreadyDone)
        {
            ON_CALL(this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            EXPECT_CALL(
                mockPlugin,
                ChatAreaMessage(
                    "Prenote",
                    "UKCP",
                    "Prenote to EGKK_APP required for BAW123 (summary)",
                    true,
                    true,
                    true,
                    true,
                    true))
                .Times(1);

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(this->controllerOther);
            this->hierarchy->AddPosition(this->controllerNoLondon);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            this->service->SendPrenotes(this->mockFlightplan);
            this->service->SendPrenotes(this->mockFlightplan);
        }

        TEST_F(PrenoteServiceTest, SendPrenotesSendsMultipleApplicablePrenotes)
        {
            ON_CALL(this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            EXPECT_CALL(
                mockPlugin,
                ChatAreaMessage(
                    "Prenote",
                    "UKCP",
                    "Prenote to EGKK_APP required for BAW123 (summary)",
                    true,
                    true,
                    true,
                    true,
                    true))
                .Times(1);

            EXPECT_CALL(
                mockPlugin,
                ChatAreaMessage(
                    "Prenote",
                    "UKCP",
                    "Prenote to LON_S_CTR required for BAW123 (summary)",
                    true,
                    true,
                    true,
                    true,
                    true))
                .Times(1);

            // Add self to the prenote hierarchy
            std::unique_ptr<ControllerPositionHierarchy> hierarchy2 =
                std::make_unique<ControllerPositionHierarchy>(*this->hierarchy);
            this->hierarchy->AddPosition(this->controllerOther);
            hierarchy2->AddPosition(this->controllerNoLondon);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(hierarchy2), true));

            this->service->SendPrenotes(this->mockFlightplan);
        }

        TEST_F(PrenoteServiceTest, CancelPrenoteClearsPrenoteMarker)
        {
            ON_CALL(this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            EXPECT_CALL(
                mockPlugin,
                ChatAreaMessage(
                    "Prenote",
                    "UKCP",
                    "Prenote to EGKK_APP required for BAW123 (summary)",
                    true,
                    true,
                    true,
                    true,
                    true))
                .Times(2);

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(this->controllerOther);
            this->hierarchy->AddPosition(this->controllerNoLondon);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            this->service->SendPrenotes(this->mockFlightplan);
            this->service->CancelPrenote(this->mockFlightplan);
            this->service->SendPrenotes(this->mockFlightplan);
        }

        TEST_F(PrenoteServiceTest, CancelPrenoteHandlesNonExistantPrenotes)
        {
            ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            EXPECT_NO_THROW(this->service->CancelPrenote(this->mockFlightplan));
        }
    } // namespace Prenote
} // namespace UKControllerPluginTest
