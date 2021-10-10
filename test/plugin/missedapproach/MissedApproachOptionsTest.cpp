#include "euroscope/UserSetting.h"
#include "missedapproach/MissedApproachOptions.h"

using ::testing::NiceMock;
using ::testing::Return;

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::MissedApproach::MissedApproachOptions;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachOptionsTest : public testing::Test
    {
        public:
        MissedApproachOptionsTest() : settings(mockUserSettingProvider), options(settings)
        {
        }
        NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
        UserSetting settings;
        MissedApproachOptions options;
    };

    TEST_F(MissedApproachOptionsTest, ItHasDefaultAudioAlertCurrentUser)
    {
        EXPECT_FALSE(options.AudioAlertForCurrentUser());
    }

    TEST_F(MissedApproachOptionsTest, ItHasDefaultAudioAlertServiceProvisions)
    {
        EXPECT_EQ(ServiceType::Invalid, options.AudioAlertServiceProvisions());
    }

    TEST_F(MissedApproachOptionsTest, ItSetsAudioAlertCurrentUser)
    {
        EXPECT_CALL(
            mockUserSettingProvider,
            SetKey("missedApproachAudioCurrentUser", "Play Missed Approach Alarm If User Initiated", "1"))
            .Times(1);

        options.SetAudioAlertForCurrentUser(true);

        EXPECT_TRUE(options.AudioAlertForCurrentUser());
    }

    TEST_F(MissedApproachOptionsTest, ItSetsAudioServiceProvisions)
    {
        EXPECT_CALL(
            mockUserSettingProvider,
            SetKey("missedApproachAudioServiceProvision", "Missed Approach Audio For Service Provision", "8"))
            .Times(1);

        options.SetAudioAlertServiceProvisions(ServiceType::FinalApproach);

        EXPECT_EQ(ServiceType::FinalApproach, options.AudioAlertServiceProvisions());
    }
} // namespace UKControllerPluginTest::MissedApproach
