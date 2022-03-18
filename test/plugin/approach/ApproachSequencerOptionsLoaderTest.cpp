#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "approach/AirfieldApproachOptions.h"
#include "approach/ApproachSequencerOptions.h"
#include "approach/ApproachSequencerOptionsLoader.h"
#include "controller/ControllerPositionHierarchy.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Approach::ApproachSequencerOptions;
using UKControllerPlugin::Approach::ApproachSequencerOptionsLoader;
using UKControllerPlugin::Approach::ApproachSequencingMode;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerOptionsLoaderTest : public testing::Test
    {
        public:
        ApproachSequencerOptionsLoaderTest() : userSetting(settingProvider), loader(options, airfields)
        {
            airfields.AddAirfield(std::make_shared<AirfieldModel>(1, "EGKK", nullptr));
            airfields.AddAirfield(std::make_shared<AirfieldModel>(2, "EGLL", nullptr));
        }

        testing::NiceMock<Euroscope::MockUserSettingProviderInterface> settingProvider;
        UserSetting userSetting;
        AirfieldCollection airfields;
        ApproachSequencerOptions options;
        ApproachSequencerOptionsLoader loader;
    };

    TEST_F(ApproachSequencerOptionsLoaderTest, ItLoadsSettings)
    {
        EXPECT_CALL(settingProvider, GetKey("approachSequencerModeEGKK")).Times(2).WillRepeatedly(testing::Return("1"));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerTargetEGKK")).Times(1).WillOnce(testing::Return("4.5"));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerSepEGKK")).Times(1).WillOnce(testing::Return("6.5"));

        EXPECT_CALL(settingProvider, GetKey("approachSequencerModeEGLL")).Times(1).WillOnce(testing::Return(""));

        loader.LoadSettings(userSetting);

        EXPECT_EQ(1, options.CountAirfieldOptions());
        auto& optionGatwick = options.Get("EGKK");
        EXPECT_EQ(ApproachSequencingMode::MinimumDistance, optionGatwick.defaultMode);
        EXPECT_DOUBLE_EQ(4.5, optionGatwick.targetDistance);
        EXPECT_DOUBLE_EQ(6.5, optionGatwick.minimumSeparationRequirement);
        EXPECT_EQ(1, options.CountAirfieldOptions());
    }

    TEST_F(ApproachSequencerOptionsLoaderTest, ItLoadsTheModeToWakeIfNotValidValue)
    {
        EXPECT_CALL(settingProvider, GetKey("approachSequencerModeEGKK")).Times(2).WillRepeatedly(testing::Return("66"));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerTargetEGKK")).Times(1).WillOnce(testing::Return("4.5"));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerSepEGKK")).Times(1).WillOnce(testing::Return("6.5"));

        EXPECT_CALL(settingProvider, GetKey("approachSequencerModeEGLL")).Times(1).WillOnce(testing::Return(""));

        loader.LoadSettings(userSetting);

        EXPECT_EQ(1, options.CountAirfieldOptions());
        auto& optionGatwick = options.Get("EGKK");
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, optionGatwick.defaultMode);
        EXPECT_DOUBLE_EQ(4.5, optionGatwick.targetDistance);
        EXPECT_DOUBLE_EQ(6.5, optionGatwick.minimumSeparationRequirement);
        EXPECT_EQ(1, options.CountAirfieldOptions());
    }

    TEST_F(ApproachSequencerOptionsLoaderTest, ItLoadsDefaultSettingsForTargetAndSep)
    {
        EXPECT_CALL(settingProvider, GetKey("approachSequencerModeEGKK")).Times(2).WillRepeatedly(testing::Return("1"));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerTargetEGKK")).Times(1).WillOnce(testing::Return(""));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerSepEGKK")).Times(1).WillOnce(testing::Return(""));

        EXPECT_CALL(settingProvider, GetKey("approachSequencerModeEGLL")).Times(1).WillOnce(testing::Return(""));

        loader.LoadSettings(userSetting);

        EXPECT_EQ(1, options.CountAirfieldOptions());
        auto& optionGatwick = options.Get("EGKK");
        EXPECT_EQ(ApproachSequencingMode::MinimumDistance, optionGatwick.defaultMode);
        EXPECT_DOUBLE_EQ(6.0, optionGatwick.targetDistance);
        EXPECT_DOUBLE_EQ(3.0, optionGatwick.minimumSeparationRequirement);
        EXPECT_EQ(1, options.CountAirfieldOptions());
    }

    TEST_F(ApproachSequencerOptionsLoaderTest, ItSavesSettings)
    {
        static_cast<void>(options.Get("EGBB"));

        EXPECT_CALL(settingProvider, SetKey("approachSequencerModeEGBB", "EGBB Approach Sequencer Default Mode", "0"))
            .Times(1);

        EXPECT_CALL(
            settingProvider,
            SetKey("approachSequencerTargetEGBB", "EGBB Approach Sequencer Default Target Distance", "6.000000"))
            .Times(1);

        EXPECT_CALL(
            settingProvider,
            SetKey("approachSequencerSepEGBB", "EGBB Approach Sequencer Default Minimum Separation", "3.000000"))
            .Times(1);

        loader.SaveSettings(userSetting);
    }
} // namespace UKControllerPluginTest::Approach
