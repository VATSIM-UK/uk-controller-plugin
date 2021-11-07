#include "euroscope/UserSettingAwareCollection.h"
#include "missedapproach/MissedApproachOptions.h"
#include "missedapproach/MissedApproachUserSettingHandler.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Euroscope::UserSettingAwareCollection;
using UKControllerPlugin::MissedApproach::MissedApproachOptions;
using UKControllerPlugin::MissedApproach::MissedApproachUserSettingHandler;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachUserSettingHandlerTest : public testing::Test
    {
        public:
        MissedApproachUserSettingHandlerTest()
            : mockUserSettingAware(std::make_shared<NiceMock<Euroscope::MockUserSettingAwareInterface>>()),
              settings(mockUserSettingProvider), options(std::make_shared<MissedApproachOptions>()),
              handler(options, collection, settings)
        {
            collection.RegisterHandler(mockUserSettingAware);
        }

        UserSettingAwareCollection collection;
        std::shared_ptr<NiceMock<Euroscope::MockUserSettingAwareInterface>> mockUserSettingAware;
        NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
        UserSetting settings;
        std::shared_ptr<MissedApproachOptions> options;
        MissedApproachUserSettingHandler handler;
    };

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingSettingsSetsDefaultServiceProvisionsIfNoSetting)
    {
        options->SetServiceProvisions(ServiceType::FinalApproach);
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachServiceProvision")).Times(1).WillOnce(Return(""));

        handler.UserSettingsUpdated(settings);
        EXPECT_EQ(ServiceType::Invalid, options->ServiceProvisions());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingSettingSetsDefaultServiceProvisionsIfInvalid)
    {
        options->SetServiceProvisions(ServiceType::FinalApproach);
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachServiceProvision")).Times(1).WillOnce(Return("66"));

        handler.UserSettingsUpdated(settings);
        EXPECT_EQ(ServiceType::Invalid, options->ServiceProvisions());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingSettingSetsServiceProvisions)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachServiceProvision")).Times(1).WillOnce(Return("8"));

        handler.UserSettingsUpdated(settings);
        EXPECT_EQ(ServiceType::FinalApproach, options->ServiceProvisions());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingSettingsSetsDefaultCurrentUserIfNoSetting)
    {
        options->SetAudioAlertForCurrentUser(true);
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachAudioCurrentUser")).Times(1).WillOnce(Return(""));

        handler.UserSettingsUpdated(settings);
        EXPECT_FALSE(options->AudioAlertForCurrentUser());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingSettingSetsDefaultCurrentUsersIfInvalid)
    {
        options->SetAudioAlertForCurrentUser(true);
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachAudioCurrentUser")).Times(1).WillOnce(Return("66"));

        handler.UserSettingsUpdated(settings);
        EXPECT_FALSE(options->AudioAlertForCurrentUser());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingSettingSetsCurrentUser)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachAudioCurrentUser")).Times(1).WillOnce(Return("1"));

        handler.UserSettingsUpdated(settings);
        EXPECT_TRUE(options->AudioAlertForCurrentUser());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingSettingsSetsDefaultAudioAlertIfNoSetting)
    {
        options->SetAudioAlertForCurrentUser(true);
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachAudio")).Times(1).WillOnce(Return(""));

        handler.UserSettingsUpdated(settings);
        EXPECT_FALSE(options->AudioAlertForCurrentUser());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingSettingSetsDefaultAudioAlertIfInvalid)
    {
        options->SetAudioAlertForCurrentUser(true);
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachAudio")).Times(1).WillOnce(Return("66"));

        handler.UserSettingsUpdated(settings);
        EXPECT_FALSE(options->AudioAlertForCurrentUser());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingSettingSetsAudioAlert)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachAudio")).Times(1).WillOnce(Return("1"));

        handler.UserSettingsUpdated(settings);
        EXPECT_TRUE(options->AudioAlert());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingAsrSetsDefaulAirfieldsIfNoSetting)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachAirfields")).Times(1).WillOnce(Return(""));

        handler.UserSettingsUpdated(settings);
        EXPECT_EQ(std::vector<std::string>{}, options->Airfields());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingAsrSetsAirfields)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachAirfields")).Times(1).WillOnce(Return("EGGD;EGLL"));

        handler.UserSettingsUpdated(settings);
        EXPECT_EQ(std::vector<std::string>({"EGGD", "EGLL"}), options->Airfields());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, ItSetsAudioAlert)
    {
        EXPECT_CALL(mockUserSettingProvider, SetKey("missedApproachAudio", "Play Missed Approach Alarm", "1")).Times(1);

        EXPECT_CALL(*mockUserSettingAware, UserSettingsUpdated(testing::_)).Times(1);

        handler.SetAudioAlert(true);

        EXPECT_TRUE(options->AudioAlert());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, ItSetsAudioAlertCurrentUser)
    {
        EXPECT_CALL(
            mockUserSettingProvider,
            SetKey("missedApproachAudioCurrentUser", "Play Missed Approach Alarm If User Initiated", "1"))
            .Times(1);

        EXPECT_CALL(*mockUserSettingAware, UserSettingsUpdated(testing::_)).Times(1);

        handler.SetAudioAlertForCurrentUser(true);

        EXPECT_TRUE(options->AudioAlertForCurrentUser());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, ItSetsAudioServiceProvisions)
    {
        EXPECT_CALL(
            mockUserSettingProvider,
            SetKey("missedApproachServiceProvision", "Missed Approach Alerts Service Provision", "8"))
            .Times(1);

        EXPECT_CALL(*mockUserSettingAware, UserSettingsUpdated(testing::_)).Times(1);

        handler.SetServiceProvisions(ServiceType::FinalApproach);

        EXPECT_EQ(ServiceType::FinalApproach, options->ServiceProvisions());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, ItSetsAirfields)
    {
        EXPECT_CALL(
            mockUserSettingProvider, SetKey("missedApproachAirfields", "Missed Approach Alert Airfields", "EGGD;EGLL"))
            .Times(1);

        EXPECT_CALL(*mockUserSettingAware, UserSettingsUpdated(testing::_)).Times(1);

        handler.SetAirfields(std::vector<std::string>({"EGGD", "EGLL"}));

        EXPECT_EQ(std::vector<std::string>({"EGGD", "EGLL"}), options->Airfields());
    }
} // namespace UKControllerPluginTest::MissedApproach
