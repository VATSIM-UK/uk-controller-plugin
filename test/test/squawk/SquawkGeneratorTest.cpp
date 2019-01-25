#include "pch/pch.h"
#include "squawk/SquawkGenerator.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockCurlApi.h"
#include "api/ApiInterface.h"
#include "mock/MockTaskRunnerInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"
#include "controller/ActiveCallsignCollection.h"
#include "squawk/SquawkAssignment.h"
#include "airfield/AirfieldOwnershipManager.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ActiveCallsign.h"
#include "mock/MockApiInterface.h"
#include "api/ApiNotFoundException.h"
#include "squawk/ApiSquawkAllocation.h"

using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Squawk::SquawkGenerator;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Squawk::SquawkAssignment;
using UKControllerPlugin::Airfield::AirfieldOwnershipManager;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Api::ApiNotFoundException;
using UKControllerPlugin::Squawk::ApiSquawkAllocation;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::_;
using ::testing::Throw;

namespace UKControllerPluginTest {
    namespace Squawk {

        class SquawkGeneratorTest : public ::testing::Test
        {
            public:
                void SetUp()
                {
                    this->mockFlightplan = std::make_unique<NiceMock<MockEuroScopeCFlightPlanInterface>>();
                    this->mockRadarTarget = std::make_unique<NiceMock<MockEuroScopeCRadarTargetInterface>>();
                    this->airfieldOwnership = std::make_unique<AirfieldOwnershipManager>(
                        this->airfields,
                        this->activeCallsigns
                    );
                    this->assignmentRules = std::make_unique<SquawkAssignment>(
                        this->flightplans,
                        this->pluginLoopback,
                        *this->airfieldOwnership,
                        this->activeCallsigns,
                        false
                    );
                    this->generator = std::make_unique<SquawkGenerator>(
                        this->api,
                        &this->taskRunner,
                        &this->pluginLoopback,
                        *this->assignmentRules,
                        this->activeCallsigns,
                        this->flightplans
                    );

                    this->controller = std::unique_ptr<ControllerPosition>(
                        new ControllerPosition("EGKK_APP", 126.820, "APP", { "EGKK" })
                    );
                    this->activeCallsigns.AddUserCallsign(
                        ActiveCallsign(
                        "EGKK_APP",
                        "Testy McTestface",
                        *this->controller
                        )
                    );
                    this->airfieldOwnership->RefreshOwner("EGKK");
                }

                NiceMock<MockCurlApi> mockCurl;
                NiceMock<MockApiInterface> api;
                std::unique_ptr<SquawkGenerator> generator;
                MockTaskRunnerInterface taskRunner;
                std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplan;
                std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
                NiceMock<MockEuroscopePluginLoopbackInterface> pluginLoopback;
                StoredFlightplanCollection flightplans;
                ActiveCallsignCollection activeCallsigns;
                std::unique_ptr<SquawkAssignment> assignmentRules;
                std::unique_ptr<AirfieldOwnershipManager> airfieldOwnership;
                std::unique_ptr<ControllerPosition> controller;
                AirfieldCollection airfields;
        };

