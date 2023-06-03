#include "squawk/SquawkGenerator.h"
#include "squawk/SquawkAssignmentDeleteForConspicuityFailedEvent.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "squawk/SquawkAssignment.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "controller/ControllerPosition.h"
#include "api/ApiNotFoundException.h"
#include "squawk/ApiSquawkAllocationHandler.h"
#include "ownership/ServiceProvision.h"
#include "test/EventBusTestCase.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiNotFoundException;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPlugin::Squawk::ApiSquawkAllocation;
using UKControllerPlugin::Squawk::ApiSquawkAllocationHandler;
using UKControllerPlugin::Squawk::SquawkAssignment;
using UKControllerPlugin::Squawk::SquawkGenerator;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest {
    namespace Squawk {

        class SquawkGeneratorTest : public UKControllerPluginUtilsTest::EventBusTestCase
        {
            public:
            void SetUp() override
            {
                UKControllerPluginUtilsTest::EventBusTestCase::SetUp();
                this->squawkAllocationHandler = std::make_shared<ApiSquawkAllocationHandler>(this->pluginLoopback);
                this->mockFlightplan = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();
                this->mockRadarTarget = std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
                this->mockSelfController = std::make_shared<NiceMock<MockEuroScopeCControllerInterface>>();
                this->airfieldOwnership = std::make_unique<AirfieldServiceProviderCollection>();
                this->assignmentRules = std::make_unique<SquawkAssignment>(
                    this->flightplans, this->pluginLoopback, *this->airfieldOwnership, this->activeCallsigns);
                this->generator = std::make_unique<SquawkGenerator>(
                    this->api,
                    &this->taskRunner,
                    *this->assignmentRules,
                    this->activeCallsigns,
                    this->flightplans,
                    this->squawkAllocationHandler);

                this->controller = std::unique_ptr<ControllerPosition>(
                    new ControllerPosition(1, "EGKK_APP", 126.820, {"EGKK"}, true, false));
                this->activeCallsigns.AddUserCallsign(
                    ActiveCallsign("EGKK_APP", "Testy McTestface", *this->controller, true));
                this->airfieldOwnership->SetProvidersForAirfield(
                    "EGKK",
                    std::vector<std::shared_ptr<ServiceProvision>>{std::make_shared<ServiceProvision>(
                        ServiceType::Delivery,
                        std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>(
                            this->activeCallsigns.GetUserCallsign()))});

                ON_CALL(*this->mockSelfController, IsVatsimRecognisedController()).WillByDefault(Return(true));
            }

            NiceMock<MockCurlApi> mockCurl;
            NiceMock<MockApiInterface> api;
            std::unique_ptr<SquawkGenerator> generator;
            MockTaskRunnerInterface taskRunner;
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplan;
            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
            std::shared_ptr<NiceMock<MockEuroScopeCControllerInterface>> mockSelfController;
            NiceMock<MockEuroscopePluginLoopbackInterface> pluginLoopback;
            StoredFlightplanCollection flightplans;
            ActiveCallsignCollection activeCallsigns;
            std::unique_ptr<SquawkAssignment> assignmentRules;
            std::unique_ptr<AirfieldServiceProviderCollection> airfieldOwnership;
            std::unique_ptr<ControllerPosition> controller;
            std::shared_ptr<ApiSquawkAllocationHandler> squawkAllocationHandler;
        };

        TEST_F(SquawkGeneratorTest, GeneralSquawkReturnsFalseOnNoAction)
        {
            this->activeCallsigns.Flush();
            EXPECT_FALSE(
                this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, GeneralSquawkReturnsFalseOnNoActionRequestAlreadyHappening)
        {
            MockTaskRunnerInterface mockRunnerNoExecute(false);
            SquawkGenerator newGenerator(
                this->api,
                &mockRunnerNoExecute,
                *this->assignmentRules,
                this->activeCallsigns,
                this->flightplans,
                this->squawkAllocationHandler);

            ON_CALL(*this->mockFlightplan, SetSquawk("7000")).WillByDefault(Return());

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel).WillByDefault(Return(1));

            EXPECT_TRUE(newGenerator.RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
            EXPECT_FALSE(newGenerator.RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, GeneralSquawkSetsAircraftSquawks)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination()).WillByDefault(Return("EGPF"));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel).WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("7000")).WillRepeatedly(Return());

            ApiSquawkAllocation allocation{"BAW1252", "1423"};

            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252")).Times(1).WillOnce(Return(allocation));

            this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget);
            EXPECT_TRUE(allocation == this->squawkAllocationHandler->First());
            EXPECT_EQ(1, this->squawkAllocationHandler->Count());
        }

        TEST_F(SquawkGeneratorTest, GeneralSquawkCreatesAssignmentIfNoneExists)
        {
            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel).WillByDefault(Return(1));

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination()).WillByDefault(Return("EGPF"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("7000")).WillRepeatedly(Return());

            ApiSquawkAllocation allocation{"BAW1252", "4521"};
            EXPECT_CALL(this->api, CreateGeneralSquawkAssignment("BAW1252", "EGKK", "EGPF"))
                .Times(1)
                .WillOnce(Return(allocation));

            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252"))
                .Times(1)
                .WillOnce(Throw(ApiNotFoundException("Not found")));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget);
            EXPECT_TRUE(allocation == this->squawkAllocationHandler->First());
            EXPECT_EQ(1, this->squawkAllocationHandler->Count());
        }

        TEST_F(SquawkGeneratorTest, GeneralSquawkForcesSquawkWhereNecessary)
        {
            StoredFlightplan storedPlan("BAW1252", "EGKK", "EGPH");
            this->flightplans.UpdatePlan(storedPlan);

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination()).WillByDefault(Return("EGPF"));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel).WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("7000")).WillRepeatedly(Return());

            ApiSquawkAllocation allocation{"BAW1252", "1423"};
            EXPECT_CALL(this->api, CreateGeneralSquawkAssignment("BAW1252", "EGKK", "EGPF"))
                .Times(1)
                .WillOnce(Return(allocation));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget);
            EXPECT_TRUE(allocation == this->squawkAllocationHandler->First());
            EXPECT_EQ(1, this->squawkAllocationHandler->Count());
        }

        TEST_F(SquawkGeneratorTest, GeneralSquawkReturnsTrueOnAction)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination()).WillByDefault(Return("EGPF"));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel).WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("7000")).WillRepeatedly(Return());

            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{"BAW1252", "1423"}));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            EXPECT_TRUE(
                this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, LocalSquawkReturnsFalseOnNoActionRequestAlreadyHappening)
        {
            MockTaskRunnerInterface mockRunnerNoExecute(false);
            SquawkGenerator newGenerator(
                this->api,
                &mockRunnerNoExecute,
                *this->assignmentRules,
                this->activeCallsigns,
                this->flightplans,
                this->squawkAllocationHandler);

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetPosition()).WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(1));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_)).WillByDefault(Return(1));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            EXPECT_TRUE(newGenerator.RequestLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
            EXPECT_FALSE(newGenerator.RequestLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, LocalSquawkSetsAircraftSquawks)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetPosition()).WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(1));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_)).WillByDefault(Return(1));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            EXPECT_CALL(*this->mockFlightplan, GetFlightRules()).Times(1).WillOnce(Return("I"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("7000")).WillRepeatedly(Return());

            ApiSquawkAllocation allocation{"BAW1252", "1423"};
            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252")).Times(1).WillOnce(Return(allocation));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            this->generator->RequestLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget);
            EXPECT_TRUE(allocation == this->squawkAllocationHandler->First());
            EXPECT_EQ(1, this->squawkAllocationHandler->Count());
        }

        TEST_F(SquawkGeneratorTest, LocalSquawkCreatesAssignmentIfDoesntExist)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetPosition()).WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(1));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_)).WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, GetFlightRules()).Times(1).WillOnce(Return("I"));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel).WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("7000")).WillRepeatedly(Return());

            ApiSquawkAllocation allocation{"BAW1252", "4521"};
            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252"))
                .Times(1)
                .WillOnce(Throw(ApiNotFoundException("Not Found")));

            EXPECT_CALL(this->api, CreateLocalSquawkAssignment("BAW1252", "EGKK", "I"))
                .Times(1)
                .WillOnce(Return(allocation));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            this->generator->RequestLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget);
            EXPECT_TRUE(allocation == this->squawkAllocationHandler->First());
            EXPECT_EQ(1, this->squawkAllocationHandler->Count());
        }

        TEST_F(SquawkGeneratorTest, LocalSquawkReturnsTrueOnAction)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetPosition()).WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(1));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_)).WillByDefault(Return(1));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            EXPECT_CALL(*this->mockFlightplan, GetFlightRules()).Times(1).WillOnce(Return("I"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("7000")).WillRepeatedly(Return());

            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{"BAW1252", "1423"}));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            EXPECT_TRUE(this->generator->RequestLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, LocalSquawkReturnsFalseOnNoAction)
        {
            this->activeCallsigns.Flush();
            EXPECT_FALSE(this->generator->RequestLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, PreviousAssignmentAssignsPreviousSquawk)
        {
            StoredFlightplan plan("BAW1252", "EGKK", "EGLL");
            plan.SetPreviouslyAssignedSquawk("2342");
            this->flightplans.UpdatePlan(plan);

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel).WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("2342")).Times(1);

            this->generator->ReassignPreviousSquawkToAircraft(*this->mockFlightplan, *this->mockRadarTarget);
        }

        TEST_F(SquawkGeneratorTest, PreviousAssignmentAssignsReturnsTrueOnAction)
        {
            StoredFlightplan plan("BAW1252", "EGKK", "EGLL");
            plan.SetPreviouslyAssignedSquawk("2342");
            this->flightplans.UpdatePlan(plan);

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel).WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("2342")).Times(1);

            EXPECT_TRUE(
                this->generator->ReassignPreviousSquawkToAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, PreviousAssignmentAssignsReturnsFalseOnNoAction)
        {
            this->activeCallsigns.Flush();
            EXPECT_FALSE(
                this->generator->ReassignPreviousSquawkToAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, ForceGeneralSquawkForAircraftReturnsFalseOnNoAction)
        {
            ON_CALL(this->pluginLoopback, GetUserControllerObject()).WillByDefault(Return(nullptr));

            EXPECT_FALSE(this->generator->ForceGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, ForceGeneralSquawkForAircraftAssignsSquawk)
        {
            ON_CALL(this->pluginLoopback, GetUserControllerObject()).WillByDefault(Return(this->mockSelfController));

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination()).WillByDefault(Return("EGPF"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("7000")).WillRepeatedly(Return());

            ApiSquawkAllocation allocation{"BAW1252", "1423"};
            EXPECT_CALL(this->api, CreateGeneralSquawkAssignment("BAW1252", "EGKK", "EGPF"))
                .Times(1)
                .WillOnce(Return(allocation));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            EXPECT_TRUE(this->generator->ForceGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
            EXPECT_TRUE(allocation == this->squawkAllocationHandler->First());
            EXPECT_EQ(1, this->squawkAllocationHandler->Count());
        }

        TEST_F(SquawkGeneratorTest, ForceLocalSquawkForAircraftReturnsFalseOnNoAction)
        {
            ON_CALL(this->pluginLoopback, GetUserControllerObject()).WillByDefault(Return(nullptr));

            EXPECT_FALSE(this->generator->ForceLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, ForceLocalSquawkForAircraftReturnsFalseOnNoUserController)
        {
            ON_CALL(this->pluginLoopback, GetUserControllerObject()).WillByDefault(Return(nullptr));

            ON_CALL(this->pluginLoopback, GetUserControllerObject()).WillByDefault(Return(this->mockSelfController));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            this->activeCallsigns.RemoveCallsign(
                ActiveCallsign("EGKK_APP", "Testy McTestface", *this->controller, true));

            EXPECT_FALSE(this->generator->ForceLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, ForceLocalSquawkAssignsSquawks)
        {
            ON_CALL(this->pluginLoopback, GetUserControllerObject()).WillByDefault(Return(this->mockSelfController));

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetPosition()).WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(1));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_)).WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, GetFlightRules()).Times(1).WillOnce(Return("I"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("7000")).WillRepeatedly(Return());

            ApiSquawkAllocation allocation{"BAW1252", "1423"};
            EXPECT_CALL(this->api, CreateLocalSquawkAssignment("BAW1252", "EGKK", "I"))
                .Times(1)
                .WillOnce(Return(allocation));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            EXPECT_TRUE(this->generator->ForceLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
            EXPECT_TRUE(allocation == this->squawkAllocationHandler->First());
            EXPECT_EQ(1, this->squawkAllocationHandler->Count());
        }

        TEST_F(SquawkGeneratorTest, AssignCircuitSquawkStopsIfAssignmentNotRequired)
        {
            ON_CALL(*this->mockFlightplan, IsVfr()).WillByDefault(Return(false));

            EXPECT_FALSE(
                this->generator->AssignCircuitSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, AssignCircuitSquawkAssignsIfRequired)
        {
            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel).WillByDefault(Return(1));

            ON_CALL(*this->mockFlightplan, IsVfr()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetRawRouteString()).WillByDefault(Return("VFR CIRCUIT"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            EXPECT_TRUE(this->generator->AssignCircuitSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, DeleteApiSquawkAndSetToReturnsFalseOnNoAction)
        {
            this->activeCallsigns.Flush();
            EXPECT_CALL(*this->mockFlightplan, SetSquawk).Times(0);
            EXPECT_FALSE(this->generator->DeleteApiSquawkAndSetTo("7000", *this->mockFlightplan));
        }

        TEST_F(SquawkGeneratorTest, DeleteApiSquawkAndSetToReturnsFalseOnNoActionRequestAlreadyHappening)
        {
            MockTaskRunnerInterface mockRunnerNoExecute(false);
            SquawkGenerator newGenerator(
                this->api,
                &mockRunnerNoExecute,
                *this->assignmentRules,
                this->activeCallsigns,
                this->flightplans,
                this->squawkAllocationHandler);

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));
            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));
            ON_CALL(*this->mockFlightplan, GetAssignedSquawk()).WillByDefault(Return("1234"));
            EXPECT_CALL(*this->mockFlightplan, SetSquawk("7000")).Times(1);

            EXPECT_TRUE(newGenerator.DeleteApiSquawkAndSetTo("7000", *this->mockFlightplan));
            EXPECT_FALSE(newGenerator.DeleteApiSquawkAndSetTo("7000", *this->mockFlightplan));
        }

        TEST_F(SquawkGeneratorTest, DeleteApiSquawkAndSetToDeletesSquawk)
        {
            StoredFlightplan storedPlan("BAW1252", "EGKK", "EGPH");
            storedPlan.SetPreviouslyAssignedSquawk("1234");
            this->flightplans.UpdatePlan(storedPlan);

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));
            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));
            ON_CALL(*this->mockFlightplan, GetAssignedSquawk()).WillByDefault(Return("1234"));
            EXPECT_CALL(*this->mockFlightplan, SetSquawk("7010")).Times(1);
            EXPECT_CALL(this->api, DeleteSquawkAssignment("BAW1252")).Times(1);

            EXPECT_TRUE(this->generator->DeleteApiSquawkAndSetTo("7010", *this->mockFlightplan));
            EXPECT_EQ("7010", this->flightplans.GetFlightplanForCallsign("BAW1252").GetPreviouslyAssignedSquawk());
        }

        TEST_F(SquawkGeneratorTest, DeleteApiSquawkAndSetToDeletesSquawkAndTriggersFailedEventIfDeleteFails)
        {
            StoredFlightplan storedPlan("BAW1252", "EGKK", "EGPH");
            storedPlan.SetPreviouslyAssignedSquawk("1234");
            this->flightplans.UpdatePlan(storedPlan);

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));
            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));
            ON_CALL(*this->mockFlightplan, GetAssignedSquawk()).WillByDefault(Return("1234"));
            EXPECT_CALL(*this->mockFlightplan, SetSquawk("2831")).Times(1);
            EXPECT_CALL(this->api, DeleteSquawkAssignment("BAW1252"))
                .WillOnce(Throw(ApiNotFoundException("Not found")));

            EXPECT_TRUE(this->generator->DeleteApiSquawkAndSetTo("2831", *this->mockFlightplan));
            EXPECT_EQ("2831", this->flightplans.GetFlightplanForCallsign("BAW1252").GetPreviouslyAssignedSquawk());

            AssertSingleEventDispatched();
            AssertFirstEventDispatched<UKControllerPlugin::Squawk::SquawkAssignmentDeleteForConspicuityFailedEvent>(
                [](const auto& event) {
                    EXPECT_EQ("BAW1252", event.callsign);
                    EXPECT_EQ("1234", event.previousSquawk);
                });
        }
    } // namespace Squawk
} // namespace UKControllerPluginTest
