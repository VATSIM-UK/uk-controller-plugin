#include "squawk/SquawkEventHandler.h"
#include "squawk/SquawkGenerator.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"
#include "controller/ControllerPosition.h"
#include "api/ApiRequestBuilder.h"
#include "squawk/SquawkAssignment.h"
#include "login/Login.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "squawk/ApiSquawkAllocationHandler.h"

using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPlugin::Squawk::ApiSquawkAllocation;
using UKControllerPlugin::Squawk::ApiSquawkAllocationHandler;
using UKControllerPlugin::Squawk::SquawkAssignment;
using UKControllerPlugin::Squawk::SquawkEventHandler;
using UKControllerPlugin::Squawk::SquawkGenerator;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPluginTest::Windows::MockWinApi;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Test;
using ::testing::Throw;

namespace UKControllerPluginTest {
    namespace Squawk {

        class SquawkEventHandlerTest : public Test
        {
            public:
            SquawkEventHandlerTest()
                : login(this->pluginLoopback, ControllerStatusEventHandlerCollection()),
                  apiSquawkAllocations(new ApiSquawkAllocationHandler(this->pluginLoopback)),
                  generator(
                      this->mockApi,
                      &this->taskRunner,
                      this->assignmentRules,
                      this->activeCallsigns,
                      this->plans,
                      this->apiSquawkAllocations),
                  assignmentRules(this->plans, this->pluginLoopback, this->airfieldOwnership, this->activeCallsigns),
                  controller(1, "EGKK_APP", 126.820, {"EGKK"}, true, false),
                  userCallsign("EGKK_APP", "Testy McTestface", this->controller, true),
                  notUserCallsign("EGKK_APP", "Testy McTestface", this->controller, false),
                  handler(this->generator, this->activeCallsigns, this->plans, this->pluginLoopback, this->login, false)

            {
            }

            void SetUp()
            {
                this->mockFlightplan.reset(new NiceMock<MockEuroScopeCFlightPlanInterface>);
                this->mockRadarTarget.reset(new NiceMock<MockEuroScopeCRadarTargetInterface>);
                this->mockSelfController.reset(new NiceMock<MockEuroScopeCControllerInterface>);

                ON_CALL(*this->mockSelfController, IsVatsimRecognisedController()).WillByDefault(Return(true));

                this->activeCallsigns.AddUserCallsign(this->userCallsign);
                this->airfieldOwnership.SetProvidersForAirfield(
                    "EGKK",
                    std::vector<std::shared_ptr<ServiceProvision>>{std::make_shared<ServiceProvision>(
                        ServiceType::Delivery,
                        std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>(this->userCallsign))});

                // By default, lets assume we've been logged in for a while.
                this->login.SetLoginTime(std::chrono::system_clock::now() - std::chrono::minutes(5));
            }

