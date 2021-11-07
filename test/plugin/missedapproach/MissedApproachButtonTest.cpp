#include "api/ApiException.h"
#include "controller/ControllerPosition.h"
#include "euroscope/UserSetting.h"
#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachAudioAlert.h"
#include "missedapproach/MissedApproachButton.h"
#include "missedapproach/MissedApproachCollection.h"
#include "missedapproach/MissedApproachOptions.h"
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
using UKControllerPlugin::MissedApproach::TriggerMissedApproach;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPlugin::Time::ParseTimeString;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachButtonTest : public testing::Test
    {
        public:
        MissedApproachButtonTest()
            : settings(mockUserSettingProvider),
              mockRadarTarget(std::make_shared<NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>()),
              mockFlightplan(std::make_shared<NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>()),
              kkTwr(2, "EGKK_TWR", 199.999, {"EGKK"}, true, false),
              userTowerCallsign(std::make_shared<ActiveCallsign>("EGKK_TWR", "Testy McTest", kkTwr, true)),
              collection(std::make_shared<MissedApproachCollection>()),
              options(std::make_shared<MissedApproachOptions>()),
              audioAlert(std::make_shared<MissedApproachAudioAlert>(options, plugin, serviceProviders, windows)),
              trigger(std::make_shared<TriggerMissedApproach>(collection, windows, api, serviceProviders, audioAlert)),
              button(collection, trigger, plugin, serviceProviders, 55)
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
        MissedApproachButton button;
    };

    TEST_F(MissedApproachButtonTest, LoadingAsrSetsDefaultVisibilityIfNoSetting)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachButtonVisibility")).Times(1).WillOnce(Return(""));

        button.AsrLoadedEvent(settings);
        EXPECT_FALSE(button.IsVisible());
    }

    TEST_F(MissedApproachButtonTest, LoadingAsrSetsDefaultVisibilityIfInvalid)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachButtonVisibility")).Times(1).WillOnce(Return("abc"));

        button.AsrLoadedEvent(settings);
        EXPECT_FALSE(button.IsVisible());
    }

    TEST_F(MissedApproachButtonTest, LoadingAsrSetsVisibility)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachButtonVisibility")).Times(1).WillOnce(Return("1"));

        button.AsrLoadedEvent(settings);
        EXPECT_TRUE(button.IsVisible());
    }

    TEST_F(MissedApproachButtonTest, LoadingAsrSetsDefaultXPositionsIfNoSetting)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachButtonXPosition")).Times(1).WillOnce(Return(""));

        button.AsrLoadedEvent(settings);

        EXPECT_EQ(200, button.Position().left);
        EXPECT_EQ(325, button.Position().right);
    }

    TEST_F(MissedApproachButtonTest, LoadingAsrSetsDefaultXPositionsIfInvalid)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachButtonXPosition")).Times(1).WillOnce(Return("abc"));

        button.AsrLoadedEvent(settings);

        EXPECT_EQ(200, button.Position().left);
        EXPECT_EQ(325, button.Position().right);
    }

    TEST_F(MissedApproachButtonTest, LoadingAsrSetsXPositions)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachButtonXPosition")).Times(1).WillOnce(Return("50"));

        button.AsrLoadedEvent(settings);

        EXPECT_EQ(50, button.Position().left);
        EXPECT_EQ(175, button.Position().right);
    }

    TEST_F(MissedApproachButtonTest, LoadingAsrSetsDefaultYPositionsIfNoSetting)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachButtonYPosition")).Times(1).WillOnce(Return(""));

        button.AsrLoadedEvent(settings);

        EXPECT_EQ(200, button.Position().top);
        EXPECT_EQ(275, button.Position().bottom);
    }

    TEST_F(MissedApproachButtonTest, LoadingAsrSetsDefaultYPositionsIfInvalid)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachButtonYPosition")).Times(1).WillOnce(Return("abc"));

        button.AsrLoadedEvent(settings);

        EXPECT_EQ(200, button.Position().top);
        EXPECT_EQ(275, button.Position().bottom);
    }

    TEST_F(MissedApproachButtonTest, LoadingAsrSetsYPositions)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachButtonYPosition")).Times(1).WillOnce(Return("50"));

        button.AsrLoadedEvent(settings);

        EXPECT_EQ(50, button.Position().top);
        EXPECT_EQ(125, button.Position().bottom);
    }

    TEST_F(MissedApproachButtonTest, SavingAsrSavesEverything)
    {
        EXPECT_CALL(
            mockUserSettingProvider,
            SetKey(
                "missedApproachButtonVisibility",
                "Missed Approach Button "
                "Visibility",
                "0"))
            .Times(1);

        EXPECT_CALL(
            mockUserSettingProvider,
            SetKey("missedApproachButtonXPosition", "Missed Approach Button X Position", "200"))
            .Times(1);

        EXPECT_CALL(
            mockUserSettingProvider,
            SetKey("missedApproachButtonYPosition", "Missed Approach Button Y Position", "200"))
            .Times(1);

        button.AsrClosingEvent(settings);
    }

    TEST_F(MissedApproachButtonTest, ItMovesThings)
    {
        button.Move({100, 300, 1, 2}, "");

        EXPECT_EQ(100, button.Position().left);
        EXPECT_EQ(300, button.Position().top);
        EXPECT_EQ(225, button.Position().right);
        EXPECT_EQ(375, button.Position().bottom);
    }

    TEST_F(MissedApproachButtonTest, ItResetsPosition)
    {
        button.Move({100, 300, 1, 2}, "");
        button.ResetPosition();

        EXPECT_EQ(200, button.Position().left);
        EXPECT_EQ(200, button.Position().top);
        EXPECT_EQ(325, button.Position().right);
        EXPECT_EQ(275, button.Position().bottom);
    }

    TEST_F(MissedApproachButtonTest, LeftClickTriggersMissedApproach)
    {

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::_, testing::_))
            .Times(1)
            .WillOnce(testing::Return(IDNO));

        button.LeftClick(mockRadarScreen, 1, "", POINT{}, RECT{});
    }

    TEST_F(MissedApproachButtonTest, LeftClickDoesntTriggerApproachIfNoFlightplanFound)
    {
        ON_CALL(this->plugin, GetSelectedFlightplan).WillByDefault(Return(nullptr));

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::_, testing::_)).Times(0);

        button.LeftClick(mockRadarScreen, 1, "", POINT{}, RECT{});
    }

    TEST_F(MissedApproachButtonTest, LeftClickDoesntTriggerApproachIfNoRadarTargetFound)
    {
        ON_CALL(this->plugin, GetSelectedRadarTarget).WillByDefault(Return(nullptr));

        EXPECT_CALL(windows, OpenMessageBox(testing::_, testing::_, testing::_)).Times(0);

        button.LeftClick(mockRadarScreen, 1, "", POINT{}, RECT{});
    }
} // namespace UKControllerPluginTest::MissedApproach
