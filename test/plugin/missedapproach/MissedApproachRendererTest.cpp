#include "missedapproach/MissedApproachCollection.h"
#include "missedapproach/MissedApproachRenderer.h"
#include "missedapproach/MissedApproachRenderOptions.h"
#include "ownership/AirfieldServiceProviderCollection.h"

using UKControllerPlugin::MissedApproach::MissedApproachCollection;
using UKControllerPlugin::MissedApproach::MissedApproachRenderer;
using UKControllerPlugin::MissedApproach::MissedApproachRenderMode;
using UKControllerPlugin::MissedApproach::MissedApproachRenderOptions;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachRendererTest : public testing::Test
    {
        public:
        MissedApproachRendererTest()
            : missedApproaches(std::make_shared<MissedApproachCollection>()),
              options(std::make_shared<MissedApproachRenderOptions>()),
              renderer(missedApproaches, serviceProviders, mockPlugin, options)
        {
        }

        AirfieldServiceProviderCollection serviceProviders;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        std::shared_ptr<MissedApproachCollection> missedApproaches;
        std::shared_ptr<MissedApproachRenderOptions> options;
        MissedApproachRenderer renderer;
    };

    TEST_F(MissedApproachRendererTest, ItRendersIfModeSetToCircle)
    {
        options->SetMode(MissedApproachRenderMode::Circle);
        EXPECT_TRUE(renderer.IsVisible());
    }

    TEST_F(MissedApproachRendererTest, ItRendersIfModeSetToLine)
    {
        options->SetMode(MissedApproachRenderMode::Line);
        EXPECT_TRUE(renderer.IsVisible());
    }

    TEST_F(MissedApproachRendererTest, ItRendersIfModeSetToCircleAndLine)
    {
        options->SetMode(MissedApproachRenderMode::Circle | MissedApproachRenderMode::Line);
        EXPECT_TRUE(renderer.IsVisible());
    }

    TEST_F(MissedApproachRendererTest, ItDoesntRenderIfModeSetToNone)
    {
        options->SetMode(MissedApproachRenderMode::None);
        EXPECT_FALSE(renderer.IsVisible());
    }
} // namespace UKControllerPluginTest::MissedApproach
