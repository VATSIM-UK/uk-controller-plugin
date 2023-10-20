#include "initialheading/InitialHeadingEventHandler.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "login/Login.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "sid/StandardInstrumentDeparture.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::InitialHeading::InitialHeadingEventHandler;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Test;
using ::testing::Throw;

namespace UKControllerPluginTest {
    namespace InitialHeading {

        class InitialHeadingEventHandlerTest : public Test
        {
            public:
            InitialHeadingEventHandlerTest()
                : controller(1, "LON_S_CTR", 129.420, {"EGKK"}, true, false),
                  userCallsign("LON_S_CTR", "Test", controller, true),
                  notUserCallsign("LON_S_CTR", "Test", controller, false),
                  login(plugin, ControllerStatusEventHandlerCollection()),
                  handler(sidMapper, callsigns, owners, login, plugin)
            {
            }

            virtual void SetUp()
            {
                // Pretend we've been logged in a while
                login.SetLoginStatus(EuroScopePlugIn::CONNECTION_TYPE_DIRECT);
                login.SetLoginTime(std::chrono::system_clock::now() - std::chrono::minutes(15));
                sid1 = std::make_shared<StandardInstrumentDeparture>(1, 2, "ADMAG2X", 6000, 125, 1);
                sid2 = std::make_shared<StandardInstrumentDeparture>(2, 3, "CLN3X", 5000, 200, 1);
                sid3 = std::make_shared<StandardInstrumentDeparture>(3, 4, "DAGGA1X", 500, 0, 1);

                this->mockFlightplanPointer.reset(new NiceMock<MockEuroScopeCFlightPlanInterface>);
                this->mockRadarTargetPointer.reset(new NiceMock<MockEuroScopeCRadarTargetInterface>);
                ON_CALL(mockFlightPlan, GetCallsign()).WillByDefault(Return("BAW123"));

                ON_CALL(*mockFlightplanPointer, GetCallsign()).WillByDefault(Return("BAW123"));
                ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightPlan)))
                    .WillByDefault(testing::Return(sid1));
            }

            void SetServiceProvision(bool isUser)
            {
                auto callsign = isUser ? this->userCallsign : this->notUserCallsign;
                this->owners.SetProvidersForAirfield(
                    "EGKK",
                    std::vector<std::shared_ptr<ServiceProvision>>{std::make_shared<ServiceProvision>(
                        ServiceType::Delivery,
                        std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>(callsign))});
            }

            inline static const double MAX_DISTANCE_FROM_ORIGIN = 3.0;
            inline static const int MAX_ASSIGNMENT_ALTITUDE = 1000;
            inline static const int MAX_ASSIGNMENT_SPEED = 40;
            ControllerPosition controller;
            ActiveCallsign userCallsign;
            ActiveCallsign notUserCallsign;
            NiceMock<Sid::MockSidMapperInterface> sidMapper;
            std::shared_ptr<StandardInstrumentDeparture> sid1;
            std::shared_ptr<StandardInstrumentDeparture> sid2;
            std::shared_ptr<StandardInstrumentDeparture> sid3;
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplanPointer;
            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> mockRadarTargetPointer;
            NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightPlan;
            NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
            NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
            Login login;
            ActiveCallsignCollection callsigns;
            AirfieldServiceProviderCollection owners;
            InitialHeadingEventHandler handler;
        };

        TEST_F(InitialHeadingEventHandlerTest, TestItDefaultsUserSettingToEnabled)
        {
            EXPECT_TRUE(this->handler.UserAutomaticAssignmentsAllowed());
        }

        TEST_F(InitialHeadingEventHandlerTest, TestItSetsEnabledFromUserSettings)
        {
            NiceMock<MockUserSettingProviderInterface> mockUserSettings;
            UserSetting userSetting(mockUserSettings);

            ON_CALL(mockUserSettings, GetKey(GeneralSettingsEntries::initialHeadingToggleSettingsKey))
                .WillByDefault(Return("0"));

            this->handler.UserSettingsUpdated(userSetting);
            EXPECT_FALSE(this->handler.UserAutomaticAssignmentsAllowed());
        }

        TEST_F(InitialHeadingEventHandlerTest, TestItDefaultsToEnabledIfNoSettingPresent)
        {
            NiceMock<MockUserSettingProviderInterface> mockUserSettings;
            UserSetting userSetting(mockUserSettings);

            ON_CALL(mockUserSettings, GetKey(GeneralSettingsEntries::initialHeadingToggleSettingsKey))
                .WillByDefault(Return("0"));

            NiceMock<MockUserSettingProviderInterface> mockUserSettingsNoSetting;
            UserSetting userSettingNoSetting(mockUserSettingsNoSetting);

            ON_CALL(mockUserSettingsNoSetting, GetKey(GeneralSettingsEntries::initialHeadingToggleSettingsKey))
                .WillByDefault(Return(""));

            this->handler.UserSettingsUpdated(userSetting);
            EXPECT_FALSE(this->handler.UserAutomaticAssignmentsAllowed());
            this->handler.UserSettingsUpdated(userSettingNoSetting);
            EXPECT_TRUE(this->handler.UserAutomaticAssignmentsAllowed());
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNothingIfUserSettingDisabled)
        {
            NiceMock<MockUserSettingProviderInterface> mockUserSettings;
            UserSetting userSetting(mockUserSettings);

            ON_CALL(mockUserSettings, GetKey(GeneralSettingsEntries::initialHeadingToggleSettingsKey))
                .WillByDefault(Return("0"));

            this->handler.UserSettingsUpdated(userSetting);

            StrictMock<MockEuroScopeCFlightPlanInterface> mockFp;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRt;
            EXPECT_NO_THROW(this->handler.FlightPlanEvent(mockFp, mockRt));
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNothingOnProxies)
        {
            this->login.SetLoginStatus(EuroScopePlugIn::CONNECTION_TYPE_VIA_PROXY);

            StrictMock<MockEuroScopeCFlightPlanInterface> mockFp;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRt;
            EXPECT_NO_THROW(this->handler.FlightPlanEvent(mockFp, mockRt));
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDefersIfNotLoggedInLongEnough)
        {
            EXPECT_CALL(this->mockFlightPlan, GetCallsign).Times(1).WillOnce(Return("BAW123"));

            EXPECT_CALL(this->mockRadarTarget, GetFlightLevel).Times(0);

            login.SetLoginTime(std::chrono::system_clock::now() + std::chrono::minutes(15));
            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfTooHigh)
        {
            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE - 1));
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(1)
                .WillOnce(Return(MAX_DISTANCE_FROM_ORIGIN - 1));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE + 1));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfExactlyAtSeaLevel)
        {
            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(0));
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin()).Times(0);

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfTooFarFromOrigin)
        {
            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(MAX_DISTANCE_FROM_ORIGIN + 1));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfExactlyOnOrigin)
        {
            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin()).Times(1).WillOnce(Return(0));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfTravellingTooFast)
        {
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED + 1));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfAlreadyHasAnAssignedHeading)
        {
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin()).Times(2).WillOnce(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(1).WillOnce(Return(true));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfTracked)
        {
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked()).Times(1).WillOnce(Return(true));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfAircraftIsSimulated)
        {
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin()).Times(2).WillOnce(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated()).Times(1).WillOnce(Return(true));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfSidHasNoInitialHeading)
        {
            this->SetServiceProvision(true);

            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightPlan)))
                .Times(1)
                .WillOnce(testing::Return(sid3));

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin()).Times(2).WillOnce(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated()).Times(1).WillOnce(Return(false));

            ON_CALL(mockFlightPlan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(mockFlightPlan, GetSidName()).WillByDefault(Return("DAGGA1X"));

            EXPECT_CALL(mockFlightPlan, SetHeading(_)).Times(0);

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfUserCallsignIsNotActive)
        {
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, GetOrigin()).Times(1).WillOnce(Return("EGKK"));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfAirfieldIsNotOwnedByUser)
        {
            callsigns.AddUserCallsign(userCallsign);

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin()).Times(2).WillOnce(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, GetOrigin()).Times(1).WillOnce(Return("EGKK"));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfSidNotFound)
        {
            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightPlan)))
                .Times(1)
                .WillOnce(testing::Return(nullptr));

            callsigns.AddUserCallsign(userCallsign);

            this->SetServiceProvision(true);

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, GetOrigin()).Times(1).WillOnce(Return("EGKK"));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfAlreadyAssignedOnSameSid)
        {
            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightPlan)))
                .Times(1)
                .WillRepeatedly(testing::Return(sid1));

            callsigns.AddUserCallsign(userCallsign);

            this->SetServiceProvision(true);

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, GetSidName()).Times(2).WillRepeatedly(Return("ADMAG2X"));

            EXPECT_CALL(mockFlightPlan, GetOrigin()).Times(2).WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(mockFlightPlan, GetCallsign()).Times(5).WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(mockFlightPlan, SetHeading(125)).Times(1);

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventAssignsIfSidFound)
        {
            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightPlan)))
                .Times(1)
                .WillOnce(testing::Return(sid1));

            callsigns.AddUserCallsign(userCallsign);

            this->SetServiceProvision(true);

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, GetSidName()).Times(1).WillOnce(Return("ADMAG2X"));

            EXPECT_CALL(mockFlightPlan, GetOrigin()).Times(2).WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(mockFlightPlan, GetCallsign()).Times(3).WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(mockFlightPlan, SetHeading(125)).Times(1);

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesAssignIfDifferentSid)
        {
            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightPlan)))
                .Times(2)
                .WillOnce(testing::Return(sid1))
                .WillOnce(testing::Return(sid2));

            callsigns.AddUserCallsign(userCallsign);

            this->SetServiceProvision(true);

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(4)
                .WillRepeatedly(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(2).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));
            EXPECT_CALL(mockRadarTarget, GetAltitude()).Times(2).WillOnce(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(2).WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked()).Times(2).WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated()).Times(2).WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, GetSidName())
                .Times(3)
                .WillOnce(Return("ADMAG2X"))
                .WillOnce(Return("CLN3X"))
                .WillOnce(Return("CLN3X"));

            EXPECT_CALL(mockFlightPlan, GetOrigin()).Times(4).WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(mockFlightPlan, GetCallsign()).Times(7).WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(mockFlightPlan, SetHeading(125)).Times(1);

            EXPECT_CALL(mockFlightPlan, SetHeading(200)).Times(1);

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(2).WillRepeatedly(Return(MAX_ASSIGNMENT_SPEED));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesAssignAfterDisconnect)
        {
            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightPlan)))
                .Times(2)
                .WillRepeatedly(testing::Return(sid1));

            callsigns.AddUserCallsign(userCallsign);

            this->SetServiceProvision(true);

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(4)
                .WillRepeatedly(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel()).Times(4).WillRepeatedly(Return(MAX_ASSIGNMENT_ALTITUDE));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(2).WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked()).Times(2).WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated()).Times(2).WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, GetSidName()).Times(2).WillRepeatedly(Return("ADMAG2X"));

            EXPECT_CALL(mockFlightPlan, GetOrigin()).Times(4).WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(mockFlightPlan, GetCallsign()).Times(7).WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(mockFlightPlan, SetHeading(125)).Times(2);

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(2).WillRepeatedly(Return(MAX_ASSIGNMENT_SPEED));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
            handler.FlightPlanDisconnectEvent(mockFlightPlan);
            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleMarksAsAlreadyAssigned)
        {
            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightPlan)))
                .Times(1)
                .WillOnce(testing::Return(sid1));

            ON_CALL(this->mockFlightPlan, GetSidName()).WillByDefault(Return("ADMAG2X"));

            ON_CALL(this->mockFlightPlan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked()).WillByDefault(Return(false));

            ON_CALL(this->mockFlightPlan, GetDistanceFromOrigin()).WillByDefault(Return(5));

            ON_CALL(this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(500));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(125)).Times(1);

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading()).Times(0);

            EXPECT_CALL(mockFlightPlan, IsSimulated()).Times(0);

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed()).Times(0);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleSetsInitialHeading)
        {
            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightPlan)))
                .Times(1)
                .WillOnce(testing::Return(sid1));

            ON_CALL(this->mockFlightPlan, GetSidName()).WillByDefault(Return("ADMAG2X"));

            ON_CALL(this->mockFlightPlan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked()).WillByDefault(Return(false));

            ON_CALL(this->mockFlightPlan, GetDistanceFromOrigin()).WillByDefault(Return(5));

            ON_CALL(this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(500));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(125)).Times(1);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleSetsInitialHeadingWhenTrackedByUser)
        {
            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightPlan)))
                .Times(1)
                .WillOnce(testing::Return(sid1));

            ON_CALL(this->mockFlightPlan, GetSidName()).WillByDefault(Return("ADMAG2X"));

            ON_CALL(this->mockFlightPlan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked()).WillByDefault(Return(true));

            ON_CALL(this->mockFlightPlan, IsTrackedByUser()).WillByDefault(Return(true));

            ON_CALL(this->mockFlightPlan, GetDistanceFromOrigin()).WillByDefault(Return(5));

            ON_CALL(this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(500));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(125)).Times(1);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleDoesNothingIfAircraftOutOfRange)
        {
            ON_CALL(this->mockFlightPlan, GetSidName()).WillByDefault(Return("ADMAG2X"));

            ON_CALL(this->mockFlightPlan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked()).WillByDefault(Return(false));

            ON_CALL(this->mockFlightPlan, GetDistanceFromOrigin()).WillByDefault(Return(0));

            ON_CALL(this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(0));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(_)).Times(0);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleDoesNothingIfAircraftNotInAirportProximity)
        {
            ON_CALL(this->mockFlightPlan, GetSidName()).WillByDefault(Return("ADMAG2X"));

            ON_CALL(this->mockFlightPlan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked()).WillByDefault(Return(false));

            ON_CALL(this->mockFlightPlan, GetDistanceFromOrigin()).WillByDefault(Return(11));

            ON_CALL(this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(500));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(_)).Times(0);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleDoesNothingIfAircraftTrackedByAnotherUser)
        {
            ON_CALL(this->mockFlightPlan, GetSidName()).WillByDefault(Return("ADMAG2X"));

            ON_CALL(this->mockFlightPlan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked()).WillByDefault(Return(true));

            ON_CALL(this->mockFlightPlan, IsTrackedByUser()).WillByDefault(Return(false));

            ON_CALL(this->mockFlightPlan, GetDistanceFromOrigin()).WillByDefault(Return(5));

            ON_CALL(this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(500));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(_)).Times(0);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleDoesNothingIfNoSidFound)
        {
            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightPlan)))
                .Times(1)
                .WillOnce(testing::Return(nullptr));

            ON_CALL(this->mockFlightPlan, GetSidName()).WillByDefault(Return("ADMAG3X"));

            ON_CALL(this->mockFlightPlan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked()).WillByDefault(Return(false));

            ON_CALL(this->mockFlightPlan, GetDistanceFromOrigin()).WillByDefault(Return(5));

            ON_CALL(this->mockRadarTarget, GetFlightLevel()).WillByDefault(Return(500));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(_)).Times(0);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
        }

        TEST_F(InitialHeadingEventHandlerTest, NewActiveCallsignAssignsIfUserCallsign)
        {
            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplanPointer)))
                .Times(1)
                .WillOnce(testing::Return(sid1));

            callsigns.AddUserCallsign(userCallsign);

            this->SetServiceProvision(true);

            this->plugin.AddAllFlightplansItem({this->mockFlightplanPointer, this->mockRadarTargetPointer});

            EXPECT_CALL(*mockFlightplanPointer, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(*mockFlightplanPointer, HasControllerAssignedHeading()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(*mockFlightplanPointer, IsTracked()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(*mockFlightplanPointer, IsSimulated()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(*mockFlightplanPointer, GetSidName()).Times(1).WillRepeatedly(Return("ADMAG2X"));

            EXPECT_CALL(*mockFlightplanPointer, GetOrigin()).Times(2).WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(*mockFlightplanPointer, GetCallsign()).Times(3).WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(*mockFlightplanPointer, SetHeading(125)).Times(1);

            EXPECT_CALL(*mockRadarTargetPointer, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED));

            ON_CALL(*this->mockRadarTargetPointer, GetFlightLevel()).WillByDefault(Return(MAX_ASSIGNMENT_ALTITUDE - 1));
            ON_CALL(*this->mockRadarTargetPointer, GetAltitude()).WillByDefault(Return(MAX_ASSIGNMENT_ALTITUDE - 1));

            handler.ActiveCallsignAdded(userCallsign);
        }

        TEST_F(InitialHeadingEventHandlerTest, NewActiveCallsignDoesNotAssignIfNotUserCallsign)
        {
            this->SetServiceProvision(false);

            this->plugin.AddAllFlightplansItem({this->mockFlightplanPointer, this->mockRadarTargetPointer});
            this->plugin.ExpectNoFlightplanLoop();

            handler.ActiveCallsignAdded(notUserCallsign);
        }

        TEST_F(InitialHeadingEventHandlerTest, TimedEventAssignsIfUserCallsign)
        {
            EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplanPointer)))
                .Times(1)
                .WillOnce(testing::Return(sid1));

            callsigns.AddUserCallsign(userCallsign);

            this->SetServiceProvision(true);

            this->plugin.AddAllFlightplansItem({this->mockFlightplanPointer, this->mockRadarTargetPointer});

            EXPECT_CALL(*mockFlightplanPointer, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(MAX_DISTANCE_FROM_ORIGIN));

            EXPECT_CALL(*mockFlightplanPointer, HasControllerAssignedHeading()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(*mockFlightplanPointer, IsTracked()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(*mockFlightplanPointer, IsSimulated()).Times(1).WillOnce(Return(false));

            EXPECT_CALL(*mockFlightplanPointer, GetSidName()).Times(1).WillRepeatedly(Return("ADMAG2X"));

            EXPECT_CALL(*mockFlightplanPointer, GetOrigin()).Times(2).WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(*mockFlightplanPointer, GetCallsign()).Times(3).WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(*mockFlightplanPointer, SetHeading(125)).Times(1);

            EXPECT_CALL(*mockRadarTargetPointer, GetGroundSpeed()).Times(1).WillOnce(Return(MAX_ASSIGNMENT_SPEED));

            ON_CALL(*this->mockRadarTargetPointer, GetFlightLevel()).WillByDefault(Return(MAX_ASSIGNMENT_ALTITUDE - 1));
            ON_CALL(*this->mockRadarTargetPointer, GetAltitude()).WillByDefault(Return(MAX_ASSIGNMENT_ALTITUDE - 1));

            handler.TimedEventTrigger();
        }

        TEST_F(InitialHeadingEventHandlerTest, TimedEventDoesNotAssignIfNoUserCallsign)
        {
            callsigns.AddCallsign(userCallsign);
            this->SetServiceProvision(false);

            this->plugin.AddAllFlightplansItem({this->mockFlightplanPointer, this->mockRadarTargetPointer});
            this->plugin.ExpectNoFlightplanLoop();

            handler.TimedEventTrigger();
        }
    } // namespace InitialHeading
} // namespace UKControllerPluginTest
