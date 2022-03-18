#include "approach/ApproachSequencerDisplayAsrLoader.h"
#include "approach/ApproachSequencerDisplayOptions.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Approach::ApproachSequencerDisplayAsrLoader;
using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerDisplayAsrLoaderTest : public testing::Test
    {
        public:
        ApproachSequencerDisplayAsrLoaderTest()
            : settings(settingProvider), options(std::make_shared<ApproachSequencerDisplayOptions>()), loader(options)
        {
        }

        testing::NiceMock<Euroscope::MockUserSettingProviderInterface> settingProvider;
        UserSetting settings;
        std::shared_ptr<ApproachSequencerDisplayOptions> options;
        ApproachSequencerDisplayAsrLoader loader;
    };

    TEST_F(ApproachSequencerDisplayAsrLoaderTest, AsrLoadingLoadsPosition)
    {
        EXPECT_CALL(settingProvider, GetKey(testing::_)).Times(3).WillRepeatedly(testing::Return(""));

        EXPECT_CALL(settingProvider, GetKey("approachSequencerXPosition")).Times(1).WillOnce(testing::Return("250"));

        EXPECT_CALL(settingProvider, GetKey("approachSequencerYPosition")).Times(1).WillOnce(testing::Return("150"));

        loader.AsrLoadedEvent(settings);
        EXPECT_EQ(250, options->Position().x);
        EXPECT_EQ(150, options->Position().y);
    }

    TEST_F(ApproachSequencerDisplayAsrLoaderTest, AsrLoadingLoadsDefaultPosition)
    {
        EXPECT_CALL(settingProvider, GetKey(testing::_)).Times(3).WillRepeatedly(testing::Return(""));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerXPosition")).Times(1).WillOnce(testing::Return(""));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerYPosition")).Times(1).WillOnce(testing::Return(""));

        loader.AsrLoadedEvent(settings);
        EXPECT_EQ(200, options->Position().x);
        EXPECT_EQ(200, options->Position().y);
    }

    TEST_F(ApproachSequencerDisplayAsrLoaderTest, AsrLoadingLoadsVisible)
    {
        EXPECT_CALL(settingProvider, GetKey(testing::_)).Times(4).WillRepeatedly(testing::Return(""));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerVisible")).Times(1).WillOnce(testing::Return("1"));

        loader.AsrLoadedEvent(settings);
        EXPECT_TRUE(options->IsVisible());
    }

    TEST_F(ApproachSequencerDisplayAsrLoaderTest, AsrLoadingLoadsDefaultVisibility)
    {
        EXPECT_CALL(settingProvider, GetKey(testing::_)).Times(4).WillRepeatedly(testing::Return(""));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerVisible")).Times(1).WillOnce(testing::Return(""));

        loader.AsrLoadedEvent(settings);
        EXPECT_FALSE(options->IsVisible());
    }

    TEST_F(ApproachSequencerDisplayAsrLoaderTest, AsrLoadingLoadsContentCollapsed)
    {
        EXPECT_CALL(settingProvider, GetKey(testing::_)).Times(4).WillRepeatedly(testing::Return(""));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerContentCollapsed"))
            .Times(1)
            .WillOnce(testing::Return("1"));

        loader.AsrLoadedEvent(settings);
        EXPECT_TRUE(options->ContentCollapsed());
    }

    TEST_F(ApproachSequencerDisplayAsrLoaderTest, AsrLoadingLoadsDefaultContentCollapsed)
    {
        EXPECT_CALL(settingProvider, GetKey(testing::_)).Times(4).WillRepeatedly(testing::Return(""));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerContentCollapsed"))
            .Times(1)
            .WillOnce(testing::Return(""));

        loader.AsrLoadedEvent(settings);
        EXPECT_FALSE(options->ContentCollapsed());
    }

    TEST_F(ApproachSequencerDisplayAsrLoaderTest, AsrLoadingLoadsAirfield)
    {
        EXPECT_CALL(settingProvider, GetKey(testing::_)).Times(4).WillRepeatedly(testing::Return(""));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerAirfield")).Times(1).WillOnce(testing::Return("EGBB"));

        loader.AsrLoadedEvent(settings);
        EXPECT_EQ("EGBB", options->Airfield());
    }

    TEST_F(ApproachSequencerDisplayAsrLoaderTest, AsrLoadingLoadsDefaultAirfield)
    {
        EXPECT_CALL(settingProvider, GetKey(testing::_)).Times(4).WillRepeatedly(testing::Return(""));
        EXPECT_CALL(settingProvider, GetKey("approachSequencerAirfield")).Times(1).WillOnce(testing::Return(""));

        loader.AsrLoadedEvent(settings);
        EXPECT_TRUE(options->Airfield().empty());
    }

    TEST_F(ApproachSequencerDisplayAsrLoaderTest, AsrClosingSavesFields)
    {
        options->Airfield("EGBB");
        options->SetVisible(true);
        options->SetCollapsed(true);
        options->Position({350, 450});

        EXPECT_CALL(settingProvider, SetKey("approachSequencerXPosition", "Approach Sequencer X Position", "350"))
            .Times(1);
        EXPECT_CALL(settingProvider, SetKey("approachSequencerYPosition", "Approach Sequencer Y Position", "450"))
            .Times(1);
        EXPECT_CALL(settingProvider, SetKey("approachSequencerVisible", "Approach Sequencer Visibility", "1")).Times(1);
        EXPECT_CALL(
            settingProvider,
            SetKey(
                "approachSequencerContentCollapsed",
                "Approach Sequencer Content "
                "Collapsed",
                "1"))
            .Times(1);
        EXPECT_CALL(settingProvider, SetKey("approachSequencerAirfield", "Approach Sequencer Airfield", "EGBB"))
            .Times(1);

        loader.AsrClosingEvent(settings);
    }
} // namespace UKControllerPluginTest::Approach
