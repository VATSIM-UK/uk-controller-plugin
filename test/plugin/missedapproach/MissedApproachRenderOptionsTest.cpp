#include "euroscope/UserSetting.h"
#include "missedapproach/MissedApproachRenderOptions.h"
#include "ownership/ServiceType.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::MissedApproach::MissedApproachRenderMode;
using UKControllerPlugin::MissedApproach::MissedApproachRenderOptions;
using UKControllerPlugin::Ownership::ServiceType;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachRenderOptionsTest : public testing::Test
    {
        public:
        MissedApproachRenderOptionsTest() : settings(mockUserSettingProvider)
        {
        }
        NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
        UserSetting settings;
        MissedApproachRenderOptions options;
    };

    TEST_F(MissedApproachRenderOptionsTest, ItHasADefaultMode)
    {
        EXPECT_EQ(MissedApproachRenderMode::None, options.Mode());
    }

    TEST_F(MissedApproachRenderOptionsTest, ItHasADefaultServiceProvisions)
    {
        EXPECT_EQ(ServiceType::Invalid, options.ServiceProvisions());
    }

    TEST_F(MissedApproachRenderOptionsTest, ItHasADefaultDuration)
    {
        EXPECT_EQ(std::chrono::seconds(0), options.Duration());
    }

    TEST_F(MissedApproachRenderOptionsTest, ItHasADefaultAirfields)
    {
        EXPECT_EQ(std::vector<std::string>{}, options.Airfields());
    }

    TEST_F(MissedApproachRenderOptionsTest, LoadingAsrSetsDefaultRenderModeIfNoSetting)
    {
        options.SetMode(MissedApproachRenderMode::Line);
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachRenderMode")).Times(1).WillOnce(Return(""));

        options.AsrLoadedEvent(settings);
        EXPECT_EQ(MissedApproachRenderMode::None, options.Mode());
    }

    TEST_F(MissedApproachRenderOptionsTest, LoadingAsrSetsDefaultRenderModeIfInvalid)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachRenderMode")).Times(1).WillOnce(Return("66"));

        options.AsrLoadedEvent(settings);
        EXPECT_EQ(MissedApproachRenderMode::None, options.Mode());
    }

    TEST_F(MissedApproachRenderOptionsTest, LoadingAsrSetsRenderMode)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachRenderMode")).Times(1).WillOnce(Return("2"));

        options.AsrLoadedEvent(settings);
        EXPECT_EQ(MissedApproachRenderMode::Circle, options.Mode());
    }

    TEST_F(MissedApproachRenderOptionsTest, LoadingAsrSetsDefaultServiceProvisionsIfNoSetting)
    {
        options.SetServiceProvisions(ServiceType::FinalApproach);
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachRenderServiceProvisions"))
            .Times(1)
            .WillOnce(Return(""));

        options.AsrLoadedEvent(settings);
        EXPECT_EQ(ServiceType::Invalid, options.ServiceProvisions());
    }

    TEST_F(MissedApproachRenderOptionsTest, LoadingAsrSetsDefaultServiceProvisionsIfInvalid)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachRenderServiceProvisions"))
            .Times(1)
            .WillOnce(Return("66"));

        options.AsrLoadedEvent(settings);
        EXPECT_EQ(ServiceType::Invalid, options.ServiceProvisions());
    }

    TEST_F(MissedApproachRenderOptionsTest, LoadingAsrSetsServiceProvisions)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachRenderServiceProvisions"))
            .Times(1)
            .WillOnce(Return("8"));

        options.AsrLoadedEvent(settings);
        EXPECT_EQ(ServiceType::FinalApproach, options.ServiceProvisions());
    }

    TEST_F(MissedApproachRenderOptionsTest, LoadingAsrSetsDefaultDurationIfNoSetting)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachRenderDuration")).Times(1).WillOnce(Return(""));

        options.AsrLoadedEvent(settings);
        EXPECT_EQ(std::chrono::seconds(5), options.Duration());
    }

    TEST_F(MissedApproachRenderOptionsTest, LoadingAsrSetsDefaultDurationIfInvalid)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachRenderDuration")).Times(1).WillOnce(Return("abc"));

        options.AsrLoadedEvent(settings);
        EXPECT_EQ(std::chrono::seconds(5), options.Duration());
    }

    TEST_F(MissedApproachRenderOptionsTest, LoadingAsrSetsDuration)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachRenderDuration")).Times(1).WillOnce(Return("10"));

        options.AsrLoadedEvent(settings);
        EXPECT_EQ(std::chrono::seconds(10), options.Duration());
    }

    TEST_F(MissedApproachRenderOptionsTest, LoadingAsrSetsDefaulAirfieldsIfNoSetting)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachRenderAirfields")).Times(1).WillOnce(Return(""));

        options.AsrLoadedEvent(settings);
        EXPECT_EQ(std::vector<std::string>{}, options.Airfields());
    }

    TEST_F(MissedApproachRenderOptionsTest, LoadingAsrSetsAirfields)
    {
        EXPECT_CALL(mockUserSettingProvider, GetKey(_)).WillRepeatedly(Return(""));

        EXPECT_CALL(mockUserSettingProvider, GetKey("missedApproachRenderAirfields"))
            .Times(1)
            .WillOnce(Return("EGGD;EGLL"));

        options.AsrLoadedEvent(settings);
        EXPECT_EQ(std::vector<std::string>({"EGGD", "EGLL"}), options.Airfields());
    }
} // namespace UKControllerPluginTest::MissedApproach
