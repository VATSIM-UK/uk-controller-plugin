#include "api/ApiException.h"
#include "controller/ControllerPosition.h"
#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachAudioAlert.h"
#include "missedapproach/MissedApproachButton.h"
#include "missedapproach/MissedApproachCollection.h"
#include "missedapproach/MissedApproachOptions.h"
#include "missedapproach/ToggleMissedApproachButton.h"
#include "missedapproach/TriggerMissedApproach.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"
#include "time/ParseTimeStrings.h"
#include "time/SystemClock.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachAudioAlert;
using UKControllerPlugin::MissedApproach::MissedApproachButton;
using UKControllerPlugin::MissedApproach::MissedApproachCollection;
using UKControllerPlugin::MissedApproach::MissedApproachOptions;
using UKControllerPlugin::MissedApproach::ToggleMissedApproachButton;
using UKControllerPlugin::MissedApproach::TriggerMissedApproach;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Time::ParseTimeString;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest::MissedApproach {
    class ToggleMissedApproachButtonTest : public testing::Test
    {
        public:
        ToggleMissedApproachButtonTest()
            : settings(mockUserSettingProvider),
              mockRadarTarget(std::make_shared<NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>()),
              mockFlightplan(std::make_shared<NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>()),
              kkTwr(2, "EGKK_TWR", 199.999, {"EGKK"}, true, false),
              userTowerCallsign(std::make_shared<ActiveCallsign>("EGKK_TWR", "Testy McTest", kkTwr, true)),
              collection(std::make_shared<MissedApproachCollection>()),
              options(std::make_shared<MissedApproachOptions>()),
              audioAlert(std::make_shared<MissedApproachAudioAlert>(options, plugin, serviceProviders, windows)),
              trigger(std::make_shared<TriggerMissedApproach>(
                  collection, windows, api, serviceProviders, audioAlert, mockIntegration)),
              button(std::make_shared<MissedApproachButton>(collection, trigger, plugin, serviceProviders, 55)),
              toggle(button, 5)
        {
            std::vector<std::shared_ptr<ServiceProvision>> provisions;
            provisions.push_back(std::make_shared<ServiceProvision>(ServiceType::Tower, userTowerCallsign));
            serviceProviders.SetProvidersForAirfield("EGKK", provisions);

            ON_CALL(*mockFlightplan, GetCallsign).WillByDefault(Return("BAW123"));
            ON_CALL(*mockFlightplan, GetDestination).WillByDefault(Return("EGKK"));

            ON_CALL(*mockFlightplan, GetDistanceToDestination()).WillByDefault(Return(5.0));
            ON_CALL(*mockRadarTarget, GetFlightLevel()).WillByDefault(Return(3000));
            ON_CALL(*mockRadarTarget, GetGroundSpeed()).WillByDefault(Return(100));

            ON_CALL(plugin, GetSelectedFlightplan).WillByDefault(Return(mockFlightplan));

            ON_CALL(plugin, GetSelectedRadarTarget).WillByDefault(Return(mockRadarTarget));
        }

        NiceMock<Integration::MockOutboundIntegrationEventHandler> mockIntegration;
        NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
        NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
        UserSetting settings;
        AirfieldServiceProviderCollection serviceProviders;
        std::shared_ptr<NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
        std::shared_ptr<NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> mockFlightplan;
        ControllerPosition kkTwr;
        std::shared_ptr<ActiveCallsign> userTowerCallsign;
        NiceMock<MockWinApi> windows;
        NiceMock<MockApiInterface> api;
        NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
        std::shared_ptr<MissedApproachCollection> collection;
        std::shared_ptr<MissedApproachOptions> options;
        std::shared_ptr<MissedApproachAudioAlert> audioAlert;
        std::shared_ptr<TriggerMissedApproach> trigger;
        std::shared_ptr<MissedApproachButton> button;
        ToggleMissedApproachButton toggle;
    };

    TEST_F(ToggleMissedApproachButtonTest, ItTogglesTheButton)
    {
        EXPECT_FALSE(button->IsVisible());
        toggle.Configure(1, "", {});
        EXPECT_TRUE(button->IsVisible());
        toggle.Configure(1, "", {});
        EXPECT_FALSE(button->IsVisible());
    }

    TEST_F(ToggleMissedApproachButtonTest, ItHasTheMenuItemWithButtonInvisible)
    {
        PopupMenuItem expected;
        expected.firstValue = "Display Missed Approach Button";
        expected.secondValue = "";
        expected.callbackFunctionId = 5;
        expected.checked = EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED;
        expected.disabled = false;
        expected.fixedPosition = false;

        EXPECT_EQ(expected, toggle.GetConfigurationMenuItem());
    }

    TEST_F(ToggleMissedApproachButtonTest, ItHasTheMenuItemWithButtonVisible)
    {
        button->ToggleVisible();
        PopupMenuItem expected;
        expected.firstValue = "Display Missed Approach Button";
        expected.secondValue = "";
        expected.callbackFunctionId = 5;
        expected.checked = EuroScopePlugIn::POPUP_ELEMENT_CHECKED;
        expected.disabled = false;
        expected.fixedPosition = false;

        EXPECT_EQ(expected, toggle.GetConfigurationMenuItem());
    }
} // namespace UKControllerPluginTest::MissedApproach
