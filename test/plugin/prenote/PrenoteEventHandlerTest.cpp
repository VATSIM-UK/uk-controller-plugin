#include "prenote/PrenoteEventHandler.h"
#include "prenote/PrenoteService.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "airfield/AirfieldCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "message/UserMessager.h"
#include "prenote/AbstractPrenote.h"
#include "airfield/AirfieldModel.h"
#include "euroscope/UserSetting.h"
#include "euroscope/GeneralSettingsEntries.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Message::UserMessager;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Prenote::AbstractPrenote;
using UKControllerPlugin::Prenote::PrenoteEventHandler;
using UKControllerPlugin::Prenote::PrenoteService;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest {
    namespace Prenote {
        class PrenoteEventHandlerTest : public Test
        {
            public:
            void SetUp(void)
            {
                this->airfields = std::make_unique<AirfieldCollection>();
                this->airfields->AddAirfield(std::unique_ptr<AirfieldModel>(new AirfieldModel("EGKK", {"EGKK_GND"})));
                this->airfieldOwnership =
                    std::make_unique<AirfieldOwnershipManager>(*this->airfields, this->activeCallsigns);

                this->messager = std::make_unique<UserMessager>(this->mockPlugin);
                std::unique_ptr<PrenoteService> service =
                    std::make_unique<PrenoteService>(*this->airfieldOwnership, this->activeCallsigns, *this->messager);
                this->userSetting = std::make_unique<UserSetting>(this->mockSettingProvider);
                this->eventHandler = std::make_unique<PrenoteEventHandler>(std::move(service), *this->userSetting);
            };

            std::unique_ptr<UserSetting> userSetting;
            NiceMock<UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface> mockSettingProvider;
            std::unique_ptr<PrenoteEventHandler> eventHandler;
            std::unique_ptr<PrenoteService> service;
            std::unique_ptr<AirfieldCollection> airfields;
            std::unique_ptr<AirfieldOwnershipManager> airfieldOwnership;
            ActiveCallsignCollection activeCallsigns;
            std::unique_ptr<UserMessager> messager;
            NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
            NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        };

        TEST_F(PrenoteEventHandlerTest, FlightplanEventCancelsSentPrenotes)
        {
            EXPECT_CALL(this->mockFlightplan, GetCallsign()).Times(1).WillOnce(Return("BAW123"));

            this->eventHandler->FlightPlanEvent(this->mockFlightplan, this->mockRadarTarget);
        }

        TEST_F(PrenoteEventHandlerTest, FlightPlanDisconnectEventCancelsSentPrenotes)
        {
            EXPECT_CALL(this->mockFlightplan, GetCallsign()).Times(1).WillOnce(Return("BAW123"));

            this->eventHandler->FlightPlanDisconnectEvent(this->mockFlightplan);
        }

        TEST_F(PrenoteEventHandlerTest, ControllerFlightPlanDataEventDoesNothingIfPrenotesDisabled)
        {
            ON_CALL(this->mockSettingProvider, KeyExists(GeneralSettingsEntries::usePrenoteSettingsKey))
                .WillByDefault(Return(true));

            ON_CALL(this->mockSettingProvider, GetKey(GeneralSettingsEntries::usePrenoteSettingsKey))
                .WillByDefault(Return("0"));

            this->eventHandler->ControllerFlightPlanDataEvent(
                this->mockFlightplan, EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE);
        }

        TEST_F(PrenoteEventHandlerTest, ControllerFlightPlanDataEventDoesNothingIfNotGroundStatusDataType)
        {
            ON_CALL(this->mockSettingProvider, KeyExists(GeneralSettingsEntries::usePrenoteSettingsKey))
                .WillByDefault(Return(true));

            ON_CALL(this->mockSettingProvider, GetKey(GeneralSettingsEntries::usePrenoteSettingsKey))
                .WillByDefault(Return("1"));

            this->eventHandler->ControllerFlightPlanDataEvent(
                this->mockFlightplan, EuroScopePlugIn::CTR_DATA_TYPE_FINAL_ALTITUDE);
        }

        TEST_F(PrenoteEventHandlerTest, ControllerFlightPlanDataEventDoesNothingIfNotPush)
        {
            ON_CALL(this->mockFlightplan, GetGroundState()).WillByDefault(Return("TAXI"));

            ON_CALL(this->mockSettingProvider, KeyExists(GeneralSettingsEntries::usePrenoteSettingsKey))
                .WillByDefault(Return(true));

            ON_CALL(this->mockSettingProvider, GetKey(GeneralSettingsEntries::usePrenoteSettingsKey))
                .WillByDefault(Return("1"));

            this->eventHandler->ControllerFlightPlanDataEvent(
                this->mockFlightplan, EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE);
        }

        TEST_F(PrenoteEventHandlerTest, ControllerFlightPlanDataEventAttempsPrenoteSendingOnPush)
        {
            // The call to GetOrigin means that it's trying to send prenotes
            EXPECT_CALL(this->mockFlightplan, GetOrigin()).Times(1).WillOnce(Return("EGKK"));

            ON_CALL(this->mockFlightplan, GetGroundState()).WillByDefault(Return("PUSH"));

            ON_CALL(this->mockSettingProvider, KeyExists(GeneralSettingsEntries::usePrenoteSettingsKey))
                .WillByDefault(Return(true));

            ON_CALL(this->mockSettingProvider, GetKey(GeneralSettingsEntries::usePrenoteSettingsKey))
                .WillByDefault(Return("1"));

            this->eventHandler->ControllerFlightPlanDataEvent(
                this->mockFlightplan, EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE);
        }
    } // namespace Prenote
} // namespace UKControllerPluginTest