            void expectGeneralAssignment()
            {
                ON_CALL(*mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

                ON_CALL(*mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

                ON_CALL(*mockFlightplan, GetDestination()).WillByDefault(Return("EGPF"));

                ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

                ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

                ON_CALL(*this->mockFlightplan, IsTracked()).WillByDefault(Return(true));

                ON_CALL(*this->mockRadarTarget, GetFlightLevel())
                    .WillByDefault(Return(this->assignmentRules.maxAssignmentAltitude + 1));

                ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

                EXPECT_CALL(*mockFlightplan, SetSquawk("7000")).Times(1);

                ApiSquawkAllocation allocation{"BAW1252", "1423"};
                EXPECT_CALL(this->mockApi, GetAssignedSquawk("BAW1252")).Times(1).WillOnce(Return(allocation));

                ON_CALL(this->pluginLoopback, GetRadarTargetForCallsign("BAW1252"))
                    .WillByDefault(Return(this->mockRadarTarget));

                ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252"))
                    .WillByDefault(Return(mockFlightplan));
            }

            void expectLocalAssignment(void)
            {
                ON_CALL(*mockFlightplan, GetCallsign()).WillByDefault(Return("GATWF"));

                ON_CALL(*mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

                ON_CALL(*mockFlightplan, GetDestination()).WillByDefault(Return("EGPF"));

                ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

                ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

                ON_CALL(*mockFlightplan, GetFlightRules()).WillByDefault(Return("V"));

                EXPECT_CALL(*mockFlightplan, SetSquawk("7000")).Times(1);

                ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

                ON_CALL(*this->mockRadarTarget, GetFlightLevel).WillByDefault(Return(1));

                ApiSquawkAllocation allocation{"BAW1252", "7261"};
                EXPECT_CALL(this->mockApi, GetAssignedSquawk("GATWF")).Times(1).WillOnce(Return(allocation));

                ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("GATWF")).WillByDefault(Return(mockFlightplan));
            }

            void ExpectNoAssignment()
            {
                EXPECT_CALL(*this->mockFlightplan, SetSquawk(_)).Times(0);
            }

            void AssertGeneralAssignment()
            {
                ApiSquawkAllocation allocation{"BAW1252", "1423"};
                EXPECT_EQ(1, this->apiSquawkAllocations->Count());
                EXPECT_TRUE(allocation == this->apiSquawkAllocations->First());
            }

            void AssertLocalAssignment()
            {
                ApiSquawkAllocation allocation{"BAW1252", "7261"};
                EXPECT_EQ(1, this->apiSquawkAllocations->Count());
                EXPECT_TRUE(allocation == this->apiSquawkAllocations->First());
            }

            Login login;
            NiceMock<MockEuroscopePluginLoopbackInterface> pluginLoopback;
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplan;
            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
            std::shared_ptr<NiceMock<MockEuroScopeCControllerInterface>> mockSelfController;
            std::shared_ptr<ApiSquawkAllocationHandler> apiSquawkAllocations;
            NiceMock<MockApiInterface> mockApi;
            NiceMock<MockWinApi> mockWinApi;
            NiceMock<MockTaskRunnerInterface> taskRunner;
            StoredFlightplanCollection plans;
            SquawkGenerator generator;
            SquawkAssignment assignmentRules;
            ActiveCallsignCollection activeCallsigns;
            ControllerPosition controller;
            ActiveCallsign userCallsign;
            ActiveCallsign notUserCallsign;
            AirfieldServiceProviderCollection airfieldOwnership;
            SquawkEventHandler handler;
        };

        TEST_F(SquawkEventHandlerTest, ItDefaultsToUserSquawksOn)
        {
            EXPECT_TRUE(this->handler.UserAllowedSquawkAssignment());
        }

        TEST_F(SquawkEventHandlerTest, ItUpdatesBasedOnUserSettings)
        {
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            EXPECT_CALL(userSettingProvider, GetKey(GeneralSettingsEntries::squawkToggleSettingsKey))
                .Times(1)
                .WillOnce(Return("0"));

            this->handler.UserSettingsUpdated(userSetting);
            EXPECT_FALSE(this->handler.UserAllowedSquawkAssignment());
        }

        TEST_F(SquawkEventHandlerTest, ItDefaultsToTrueIfNoUserSetting)
        {
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            EXPECT_CALL(userSettingProvider, GetKey(GeneralSettingsEntries::squawkToggleSettingsKey))
                .Times(1)
                .WillOnce(Return("0"));

            NiceMock<MockUserSettingProviderInterface> userSettingProviderNoValue;
            EXPECT_CALL(userSettingProviderNoValue, GetKey(GeneralSettingsEntries::squawkToggleSettingsKey))
                .Times(1)
                .WillOnce(Return(""));

            UserSetting userSettingNoValue(userSettingProviderNoValue);

            this->handler.UserSettingsUpdated(userSetting);
            this->handler.UserSettingsUpdated(userSettingNoValue);
            EXPECT_TRUE(this->handler.UserAllowedSquawkAssignment());
        }

        TEST_F(SquawkEventHandlerTest, FlightplanEventDoesNothingIfUserToggleNotOn)
        {
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            EXPECT_CALL(userSettingProvider, GetKey(GeneralSettingsEntries::squawkToggleSettingsKey))
                .Times(1)
                .WillOnce(Return("0"));

            this->handler.UserSettingsUpdated(userSetting);

            StrictMock<MockEuroScopeCFlightPlanInterface> mockFp;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRt;
            EXPECT_NO_THROW(this->handler.FlightPlanEvent(mockFp, mockRt));
        }

        TEST_F(SquawkEventHandlerTest, FlightplanEventDefersIfNotLoggedInLongEnough)
        {
            EXPECT_CALL(*this->mockFlightplan, GetCallsign()).Times(1).WillOnce(Return("BAW123"));

            this->login.SetLoginTime(std::chrono::system_clock::now());
            this->handler.FlightPlanEvent(*this->mockFlightplan, *this->mockRadarTarget);
        }

        TEST_F(SquawkEventHandlerTest, FlightplanEventReassignsOldSquawk)
        {
            StoredFlightplan plan("BAW123", "EGKK", "EGPF");
            plan.SetPreviouslyAssignedSquawk("3421");
            this->plans.UpdatePlan(plan);

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel).WillByDefault(Return(1));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk("3421")).Times(1);

