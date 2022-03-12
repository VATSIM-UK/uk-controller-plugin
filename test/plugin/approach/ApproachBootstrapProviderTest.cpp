#include "approach/ApproachBootstrapProvider.h"
#include "plugin/FunctionCallEventHandler.h"

using UKControllerPlugin::Approach::ApproachBootstrapProvider;

namespace UKControllerPluginTest::Approach {
    class ApproachBootstrapProviderTest : public BootstrapProviderTestCase
    {
        public:
        ApproachBootstrapProvider provider;
    };

    TEST_F(ApproachBootstrapProviderTest, ItRegistersTheRenderers)
    {
        this->RunBootstrapRadarScreen(provider);
        EXPECT_EQ(2, renderers.CountRenderers());
        EXPECT_EQ(2, renderers.CountRenderersInPhase(renderers.beforeTags));
    }

    TEST_F(ApproachBootstrapProviderTest, ItRegistersAsrSettingsHandlers)
    {
        this->RunBootstrapRadarScreen(provider);
        EXPECT_EQ(1, asrHandlers.CountHandlers());
    }

    TEST_F(ApproachBootstrapProviderTest, ItRegistersMenuToggle)
    {
        this->RunBootstrapRadarScreen(provider);
        EXPECT_EQ(1, configurableDisplays.CountDisplays());
        EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("Approach sequencer display toggle"));
    }

    TEST_F(ApproachBootstrapProviderTest, ItRegistersPopupList)
    {
        this->RunBootstrapRadarScreen(provider);
        EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("Toggle sequencer airfield selector"));
    }
} // namespace UKControllerPluginTest::Approach
