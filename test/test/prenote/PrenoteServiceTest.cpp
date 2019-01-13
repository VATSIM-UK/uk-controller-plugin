#include "pch/pch.h"
#include "prenote/PrenoteService.h"
#include "airfield/AirfieldOwnershipManager.h"
#include "controller/ActiveCallsignCollection.h"
#include "airfield/AirfieldCollection.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "controller/ControllerPosition.h"
#include "controller/ActiveCallsign.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "message/UserMessager.h"
#include "prenote/AbstractPrenote.h"
#include "controller/ControllerPositionHierarchy.h"
#include "airfield/Airfield.h"
#include "mock/MockUserSettingProviderInterface.h"

using UKControllerPlugin::Prenote::AbstractPrenote;
using UKControllerPlugin::Prenote::PrenoteService;
using UKControllerPlugin::Airfield::AirfieldOwnershipManager;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Airfield::Airfield;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Prenote {

        class EventHandlerPrenote : public AbstractPrenote
        {
            public:
                EventHandlerPrenote(
                    std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchy> controllers,
                    bool applicable
                ) : AbstractPrenote(std::move(controllers)), applicable(applicable)
                {

                };

                bool IsApplicable(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan
                ) const override {
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
                    this->airfields = std::make_unique<AirfieldCollection>();
                    this->airfields->AddAirfield(std::unique_ptr<Airfield>(new Airfield("EGKK", { "EGKK_GND" })));
                    this->airfieldOwnership = std::make_unique<AirfieldOwnershipManager>(
                        *this->airfields,
                        this->activeCallsigns
                    );

                    // Add controllers
                    this->controllerUser = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("EGKK_GND", 121.800, "GND", { "EGKK" })
                    );
                    this->controllerOther = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("EGKK_APP", 126.820, "APP", { "EGKK" })
                    );
                    this->controllerNoLondon = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("LON_S_CTR", 129.420, "CTR", { "EGKK" })
                    );
                    this->activeCallsigns.AddUserCallsign(
                        ActiveCallsign(
                            "EGKK_GND",
                            "Testy McTestface",
                            *this->controllerUser
                        )
                    );
                    this->activeCallsigns.AddCallsign(
                        ActiveCallsign(
                            "EGKK_APP",
                            "Testy McTestface II",
                            *this->controllerOther
                        )
                    );

                    this->activeCallsigns.AddCallsign(
                        ActiveCallsign(
                            "LON_S_CTR",
                            "Testy McTestface III",
                            *this->controllerNoLondon
                        )
                    );

                    this->hierarchy = std::make_unique<ControllerPositionHierarchy>();
                    this->airfieldOwnership->RefreshOwner("EGKK");
                    this->messager = std::make_unique<UserMessager>(this->mockPlugin);
                    this->service = std::make_unique<PrenoteService>(
                        *this->airfieldOwnership,
                        this->activeCallsigns,
                        *this->messager
                    );
                };

                std::unique_ptr<PrenoteService> service;
                std::unique_ptr<AirfieldCollection> airfields;
                std::unique_ptr<AirfieldOwnershipManager> airfieldOwnership;
                ActiveCallsignCollection activeCallsigns;
                std::unique_ptr<ControllerPosition> controllerUser;
                std::unique_ptr<ControllerPosition> controllerOther;
                std::unique_ptr<ControllerPosition> controllerNoLondon;
                std::unique_ptr<UserMessager> messager;
                std::unique_ptr<ControllerPositionHierarchy> hierarchy;
                NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
        };

        TEST_F(PrenoteServiceTest, CancelPrenoteHandlesNonPrenotedAircraft)
        {
            ON_CALL(this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            EXPECT_NO_THROW(this->service->CancelPrenote(this->mockFlightplan));
        }

        TEST_F(PrenoteServiceTest, SendPrenotesDoesNothingIfAirfieldNotOwnedByUser)
        {
            ON_CALL(this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            // Flush ownership so nobody owns Gatwick.
            this->airfieldOwnership->Flush();

            EXPECT_NO_THROW(
                this->service->SendPrenotes(this->mockFlightplan)
            );
        }

        TEST_F(PrenoteServiceTest, CountPrenotesReturnsNumberOfPrenotesStored)
        {
            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(*this->controllerOther);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), false));

            EXPECT_EQ(1, this->service->CountPrenotes());
        }

        TEST_F(PrenoteServiceTest, SendPrenotesSkipsNonApplicablePrenotes)
        {
            ON_CALL(this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(*this->controllerOther);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), false));

            EXPECT_NO_THROW(
                this->service->SendPrenotes(this->mockFlightplan)
            );
        }

        TEST_F(PrenoteServiceTest, SendPrenotesDoesNothingIfNobodyToPrenote)
        {
            ON_CALL(this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            // The target controller is offline.
            this->hierarchy->AddPosition(*this->controllerOther);
            this->activeCallsigns.RemoveCallsign(
                ActiveCallsign("EGKK_APP", "Testy McTestface II", *this->controllerOther)
            );
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            EXPECT_NO_THROW(
                this->service->SendPrenotes(this->mockFlightplan)
            );
        }

        TEST_F(PrenoteServiceTest, SendPrenotesDoesNothingIfPrenotingSelf)
        {
            ON_CALL(this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(*this->controllerUser);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            EXPECT_NO_THROW(
                this->service->SendPrenotes(this->mockFlightplan)
            );
        }

        TEST_F(PrenoteServiceTest, SendPrenotesSendsApplicablePrenotes)
        {
            ON_CALL(this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

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
                    true
                ))
                .Times(1);

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(*this->controllerOther);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            this->service->SendPrenotes(this->mockFlightplan);
        }

        TEST_F(PrenoteServiceTest, SendPrenotesStopsAfterFirstApplicableController)
        {
            ON_CALL(this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

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
                    true
                ))
                .Times(1);

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(*this->controllerOther);
            this->hierarchy->AddPosition(*this->controllerNoLondon);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            this->service->SendPrenotes(this->mockFlightplan);
        }

        TEST_F(PrenoteServiceTest, ControllerFlightPlanDataDoesNotPrenoteIfAlreadyDone)
        {
            ON_CALL(this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

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
                    true
                ))
                .Times(1);

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(*this->controllerOther);
            this->hierarchy->AddPosition(*this->controllerNoLondon);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            this->service->SendPrenotes(this->mockFlightplan);
            this->service->SendPrenotes(this->mockFlightplan);
        }

        TEST_F(PrenoteServiceTest, SendPrenotesSendsMultipleApplicablePrenotes)
        {
            ON_CALL(this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

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
                    true
                ))
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
                    true
                ))
                .Times(1);

            // Add self to the prenote hierarchy
            std::unique_ptr<ControllerPositionHierarchy> hierarchy2 = std::make_unique<ControllerPositionHierarchy>(
                *this->hierarchy
            );
            this->hierarchy->AddPosition(*this->controllerOther);
            hierarchy2->AddPosition(*this->controllerNoLondon);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(hierarchy2), true));

            this->service->SendPrenotes(this->mockFlightplan);
        }

        TEST_F(PrenoteServiceTest, CancelPrenoteClearsPrenoteMarker)
        {
            ON_CALL(this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

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
                    true
                ))
                .Times(2);

            // Add self to the prenote hierarchy
            this->hierarchy->AddPosition(*this->controllerOther);
            this->hierarchy->AddPosition(*this->controllerNoLondon);
            this->service->AddPrenote(std::make_unique<EventHandlerPrenote>(std::move(this->hierarchy), true));

            this->service->SendPrenotes(this->mockFlightplan);
            this->service->CancelPrenote(this->mockFlightplan);
            this->service->SendPrenotes(this->mockFlightplan);
        }
    }  // namespace Prenote
}  // namespace UKControllerPluginTest
