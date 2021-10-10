#include "missedapproach/MissedApproachOptions.h"

using UKControllerPlugin::MissedApproach::MissedApproachOptions;
using UKControllerPlugin::Ownership::ServiceType;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachOptionsTest : public testing::Test
    {
        public:
        MissedApproachOptionsTest()
        {
        }

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
        options.SetAudioAlertForCurrentUser(true);
        EXPECT_TRUE(options.AudioAlertForCurrentUser());
    }

    TEST_F(MissedApproachOptionsTest, ItSetsAudioAlertServiceProvisions)
    {
        options.SetAudioAlertServiceProvisions(ServiceType::FinalApproach);
        EXPECT_EQ(ServiceType::FinalApproach, options.AudioAlertServiceProvisions());
    }
} // namespace UKControllerPluginTest::MissedApproach