            this->handler.FlightPlanEvent(*this->mockFlightplan, *this->mockRadarTarget);
        }

        TEST_F(SquawkEventHandlerTest, FlightplanEventAssignsLocalSquawk)
        {
            this->expectLocalAssignment();
            this->handler.FlightPlanEvent(*this->mockFlightplan, *this->mockRadarTarget);
            this->AssertLocalAssignment();
        }

        TEST_F(SquawkEventHandlerTest, FlightplanEventAssignsGeneralSquawk)
        {
            this->expectGeneralAssignment();
            this->handler.FlightPlanEvent(*this->mockFlightplan, *this->mockRadarTarget);
            this->AssertGeneralAssignment();
        }

        TEST_F(SquawkEventHandlerTest, FlightplanControllerDataUpdateSetsPreviousSquawkIfDataTypeSquawk)
        {
            StoredFlightplan plan("BAW1252", "EGKK", "EGPF");
            plan.SetPreviouslyAssignedSquawk("3421");
            this->plans.UpdatePlan(plan);

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetAssignedSquawk()).WillByDefault(Return("1426"));

            this->handler.ControllerFlightPlanDataEvent(*this->mockFlightplan, EuroScopePlugIn::CTR_DATA_TYPE_SQUAWK);
            EXPECT_TRUE(this->plans.GetFlightplanForCallsign("BAW1252").GetPreviouslyAssignedSquawk() == "1426");
        }

        TEST_F(SquawkEventHandlerTest, FlightplanControllerDataUpdateDoesNothingIfDataTypeNotSquawk)
        {
            StoredFlightplan plan("BAW1252", "EGKK", "EGPF");
            plan.SetPreviouslyAssignedSquawk("3413");
            this->plans.UpdatePlan(plan);

            ON_CALL(*this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk()).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, GetAssignedSquawk()).WillByDefault(Return("1426"));

