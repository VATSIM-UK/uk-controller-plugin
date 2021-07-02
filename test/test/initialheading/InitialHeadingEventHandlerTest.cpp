#include "pch/pch.h"
#include "initialheading/InitialHeadingEventHandler.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "controller/ActiveCallsignCollection.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "airfield/AirfieldModel.h"
#include "login/Login.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "timedevent/DeferredEventHandler.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "mock/MockUserSettingProviderInterface.h"
#include "euroscope/UserSetting.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

using UKControllerPlugin::InitialHeading::InitialHeadingEventHandler;
using UKControllerPlugin::Sid::SidCollection;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::TimedEvent::DeferredEventHandler;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Flightplan::StoredFlightplan;

using ::testing::Test;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::Throw;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace InitialHeading {

        class InitialHeadingEventHandlerTest : public Test
        {
            public:
                InitialHeadingEventHandlerTest()
                    :  owners(airfields, callsigns),
                    login(plugin, ControllerStatusEventHandlerCollection()),
                    handler(sids, callsigns, owners, login, deferredEvents, plugin, flightplans),
                    controller(1, "LON_S_CTR", 129.420, {"EGKK"}, true, false),
                    userCallsign("LON_S_CTR", "Test", controller)
                {

                }

                virtual void SetUp() {
                    // Pretend we've been logged in a while
                    login.SetLoginStatus(EuroScopePlugIn::CONNECTION_TYPE_DIRECT);
                    login.SetLoginTime(std::chrono::system_clock::now() - std::chrono::minutes(15));
                    sids.AddSid(std::make_shared<StandardInstrumentDeparture>("EGKK", "ADMAG2X", 6000, 125));
                    sids.AddSid(std::make_shared<StandardInstrumentDeparture>("EGKK", "CLN3X", 5000, 200));
                    sids.AddSid(std::make_shared<StandardInstrumentDeparture>("EGKK", "DAGGA1X", 5000, 0));

                    ON_CALL(mockFlightPlan, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                }


                ControllerPosition controller;
                ActiveCallsign userCallsign;
                AirfieldCollection airfields;
                DeferredEventHandler deferredEvents;
                NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightPlan;
                NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
                NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
                Login login;
                StoredFlightplanCollection flightplans;
                ActiveCallsignCollection callsigns;
                AirfieldOwnershipManager owners;
                SidCollection sids;
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
            login.SetLoginTime(std::chrono::system_clock::now() + std::chrono::minutes(15));
            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
            EXPECT_EQ(1, deferredEvents.Count());
            int64_t seconds = std::chrono::duration_cast<std::chrono::seconds> (
                this->deferredEvents.NextEventTime() - std::chrono::system_clock::now()
                )
                .count();
            EXPECT_LE(seconds, 5);
            EXPECT_GT(seconds, 3);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfTooHigh)
        {
            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxAltitude + 1));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfExactlyAtSeaLevel)
        {
            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(1)
                .WillOnce(Return(0));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfTooFarFromOrigin)
        {
            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxDistanceFromOrigin + 1));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfExactlyOnOrigin)
        {
            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(1)
                .WillOnce(Return(0));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfTravellingTooFast)
        {
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed + 1));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfAlreadyHasAnAssignedHeading)
        {
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillOnce(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillOnce(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfTracked)
        {
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked())
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfAircraftIsSimulated)
        {
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillOnce(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillOnce(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated())
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfSidHasNoInitialHeading)
        {
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillOnce(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillOnce(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated())
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed));

            ON_CALL(mockFlightPlan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(mockFlightPlan, GetSidName())
                .WillByDefault(Return("DAGGA1X"));

            EXPECT_CALL(mockFlightPlan, SetHeading(_))
                .Times(0);

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfUserCallsignIsNotActive)
        {
            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, GetOrigin())
                .Times(1)
                .WillOnce(Return("EGKK"));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfAirfieldIsNotOwnedByUser)
        {
            callsigns.AddUserCallsign(userCallsign);

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillOnce(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillOnce(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, GetOrigin())
                .Times(1)
                .WillOnce(Return("EGKK"));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfSidNotFound)
        {
            callsigns.AddUserCallsign(userCallsign);

            airfields.AddAirfield(std::unique_ptr<AirfieldModel>(new AirfieldModel("EGKK", { "LON_S_CTR" })));
            owners.RefreshOwner("EGKK");

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, GetSidName())
                .Times(1)
                .WillOnce(Return("ADMAG1X"));

            EXPECT_CALL(mockFlightPlan, GetOrigin())
                .Times(2)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesNotAssignIfAlreadyAssignedOnSameSid)
        {
            callsigns.AddUserCallsign(userCallsign);

            airfields.AddAirfield(std::unique_ptr<AirfieldModel>(new AirfieldModel("EGKK", { "LON_S_CTR" })));
            owners.RefreshOwner("EGKK");

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, GetSidName())
                .Times(3)
                .WillRepeatedly(Return("ADMAG2X"));

            EXPECT_CALL(mockFlightPlan, GetOrigin())
                .Times(3)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(mockFlightPlan, GetCallsign())
                .Times(5)
                .WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(mockFlightPlan, SetHeading(125))
                .Times(1);

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventAssignsIfSidFound)
        {
            callsigns.AddUserCallsign(userCallsign);

            airfields.AddAirfield(std::unique_ptr<AirfieldModel>(new AirfieldModel("EGKK", { "LON_S_CTR" })));
            owners.RefreshOwner("EGKK");

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, GetSidName())
                .Times(2)
                .WillOnce(Return("ADMAG2X"));

            EXPECT_CALL(mockFlightPlan, GetOrigin())
                .Times(3)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(mockFlightPlan, GetCallsign())
                .Times(3)
                .WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(mockFlightPlan, SetHeading(125))
                .Times(1);

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventAcceptsDeprecatedSids)
        {
            callsigns.AddUserCallsign(userCallsign);

            airfields.AddAirfield(std::unique_ptr<AirfieldModel>(new AirfieldModel("EGKK", { "LON_S_CTR" })));
            owners.RefreshOwner("EGKK");

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(mockFlightPlan, GetSidName())
                .Times(2)
                .WillRepeatedly(Return("#ADMAG2X"));

            EXPECT_CALL(mockFlightPlan, GetOrigin())
                .Times(3)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(mockFlightPlan, GetCallsign())
                .Times(3)
                .WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(mockFlightPlan, SetHeading(125))
                .Times(1);

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesAssignIfDifferentSid)
        {
            callsigns.AddUserCallsign(userCallsign);

            airfields.AddAirfield(std::unique_ptr<AirfieldModel>(new AirfieldModel("EGKK", { "LON_S_CTR" })));
            owners.RefreshOwner("EGKK");

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(4)
                .WillRepeatedly(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(4)
                .WillRepeatedly(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(2)
                .WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked())
                .Times(2)
                .WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated())
                .Times(2)
                .WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, GetSidName())
                .Times(5)
                .WillOnce(Return("ADMAG2X"))
                .WillOnce(Return("ADMAG2X"))
                .WillOnce(Return("CLN3X"))
                .WillOnce(Return("CLN3X"))
                .WillOnce(Return("CLN3X"));

            EXPECT_CALL(mockFlightPlan, GetOrigin())
                .Times(6)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(mockFlightPlan, GetCallsign())
                .Times(7)
                .WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(mockFlightPlan, SetHeading(125))
                .Times(1);

            EXPECT_CALL(mockFlightPlan, SetHeading(200))
                .Times(1);

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxSpeed));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, FlightPlanEventDoesAssignAfterDisconnect)
        {
            callsigns.AddUserCallsign(userCallsign);

            airfields.AddAirfield(std::unique_ptr<AirfieldModel>(new AirfieldModel("EGKK", { "LON_S_CTR" })));
            owners.RefreshOwner("EGKK");

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(4)
                .WillRepeatedly(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(mockRadarTarget, GetFlightLevel())
                .Times(4)
                .WillRepeatedly(Return(handler.assignmentMaxAltitude));

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(2)
                .WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, IsTracked())
                .Times(2)
                .WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, IsSimulated())
                .Times(2)
                .WillRepeatedly(Return(false));

            EXPECT_CALL(mockFlightPlan, GetSidName())
                .Times(4)
                .WillRepeatedly(Return("ADMAG2X"));

            EXPECT_CALL(mockFlightPlan, GetOrigin())
                .Times(6)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(mockFlightPlan, GetCallsign())
                .Times(7)
                .WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(mockFlightPlan, SetHeading(125))
                .Times(2);

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxSpeed));

            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
            handler.FlightPlanDisconnectEvent(mockFlightPlan);
            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleMarksAsAlreadyAssigned)
        {
            ON_CALL(this->mockFlightPlan, GetSidName())
                .WillByDefault(Return("ADMAG2X"));

            ON_CALL(this->mockFlightPlan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked())
                .WillByDefault(Return(false));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(125))
                .Times(1);

            EXPECT_CALL(mockFlightPlan, GetDistanceFromOrigin())
                .Times(0);

            EXPECT_CALL(mockFlightPlan, HasControllerAssignedHeading())
                .Times(0);

            EXPECT_CALL(mockFlightPlan, IsSimulated())
                .Times(0);

            EXPECT_CALL(mockRadarTarget, GetGroundSpeed())
                .Times(0);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
            handler.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleSetsInitialHeading)
        {
            ON_CALL(this->mockFlightPlan, GetSidName())
                .WillByDefault(Return("ADMAG2X"));

            ON_CALL(this->mockFlightPlan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked())
                .WillByDefault(Return(false));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(125))
                .Times(1);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleSetsInitialHeadingWhenTrackedByUser)
        {
            ON_CALL(this->mockFlightPlan, GetSidName())
                .WillByDefault(Return("ADMAG2X"));

            ON_CALL(this->mockFlightPlan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked())
                .WillByDefault(Return(true));

            ON_CALL(this->mockFlightPlan, IsTrackedByUser())
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(125))
                .Times(1);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleDoesNothingIfAircraftTrackedByAnotherUser)
        {
            ON_CALL(this->mockFlightPlan, GetSidName())
                .WillByDefault(Return("ADMAG2X"));

            ON_CALL(this->mockFlightPlan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked())
                .WillByDefault(Return(true));

            ON_CALL(this->mockFlightPlan, IsTrackedByUser())
                .WillByDefault(Return(false));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(_))
                .Times(0);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
        }

        TEST_F(InitialHeadingEventHandlerTest, RecycleDoesNothingIfNoSidFound)
        {
            ON_CALL(this->mockFlightPlan, GetSidName())
                .WillByDefault(Return("ADMAG3X"));

            ON_CALL(this->mockFlightPlan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->mockFlightPlan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightPlan, IsTracked())
                .WillByDefault(Return(false));

            EXPECT_CALL(this->mockFlightPlan, SetHeading(_))
                .Times(0);

            handler.RecycleInitialHeading(this->mockFlightPlan, this->mockRadarTarget, "", POINT());
        }

        TEST_F(InitialHeadingEventHandlerTest, NewActiveCallsignAssignsIfUserCallsign)
        {
            callsigns.AddUserCallsign(userCallsign);

            airfields.AddAirfield(std::unique_ptr<AirfieldModel>(new AirfieldModel("EGKK", { "LON_S_CTR" })));
            owners.RefreshOwner("EGKK");
            this->flightplans.UpdatePlan(StoredFlightplan("BAW123", "EGKK", "EGPF"));

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockReturnFlightplan =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> mockReturnRadarTarget =
                std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(mockReturnFlightplan));

            ON_CALL(this->plugin, GetRadarTargetForCallsign("BAW123"))
                .WillByDefault(Return(mockReturnRadarTarget));

            EXPECT_CALL(*mockReturnFlightplan, GetDistanceFromOrigin())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxDistanceFromOrigin));

            EXPECT_CALL(*mockReturnFlightplan, HasControllerAssignedHeading())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(*mockReturnFlightplan, IsTracked())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(*mockReturnFlightplan, IsSimulated())
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(*mockReturnFlightplan, GetSidName())
                .Times(2)
                .WillRepeatedly(Return("ADMAG2X"));

            EXPECT_CALL(*mockReturnFlightplan, GetOrigin())
                .Times(3)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(*mockReturnFlightplan, GetCallsign())
                .Times(3)
                .WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(*mockReturnFlightplan, SetHeading(125))
                .Times(1);

            EXPECT_CALL(*mockReturnRadarTarget, GetGroundSpeed())
                .Times(1)
                .WillOnce(Return(handler.assignmentMaxSpeed));

            EXPECT_CALL(*mockReturnRadarTarget, GetFlightLevel())
                .Times(2)
                .WillRepeatedly(Return(handler.assignmentMaxSpeed));

            handler.ActiveCallsignAdded(userCallsign, true);
        }

        TEST_F(InitialHeadingEventHandlerTest, NewActiveCallsignDoesNotAssignIfNotUserCallsign)
        {
            callsigns.AddCallsign(userCallsign);
            airfields.AddAirfield(std::unique_ptr<AirfieldModel>(new AirfieldModel("EGKK", { "LON_S_CTR" })));
            owners.RefreshOwner("EGKK");
            this->flightplans.UpdatePlan(StoredFlightplan("BAW123", "EGKK", "EGPF"));

            EXPECT_CALL(mockFlightPlan, SetHeading(6000))
                .Times(0);

            handler.ActiveCallsignAdded(userCallsign, false);
        }
    }  // namespace InitialHeading
}  // namespace UKControllerPluginTest
