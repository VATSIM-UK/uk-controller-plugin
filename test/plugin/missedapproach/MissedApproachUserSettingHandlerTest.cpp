#include "euroscope/UserSetting.h"
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
        options->SetAudioAlertServiceProvisions(ServiceType::FinalApproach);
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachAudioServiceProvision"))
            .Times(1)
            .WillOnce(Return(""));

        handler.UserSettingsUpdated(settings);
        EXPECT_EQ(ServiceType::Invalid, options->AudioAlertServiceProvisions());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingSettingSetsDefaultServiceProvisionsIfInvalid)
    {
        options->SetAudioAlertServiceProvisions(ServiceType::FinalApproach);
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachAudioServiceProvision"))
            .Times(1)
            .WillOnce(Return("66"));

        handler.UserSettingsUpdated(settings);
        EXPECT_EQ(ServiceType::Invalid, options->AudioAlertServiceProvisions());
    }

    TEST_F(MissedApproachUserSettingHandlerTest, LoadingSettingSetsServiceProvisions)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachAudioServiceProvision"))
            .Times(1)
            .WillOnce(Return("8"));

        handler.UserSettingsUpdated(settings);
        EXPECT_EQ(ServiceType::FinalApproach, options->AudioAlertServiceProvisions());
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
            SetKey("missedApproachAudioServiceProvision", "Missed Approach Audio For Service Provision", "8"))
            .Times(1);

        EXPECT_CALL(*mockUserSettingAware, UserSettingsUpdated(testing::_)).Times(1);

        handler.SetAudioAlertServiceProvisions(ServiceType::FinalApproach);

        EXPECT_EQ(ServiceType::FinalApproach, options->AudioAlertServiceProvisions());
    }
} // namespace UKControllerPluginTest::MissedApproach
