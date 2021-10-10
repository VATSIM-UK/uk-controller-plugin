#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "euroscope/UserSetting.h"
#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachAudioAlert.h"
#include "missedapproach/MissedApproachOptions.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"

using testing::NiceMock;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachAudioAlert;
using UKControllerPlugin::MissedApproach::MissedApproachOptions;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Ownership::ServiceProvision;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachAudioAlertTest : public testing::Test
    {
        public:
        MissedApproachAudioAlertTest()
            : mockFlightplan(std::make_shared<MockEuroScopeCFlightPlanInterface>()), settings(mockUserSettingProvider),
              options(std::make_shared<MissedApproachOptions>(settings)),
              alert(options, mockPlugin, serviceProviders, mockWindows),
              kkTwr(2, "EGKK_TWR", 199.999, {"EGKK"}, true, false),
              userTowerCallsign(std::make_shared<ActiveCallsign>("EGKK_TWR", "Testy McTest", kkTwr, true))
        {
            ON_CALL(mockPlugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(mockFlightplan));

            ON_CALL(*mockFlightplan, GetDestination).WillByDefault(testing::Return("EGKK"));

            std::vector<std::shared_ptr<ServiceProvision>> provisions;
            provisions.push_back(std::make_shared<ServiceProvision>(ServiceType::Tower, userTowerCallsign));
            serviceProviders.SetProvidersForAirfield("EGKK", provisions);

            this->options->SetAudioAlertServiceProvisions(ServiceType::Tower);
            this->options->SetAudioAlertForCurrentUser(false);
        }

        [[nodiscard]] static auto Create(bool isUserCreated) -> std::shared_ptr<class MissedApproach>
        {
            return std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), isUserCreated);
        }

        AirfieldServiceProviderCollection serviceProviders;
        std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        NiceMock<MockWinApi> mockWindows;
        NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
        NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
        UserSetting settings;
        std::shared_ptr<MissedApproachOptions> options;
        MissedApproachAudioAlert alert;
        ControllerPosition kkTwr;
        std::shared_ptr<ActiveCallsign> userTowerCallsign;
    };

    TEST_F(MissedApproachAudioAlertTest, ItPlaysAlertIfNotCreatedByUser)
    {
        EXPECT_CALL(mockWindows, PlayWave(MAKEINTRESOURCE(WAVE_MISSED_APPROACH))).Times(1);

        alert.Play(Create(false));
    }

    TEST_F(MissedApproachAudioAlertTest, ItPlaysAlertIfCreatedByUser)
    {
        this->options->SetAudioAlertForCurrentUser(true);
        EXPECT_CALL(mockWindows, PlayWave(MAKEINTRESOURCE(WAVE_MISSED_APPROACH))).Times(1);

        alert.Play(Create(true));
    }

    TEST_F(MissedApproachAudioAlertTest, ItDoesntPlayAudioAlertIfUserNotProvidingServices)
    {
        this->serviceProviders.Flush();
        EXPECT_CALL(mockWindows, PlayWave(MAKEINTRESOURCE(WAVE_MISSED_APPROACH))).Times(0);

        alert.Play(Create(false));
    }

    TEST_F(MissedApproachAudioAlertTest, ItDoesntPlayAudioAlertIfUserNotProvidingCorrectServices)
    {
        std::vector<std::shared_ptr<ServiceProvision>> provisions;
        provisions.push_back(std::make_shared<ServiceProvision>(ServiceType::FinalApproach, userTowerCallsign));
        serviceProviders.SetProvidersForAirfield("EGKK", provisions);

        EXPECT_CALL(mockWindows, PlayWave(MAKEINTRESOURCE(WAVE_MISSED_APPROACH))).Times(0);

        alert.Play(Create(false));
    }

    TEST_F(MissedApproachAudioAlertTest, ItDoesntPlayAudioAlertIfNoFlightplan)
    {
        ON_CALL(mockPlugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));
        EXPECT_CALL(mockWindows, PlayWave(MAKEINTRESOURCE(WAVE_MISSED_APPROACH))).Times(0);

        alert.Play(Create(false));
    }

    TEST_F(MissedApproachAudioAlertTest, ItDoesntPlayAudioAlertIfCreatedByUserAndUserDoesntWantAlerts)
    {
        this->options->SetAudioAlertForCurrentUser(false);
        EXPECT_CALL(mockWindows, PlayWave(MAKEINTRESOURCE(WAVE_MISSED_APPROACH))).Times(0);

        alert.Play(Create(true));
    }
} // namespace UKControllerPluginTest::MissedApproach
