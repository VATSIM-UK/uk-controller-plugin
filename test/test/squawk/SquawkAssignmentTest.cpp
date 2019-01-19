#include "pch/pch.h"
#include "squawk/SquawkAssignment.h"
#include "mock/MockCurlApi.h"
#include "mock/MockTaskRunnerInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ActiveCallsign.h"
#include "airfield/AirfieldOwnershipManager.h"
#include "airfield/Airfield.h"
#include "controller/ActiveCallsignCollection.h"
#include "flightplan/StoredFlightplan.h"

using UKControllerPlugin::Squawk::SquawkAssignment;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Airfield::AirfieldOwnershipManager;
using UKControllerPlugin::Airfield::Airfield;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Squawk {

        class SquawkAssignmentTest : public Test
        {
            public:
                SquawkAssignmentTest(void)
                    : kkApp("EGKK_APP", 126.820, "APP", { "EGKK" }),
                    lonS("LON_S_CTR", 129.420, "CTR", { "EGLL" }),
                    airfieldOwnership(this->airfields, this->activeCallsigns),
                    assignment(this->plans, this->pluginLoopback, this->airfieldOwnership, this->activeCallsigns, false)
                {

                };

                void SetUp()
                {
                    this->mockFlightplan = std::shared_ptr<MockEuroScopeCFlightPlanInterface>(
                        new NiceMock<MockEuroScopeCFlightPlanInterface>
                    );
                    this->mockRadarTarget = std::shared_ptr<MockEuroScopeCRadarTargetInterface>(
                        new NiceMock<MockEuroScopeCRadarTargetInterface>
                    );

                    this->airfields.AddAirfield(std::unique_ptr<Airfield>(new Airfield("EGKK", { "EGKK_APP" })));
                    this->activeCallsigns.AddUserCallsign(
                        ActiveCallsign(
                            "EGKK_APP",
                            "Testy McTestface",
                            this->kkApp
                        )
                    );
                    this->activeCallsigns.AddCallsign(
                        ActiveCallsign(
                            "LON_S_CTR",
                            "Boaty McBoatface",
                            this->lonS
                        )
                    );
                    this->airfieldOwnership.RefreshOwner("EGKK");
                }

                NiceMock<MockCurlApi> mockCurl;
                NiceMock<MockEuroscopePluginLoopbackInterface> pluginLoopback;
                std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockFlightplan;
                std::shared_ptr<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
                StoredFlightplanCollection plans;
                ActiveCallsignCollection activeCallsigns;
                AirfieldOwnershipManager airfieldOwnership;
                AirfieldCollection airfields;
                ControllerPosition kkApp;
                ControllerPosition lonS;
                SquawkAssignment assignment;
        };

        TEST_F(SquawkAssignmentTest, ForceAssignmentAllowedReturnsTrueIfUserActiveAndTrackingFlightplan)
        {
            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));
            EXPECT_TRUE(this->assignment.ForceAssignmentAllowed(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, ForceAssignmentAllowedReturnsTrueIfUserActiveAndFlightplanNotTracked)
        {
            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));
            ON_CALL(*this->mockFlightplan, IsTracked())
                .WillByDefault(Return(false));
            EXPECT_TRUE(this->assignment.ForceAssignmentAllowed(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, ForceAssignmentAllowedReturnsFalseIfFlightplanTrackedByAnotherUser)
        {
            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));
            ON_CALL(*this->mockFlightplan, IsTracked())
                .WillByDefault(Return(true));
            EXPECT_FALSE(this->assignment.ForceAssignmentAllowed(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, ForceAssignmentAllowedReturnsFalseIfUserNotActive)
        {
            this->activeCallsigns.Flush();
            EXPECT_FALSE(this->assignment.ForceAssignmentAllowed(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, GeneralAssignmentOnInactiveUserReturnsFalse)
        {
            this->activeCallsigns.Flush();
            EXPECT_FALSE(this->assignment.GeneralAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, GeneralAssignmentOnTrackedAlreadyAssignedReturnsFalse)
        {
            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(true));

            EXPECT_FALSE(this->assignment.GeneralAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, GeneralAssignmentOnTrackedNotAlreadyAssignedReturnsTrue)
        {
            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            EXPECT_TRUE(this->assignment.GeneralAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, GeneralAssignmentOnUntrackedTooFastReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed + 1));

            EXPECT_FALSE(this->assignment.GeneralAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, GeneralAssignmentOnUntrackedTooFarFromOriginReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentDistanceFromOrigin + 1));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed));

            EXPECT_FALSE(this->assignment.GeneralAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, GeneralAssignmentOnUntrackedAlreadyAssignedReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentDistanceFromOrigin));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed));

            EXPECT_FALSE(this->assignment.GeneralAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, GeneralAssignmentOnUntrackedAirfieldNotOwnedReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentDistanceFromOrigin));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed));

            // Flush airfield ownership so nobody owns Gatwick
            this->airfieldOwnership.Flush();
            EXPECT_FALSE(this->assignment.GeneralAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, GeneralAssignmentOnUntrackedReturnsTrue)
        {

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentDistanceFromOrigin));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed));

            EXPECT_TRUE(this->assignment.GeneralAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentTrackedNotTrackedReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            EXPECT_FALSE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentTrackedEnrouteTooHighReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetCruiseLevel())
                .WillByDefault(Return(this->assignment.maxAssignmentAltitude + 1));

            this->activeCallsigns.Flush();
            ControllerPosition controller("LON_S_CTR", 129.420, "CTR", { "EGLL" });
            this->activeCallsigns.AddUserCallsign(
                ActiveCallsign(
                    "LON_S_CTR",
                    "Boaty McBoatface",
                    controller
                )
            );
            EXPECT_FALSE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentTrackedEnrouteAlreadyAssignedReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetCruiseLevel())
                .WillByDefault(Return(this->assignment.maxAssignmentAltitude));

            EXPECT_FALSE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentTrackedEnroutePositionLowLevelReturnsTrue)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetCruiseLevel())
                .WillByDefault(Return(this->assignment.maxAssignmentAltitude));

            EXPECT_TRUE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentTrackedAlreadyAssignedReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(true));

            EXPECT_FALSE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentTrackedTooHighReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel())
                .WillByDefault(Return(this->assignment.maxAssignmentAltitude + 1));

            EXPECT_FALSE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentTrackedOutOfLarsRangeReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel())
                .WillByDefault(Return(this->assignment.maxAssignmentAltitude));

            ON_CALL(*this->mockRadarTarget, GetPosition())
                .WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_))
                .WillByDefault(Return(this->assignment.trackedLarsRadius + 1));

            EXPECT_FALSE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentTrackedReturnsTrue)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel())
                .WillByDefault(Return(this->assignment.maxAssignmentAltitude));

            ON_CALL(*this->mockRadarTarget, GetPosition())
                .WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_))
                .WillByDefault(Return(this->assignment.trackedLarsRadius));

            EXPECT_TRUE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }
        TEST_F(SquawkAssignmentTest, LocalAssignmentUntrackedTooFastReturnFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed + 1));

            EXPECT_FALSE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentUntrackedTooFarFromOriginReturnFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentDistanceFromOrigin + 1));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed));

            EXPECT_FALSE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentUntrackedAirfieldNowOwnedReturnFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGGD"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGGD"));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentDistanceFromOrigin));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed));

            EXPECT_FALSE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentUntrackedOriginAndDestinationSameReturnTrue)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentDistanceFromOrigin));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed));

            EXPECT_TRUE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentUntrackedVfrFlightReturnTrue)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGGD"));

            ON_CALL(*this->mockFlightplan, GetFlightRules())
                .WillByDefault(Return("V"));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentDistanceFromOrigin));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed));

            EXPECT_TRUE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentUntrackedNoSidLowFlightFlightReturnTrue)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGGD"));

            ON_CALL(*this->mockFlightplan, GetFlightRules())
                .WillByDefault(Return("I"));

            ON_CALL(*this->mockFlightplan, HasSid())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetCruiseLevel())
                .WillByDefault(Return(this->assignment.maxAssignmentAltitude));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentDistanceFromOrigin));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed));

            EXPECT_TRUE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentUntrackedNoSecondaryConditionReturnFalse)
        {

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGGD"));

            ON_CALL(*this->mockFlightplan, GetFlightRules())
                .WillByDefault(Return("I"));

            ON_CALL(*this->mockFlightplan, HasSid())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetCruiseLevel())
                .WillByDefault(Return(this->assignment.maxAssignmentAltitude));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentDistanceFromOrigin));

            ON_CALL(*this->mockRadarTarget, GetGroundSpeed())
                .WillByDefault(Return(this->assignment.untrackedMaxAssignmentSpeed));

            EXPECT_FALSE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, LocalAssignmentReturnsFalseOnInactiveUser)
        {
            this->activeCallsigns.Flush();
            EXPECT_FALSE(this->assignment.LocalAssignmentNeeded(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkAssignmentTest, PreviousReassignmentAlreadyAssignedReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(true));

            EXPECT_FALSE(
                this->assignment.PreviousSquawkNeedsReassignment(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkAssignmentTest, PreviousReassignmentNotTrackedByUserReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            EXPECT_FALSE(
                this->assignment.PreviousSquawkNeedsReassignment(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkAssignmentTest, PreviousReassignmentNoResidualSquawkReturnsFalse)
        {

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            EXPECT_FALSE(
                this->assignment.PreviousSquawkNeedsReassignment(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkAssignmentTest, PreviousReassignmentNoResidualFlightplanReturnFalse)
        {
            this->plans.UpdatePlan(StoredFlightplan("BAW456", "EGKK", "EGBB"));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            EXPECT_FALSE(
                this->assignment.PreviousSquawkNeedsReassignment(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkAssignmentTest, PreviousReassignmentNoResidualSquawkReturnFalse)
        {
            this->plans.UpdatePlan(StoredFlightplan("BAW123", "EGKK", "EGBB"));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            EXPECT_FALSE(
                this->assignment.PreviousSquawkNeedsReassignment(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkAssignmentTest, PreviousReassignmentInactiveUserReturnsFalse)
        {
            this->activeCallsigns.Flush();
            EXPECT_FALSE(
                this->assignment.PreviousSquawkNeedsReassignment(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkAssignmentTest, PreviousReassignmentReturnsTrue)
        {
            StoredFlightplan plan("BAW123", "EGKK", "EGBB");
            plan.SetPreviouslyAssignedSquawk("1234");
            this->plans.UpdatePlan(plan);

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            EXPECT_TRUE(
                this->assignment.PreviousSquawkNeedsReassignment(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkAssignmentTest, ForceAssignmentReturnsTrueIfPlanDifferent)
        {
            StoredFlightplan plan("BAW123", "EGKK", "EGBB");
            this->plans.UpdatePlan(plan);

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGPH"));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            EXPECT_TRUE(this->assignment.ForceAssignmentNeeded(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, ForceAssignmentReturnsFalseIfPlanSame)
        {
            StoredFlightplan plan("BAW123", "EGKK", "EGBB");
            this->plans.UpdatePlan(plan);

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGBB"));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            EXPECT_FALSE(this->assignment.ForceAssignmentNeeded(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, ForceAssignmentReturnsFalseIfNoPlanStored)
        {
            EXPECT_FALSE(this->assignment.ForceAssignmentNeeded(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, ForceAssignmentOrNoSquawkAssignedReturnsTrueIfNoSquawkAssigned)
        {
            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            EXPECT_TRUE(this->assignment.ForceAssignmentOrNoSquawkAssigned(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, ForceAssignmentOrNoSquawkAssignedReturnsFalseIfNoForceSquawkAssigned)
        {
            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(true));

            EXPECT_FALSE(this->assignment.ForceAssignmentOrNoSquawkAssigned(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, ForceAssignmentOrNoSquawkAssignedReturnsTrueIfForce)
        {
            StoredFlightplan plan("BAW123", "EGKK", "EGBB");
            this->plans.UpdatePlan(plan);

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGPH"));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(true));

            EXPECT_TRUE(this->assignment.ForceAssignmentOrNoSquawkAssigned(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, ItKnowsWhetherSquawksAreDisabled)
        {
            EXPECT_FALSE(this->assignment.disabled);
        }

        TEST_F(SquawkAssignmentTest, CircuitAssignmentReturnsFalseOnNotVfr)
        {
            ON_CALL(*this->mockFlightplan, IsVfr())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetRawRouteString())
                .WillByDefault(Return("CIRCUITS"));

            EXPECT_FALSE(this->assignment.CircuitAssignmentNeeded(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, CircuitAssignmentReturnsIfSquawkAlreadyAssigned)
        {
            ON_CALL(*this->mockFlightplan, IsVfr())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetRawRouteString())
                .WillByDefault(Return("CIRCUITS"));

            EXPECT_FALSE(this->assignment.CircuitAssignmentNeeded(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, CircuitAssignmentReturnsTrueOnCircuits)
        {
            ON_CALL(*this->mockFlightplan, IsVfr())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetRawRouteString())
                .WillByDefault(Return("CIRCUITS"));

            EXPECT_TRUE(this->assignment.CircuitAssignmentNeeded(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, CircuitAssignmentReturnsTrueOnCircuit)
        {
            ON_CALL(*this->mockFlightplan, IsVfr())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetRawRouteString())
                .WillByDefault(Return("CIRCUIT"));

            EXPECT_TRUE(this->assignment.CircuitAssignmentNeeded(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, CircuitAssignmentReturnsTrueOnVfrCircuit)
        {
            ON_CALL(*this->mockFlightplan, IsVfr())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetRawRouteString())
                .WillByDefault(Return("VFR CIRCUIT"));

            EXPECT_TRUE(this->assignment.CircuitAssignmentNeeded(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, CircuitAssignmentReturnsTrueOnVfrCircuits)
        {
            ON_CALL(*this->mockFlightplan, IsVfr())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetRawRouteString())
                .WillByDefault(Return("VFR CIRCUITS"));

            EXPECT_TRUE(this->assignment.CircuitAssignmentNeeded(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, CircuitAssignmentHandlesCaseInsensitive)
        {
            ON_CALL(*this->mockFlightplan, IsVfr())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetRawRouteString())
                .WillByDefault(Return("vfr circuits"));

            EXPECT_TRUE(this->assignment.CircuitAssignmentNeeded(*this->mockFlightplan));
        }

        TEST_F(SquawkAssignmentTest, CircuitAssignmentDoesntAcceptCircuitsThatArentStartOfFlightplan)
        {
            ON_CALL(*this->mockFlightplan, IsVfr())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetRawRouteString())
                .WillByDefault(Return("CHEW VALLEY FROME VFR CIRCUITS"));

            EXPECT_FALSE(this->assignment.CircuitAssignmentNeeded(*this->mockFlightplan));
        }
    }  // namespace Squawk
}  // namespace UKControllerPluginTest