        TEST_F(SquawkGeneratorTest, GeneralSquawkReturnsFalseOnNoAction)
        {
            this->activeCallsigns.Flush();
            EXPECT_FALSE(
                this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkGeneratorTest, GeneralSquawkReturnsFalseOnNoActionRequestAlreadyHappening)
        {
            MockTaskRunnerInterface mockRunnerNoExecute(false);
            SquawkGenerator newGenerator(
                this->api,
                &mockRunnerNoExecute,
                &this->pluginLoopback,
                *this->assignmentRules,
                this->activeCallsigns,
                this->flightplans
            );

            ON_CALL(*this->mockFlightplan, SetSquawk(this->generator->PROCESS_SQUAWK))
                .WillByDefault(Return());

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            EXPECT_TRUE(newGenerator.RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
            EXPECT_FALSE(newGenerator.RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, GeneralSquawkSetsAircraftSquawks)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGPF"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk(this->generator->PROCESS_SQUAWK))
                .WillRepeatedly(Return());

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("1423"))
                .Times(1);

            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{ "BAW1252", "1423" }));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget);
        }

        TEST_F(SquawkGeneratorTest, GeneralSquawkCreatesAssignmentIfNoneExists)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGPF"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk(this->generator->PROCESS_SQUAWK))
                .WillRepeatedly(Return());

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("4521"))
                .Times(1);

            EXPECT_CALL(this->api, CreateGeneralSquawkAssignment("BAW1252", "EGKK", "EGPF"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{ "BAW1252", "4521" }));

            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252"))
                .Times(1)
                .WillOnce(Throw(ApiNotFoundException("Not found")));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget);
        }

        TEST_F(SquawkGeneratorTest, GeneralSquawkForcesSquawkWhereNecessary)
        {
            StoredFlightplan storedPlan("BAW1252", "EGKK", "EGPH");
            this->flightplans.UpdatePlan(storedPlan);

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGPF"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("1423"))
                .Times(1);

            EXPECT_CALL(*this->mockFlightplan, SetSquawk(this->generator->PROCESS_SQUAWK))
                .WillRepeatedly(Return());

            EXPECT_CALL(this->api, CreateGeneralSquawkAssignment("BAW1252", "EGKK", "EGPF"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{ "BAW1252", "1423" }));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget);
        }

        TEST_F(SquawkGeneratorTest, GeneralSquawkReturnsTrueOnAction)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGPF"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk(this->generator->PROCESS_SQUAWK))
                .WillRepeatedly(Return());

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("1423"))
                .Times(1);

            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{ "BAW1252", "1423" }));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            EXPECT_TRUE(
                this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkGeneratorTest, AnotherRequestCanBeMadeAfterFirstFinishes)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGPF"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk(this->generator->PROCESS_SQUAWK))
                .WillRepeatedly(Return());

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("1423"))
                .Times(2);

            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252"))
                .Times(2)
                .WillRepeatedly(Return(ApiSquawkAllocation{ "BAW1252", "1423" }));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            EXPECT_TRUE(
                this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget)
            );
            EXPECT_TRUE(
                this->generator->RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkGeneratorTest, LocalSquawkReturnsFalseOnNoActionRequestAlreadyHappening)
        {
            MockTaskRunnerInterface mockRunnerNoExecute(false);
            SquawkGenerator newGenerator(
                this->api,
                &mockRunnerNoExecute,
                &this->pluginLoopback,
                *this->assignmentRules,
                this->activeCallsigns,
                this->flightplans
            );

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetPosition())
                .WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel())
                .WillByDefault(Return(1));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_))
                .WillByDefault(Return(1));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            EXPECT_TRUE(newGenerator.RequestLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
            EXPECT_FALSE(newGenerator.RequestLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, LocalSquawkSetsAircraftSquawks)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetPosition())
                .WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel())
                .WillByDefault(Return(1));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_))
                .WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, GetFlightRules())
                .Times(1)
                .WillOnce(Return("I"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk(this->generator->PROCESS_SQUAWK))
                .WillRepeatedly(Return());

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("1423"))
                .Times(1);

            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{ "BAW1252", "1423" }));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            this->generator->RequestLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget);
        }

        TEST_F(SquawkGeneratorTest, LocalSquawkCreatesAssignmentIfDoesntExist)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetPosition())
                .WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel())
                .WillByDefault(Return(1));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_))
                .WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, GetFlightRules())
                .Times(1)
                .WillOnce(Return("I"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk(this->generator->PROCESS_SQUAWK))
                .WillRepeatedly(Return());

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("4521"))
                .Times(1);

            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252"))
                .Times(1)
                .WillOnce(Throw(ApiNotFoundException("Not Found")));

            EXPECT_CALL(this->api, CreateLocalSquawkAssignment("BAW1252", "EGKK", "I"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{ "BAW1252", "4521" }));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            this->generator->RequestLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget);
        }

        TEST_F(SquawkGeneratorTest, LocalSquawkReturnsTrueOnAction)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetPosition())
                .WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel())
                .WillByDefault(Return(1));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_))
                .WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, GetFlightRules())
                .Times(1)
                .WillOnce(Return("I"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk(this->generator->PROCESS_SQUAWK))
                .WillRepeatedly(Return());

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("1423"))
                .Times(1);

            EXPECT_CALL(this->api, GetAssignedSquawk("BAW1252"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{ "BAW1252", "1423" }));

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

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("2342"))
                .Times(1);

            this->generator->ReassignPreviousSquawkToAircraft(*this->mockFlightplan, *this->mockRadarTarget);
        }

        TEST_F(SquawkGeneratorTest, PreviousAssignmentAssignsReturnsTrueOnAction)
        {
            StoredFlightplan plan("BAW1252", "EGKK", "EGLL");
            plan.SetPreviouslyAssignedSquawk("2342");
            this->flightplans.UpdatePlan(plan);

            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));
            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("2342"))
                .Times(1);

            EXPECT_TRUE(
                this->generator->ReassignPreviousSquawkToAircraft(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkGeneratorTest, PreviousAssignmentAssignsReturnsFalseOnNoAction)
        {
            this->activeCallsigns.Flush();
            EXPECT_FALSE(
                this->generator->ReassignPreviousSquawkToAircraft(*this->mockFlightplan, *this->mockRadarTarget)
            );
        }

        TEST_F(SquawkGeneratorTest, ForceGeneralSquawkForAircraftReturnsFalseOnNoAction)
        {
            this->activeCallsigns.Flush();
            EXPECT_FALSE(this->generator->ForceGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, ForceGeneralSquawkForAircraftAssignsSquawk)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(*this->mockFlightplan, GetDestination())
                .WillByDefault(Return("EGPF"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk(this->generator->PROCESS_SQUAWK))
                .WillRepeatedly(Return());

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("1423"))
                .Times(1);

            EXPECT_CALL(this->api, CreateGeneralSquawkAssignment("BAW1252", "EGKK", "EGPF"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{ "BAW1252", "1423" }));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            EXPECT_TRUE(this->generator->ForceGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, ForceLocalSquawkForAircraftReturnsFalseOnNoAction)
        {
            this->activeCallsigns.Flush();
            EXPECT_FALSE(this->generator->ForceLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, ForceLocalSquawkAssignsSquawks)
        {
            ON_CALL(*this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk())
                .WillByDefault(Return(false));

            ON_CALL(*this->mockRadarTarget, GetPosition())
                .WillByDefault(Return(EuroScopePlugIn::CPosition()));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel())
                .WillByDefault(Return(1));

            ON_CALL(this->pluginLoopback, GetDistanceFromUserVisibilityCentre(_))
                .WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, GetFlightRules())
                .Times(1)
                .WillOnce(Return("I"));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk(this->generator->PROCESS_SQUAWK))
                .WillRepeatedly(Return());

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("1423"))
                .Times(1);

            EXPECT_CALL(this->api, CreateLocalSquawkAssignment("BAW1252", "EGKK", "I"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{ "BAW1252", "1423" }));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                .WillByDefault(Return(this->mockFlightplan));

            EXPECT_TRUE(this->generator->ForceLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, ReassignPreviousSquawkStopsIfSquawksDisabled)
        {
            SquawkAssignment disabledRules(
                this->flightplans,
                this->pluginLoopback,
                *this->airfieldOwnership,
                this->activeCallsigns,
                true
            );
            SquawkGenerator newGenerator(
                this->api,
                &this->taskRunner,
                &this->pluginLoopback,
                disabledRules,
                this->activeCallsigns,
                this->flightplans
            );

            EXPECT_FALSE(newGenerator.ReassignPreviousSquawkToAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, ForceGeneralSquawkForAircraftStopsIfSquawksDisabled)
        {
            SquawkAssignment disabledRules(
                this->flightplans,
                this->pluginLoopback,
                *this->airfieldOwnership,
                this->activeCallsigns,
                true
            );
            SquawkGenerator newGenerator(
                this->api,
                &this->taskRunner,
                &this->pluginLoopback,
                disabledRules,
                this->activeCallsigns,
                this->flightplans
            );

            EXPECT_FALSE(newGenerator.ForceGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, ForceLocalSquawkForAircraftStopsIfSquawksDisabled)
        {
            SquawkAssignment disabledRules(
                this->flightplans,
                this->pluginLoopback,
                *this->airfieldOwnership,
                this->activeCallsigns,
                true
            );
            SquawkGenerator newGenerator(
                this->api,
                &this->taskRunner,
                &this->pluginLoopback,
                disabledRules,
                this->activeCallsigns,
                this->flightplans
            );

            EXPECT_FALSE(newGenerator.ForceLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, RequestGeneralSquawkForAircraftStopsIfSquawksDisabled)
        {
            SquawkAssignment disabledRules(
                this->flightplans,
                this->pluginLoopback,
                *this->airfieldOwnership,
                this->activeCallsigns,
                true
            );
            SquawkGenerator newGenerator(
                this->api,
                &this->taskRunner,
                &this->pluginLoopback,
                disabledRules,
                this->activeCallsigns,
                this->flightplans
            );

            EXPECT_FALSE(newGenerator.RequestGeneralSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }

        TEST_F(SquawkGeneratorTest, RequestLocalSquawkForAircraftStopsIfSquawksDisabled)
        {
            SquawkAssignment disabledRules(
                this->flightplans,
                this->pluginLoopback,
                *this->airfieldOwnership,
                this->activeCallsigns,
                true
            );
            SquawkGenerator newGenerator(
                this->api,
                &this->taskRunner,
                &this->pluginLoopback,
                disabledRules,
                this->activeCallsigns,
                this->flightplans
            );

            EXPECT_FALSE(newGenerator.RequestLocalSquawkForAircraft(*this->mockFlightplan, *this->mockRadarTarget));
        }
    }  // namespace Squawk
}  // namespace UKControllerPluginTest