            this->handler.ControllerFlightPlanDataEvent(
                *this->mockFlightplan, EuroScopePlugIn::CTR_DATA_TYPE_DEPARTURE_SEQUENCE);
            EXPECT_FALSE(this->plans.GetFlightplanForCallsign("BAW1252").GetPreviouslyAssignedSquawk() == "1426");
        }

        TEST_F(SquawkEventHandlerTest, SquawkReycleGeneralForcesSquawkReset)
        {
            ON_CALL(this->pluginLoopback, GetUserControllerObject()).WillByDefault(Return(this->mockSelfController));

            ON_CALL(*mockFlightplan, GetCallsign()).WillByDefault(Return("BAW1252"));

            ON_CALL(*mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(*mockFlightplan, GetDestination()).WillByDefault(Return("EGPF"));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("BAW1252")).WillByDefault(Return(mockFlightplan));

            EXPECT_CALL(*mockFlightplan, SetSquawk("7000")).Times(1);

            EXPECT_CALL(this->mockApi, CreateGeneralSquawkAssignment("BAW1252", "EGKK", "EGPF"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{"BAW1252", "1423"}));

            this->handler.SquawkReycleGeneral(*this->mockFlightplan, *this->mockRadarTarget, "", POINT());
            this->AssertGeneralAssignment();
        }

        TEST_F(SquawkEventHandlerTest, SquawkReycleLocalForcesSquawkReset)
        {
            ON_CALL(this->pluginLoopback, GetUserControllerObject()).WillByDefault(Return(this->mockSelfController));

            ON_CALL(*mockFlightplan, GetCallsign()).WillByDefault(Return("GATWF"));

            ON_CALL(*mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(*mockFlightplan, GetDestination()).WillByDefault(Return("EGPF"));

            ON_CALL(*mockFlightplan, GetFlightRules()).WillByDefault(Return("V"));

            ON_CALL(this->pluginLoopback, GetFlightplanForCallsign("GATWF")).WillByDefault(Return(mockFlightplan));

            EXPECT_CALL(*mockFlightplan, SetSquawk("7000")).Times(1);

            EXPECT_CALL(this->mockApi, CreateLocalSquawkAssignment("GATWF", "EGKK", "V"))
                .Times(1)
                .WillOnce(Return(ApiSquawkAllocation{"BAW1252", "7261"}));

            this->handler.SquawkRecycleLocal(*this->mockFlightplan, *this->mockRadarTarget, "", POINT());
            this->AssertLocalAssignment();
        }

        TEST_F(SquawkEventHandlerTest, TimedEventTriggerDoesNothingIfUserNotActive)
        {
            this->pluginLoopback.ExpectNoFlightplanLoop();
            SquawkEventHandler handler(
                this->generator, ActiveCallsignCollection(), this->plans, this->pluginLoopback, this->login, true);
            EXPECT_NO_THROW(handler.TimedEventTrigger());
        }

        TEST_F(SquawkEventHandlerTest, TimedEventTriggerDoesNothingIfAutoAssignDisabled)
        {
            this->pluginLoopback.ExpectNoFlightplanLoop();
            SquawkEventHandler handler(
                this->generator, this->activeCallsigns, this->plans, this->pluginLoopback, this->login, true);
            EXPECT_NO_THROW(handler.TimedEventTrigger());
        }

        TEST_F(SquawkEventHandlerTest, TimedEventTriggerDoesNothingIfUserToggleOff)
        {
            NiceMock<MockUserSettingProviderInterface> userSettingProvider;
            UserSetting userSetting(userSettingProvider);

            EXPECT_CALL(userSettingProvider, GetKey(GeneralSettingsEntries::squawkToggleSettingsKey))
                .Times(1)
                .WillOnce(Return("0"));

            this->pluginLoopback.ExpectNoFlightplanLoop();

            this->handler.UserSettingsUpdated(userSetting);
            EXPECT_NO_THROW(handler.TimedEventTrigger());
        }

        TEST_F(SquawkEventHandlerTest, TimedEventTriggerDoesNothingIfFlightplanHasAssignedSquawk)
        {
            ON_CALL(*this->mockFlightplan, HasAssignedSquawk).WillByDefault(Return(true));

            this->pluginLoopback.AddAllFlightplansItem({this->mockFlightplan, this->mockRadarTarget});

            EXPECT_NO_THROW(handler.TimedEventTrigger());
        }

        TEST_F(SquawkEventHandlerTest, TimedEventTriggerDoesNothingIfAircraftTrackedByAnotherController)
        {
            ON_CALL(*this->mockFlightplan, HasAssignedSquawk).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTracked).WillByDefault(Return(true));

            this->pluginLoopback.AddAllFlightplansItem({this->mockFlightplan, this->mockRadarTarget});

            EXPECT_NO_THROW(handler.TimedEventTrigger());
        }

        TEST_F(SquawkEventHandlerTest, TimedEventTriggerDoesSquawkAssignmentForTrackedByUserAircraft)
        {
            this->pluginLoopback.AddAllFlightplansItem({this->mockFlightplan, this->mockRadarTarget});

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser).WillByDefault(Return(true));

            ON_CALL(*this->mockFlightplan, IsTracked).WillByDefault(Return(true));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(999999));

            this->expectGeneralAssignment();
            handler.TimedEventTrigger();
            this->AssertGeneralAssignment();
        }

        TEST_F(SquawkEventHandlerTest, TimedEventTriggerDoesSquawkAssignmentForUntrackedAircraft)
        {
            this->pluginLoopback.AddAllFlightplansItem({this->mockFlightplan, this->mockRadarTarget});

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTracked).WillByDefault(Return(true));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(999999));

            this->expectGeneralAssignment();
            handler.TimedEventTrigger();
            this->AssertGeneralAssignment();
        }

        TEST_F(SquawkEventHandlerTest, ActiveCallsignAddedAssignsAllSquawksWhenUserLogsOn)
        {
            this->pluginLoopback.AddAllFlightplansItem({this->mockFlightplan, this->mockRadarTarget});

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser).WillByDefault(Return(true));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(999999));

            ON_CALL(*this->mockFlightplan, GetDistanceFromOrigin).WillByDefault(Return(1.0));

            EXPECT_CALL(*this->mockFlightplan, SetSquawk(testing::_)).Times(0);

            this->expectGeneralAssignment();
            handler.ActiveCallsignAdded(this->userCallsign);
            this->AssertGeneralAssignment();
        }

        TEST_F(SquawkEventHandlerTest, ActiveCallsignAddedDoesntAssignSquawkIfNotUserLogsOn)
        {
            this->pluginLoopback.AddAllFlightplansItem({this->mockFlightplan, this->mockRadarTarget});

            ON_CALL(*this->mockFlightplan, HasAssignedSquawk).WillByDefault(Return(false));

            ON_CALL(*this->mockFlightplan, IsTrackedByUser).WillByDefault(Return(true));

            ON_CALL(*this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(999999));

            this->ExpectNoAssignment();

            handler.ActiveCallsignAdded(this->userCallsign);
        }
    } // namespace Squawk
} // namespace UKControllerPluginTest
