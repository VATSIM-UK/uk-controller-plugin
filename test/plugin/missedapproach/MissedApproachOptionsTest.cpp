#include "missedapproach/MissedApproachOptions.h"

using UKControllerPlugin::MissedApproach::MissedApproachOptions;
using UKControllerPlugin::Ownership::ServiceType;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachOptionsTest : public testing::Test
    {
        public:
        MissedApproachOptions options;
    };

    TEST_F(MissedApproachOptionsTest, ItHasDefaultAudioAlert)
    {
        EXPECT_FALSE(options.AudioAlert());
    }

    TEST_F(MissedApproachOptionsTest, ItHasDefaultAudioAlertCurrentUser)
    {
        EXPECT_FALSE(options.AudioAlertForCurrentUser());
    }

    TEST_F(MissedApproachOptionsTest, ItHasDefaultAudioAlertServiceProvisions)
    {
        EXPECT_EQ(ServiceType::Invalid, options.ServiceProvisions());
    }

    TEST_F(MissedApproachOptionsTest, ItHasDefaultAirfields)
    {
        EXPECT_TRUE(options.Airfields().empty());
    }

    TEST_F(MissedApproachOptionsTest, ItSetsAudioAlert)
    {
        options.SetAudioAlert(true);
        EXPECT_TRUE(options.AudioAlert());
    }

    TEST_F(MissedApproachOptionsTest, ItSetsAudioAlertCurrentUser)
    {
        options.SetAudioAlertForCurrentUser(true);
        EXPECT_TRUE(options.AudioAlertForCurrentUser());
    }

    TEST_F(MissedApproachOptionsTest, ItSetsAudioAlertServiceProvisions)
    {
        options.SetServiceProvisions(ServiceType::FinalApproach);
        EXPECT_EQ(ServiceType::FinalApproach, options.ServiceProvisions());
    }

    TEST_F(MissedApproachOptionsTest, ItSetsAirfields)
    {
        std::vector<std::string> expected({"EGGD", "EGFF"});
        options.SetAirfields(expected);
        EXPECT_EQ(expected, options.Airfields());
    }
} // namespace UKControllerPluginTest::MissedApproach
