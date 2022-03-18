#include "airfield/AirfieldCollection.h"
#include "approach/ApproachBootstrapProvider.h"
#include "euroscope/PluginSettingsProviderCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "wake/WakeCategoryMapperCollection.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Approach::ApproachBootstrapProvider;
using UKControllerPlugin::Euroscope::PluginSettingsProviderCollection;
using UKControllerPlugin::Wake::WakeCategoryMapperCollection;

namespace UKControllerPluginTest::Approach {
    class ApproachBootstrapProviderTest : public BootstrapProviderTestCase
    {
        public:
        ApproachBootstrapProviderTest()
        {
            container.plugin = std::make_unique<testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface>>();
            container.airfields = std::make_unique<AirfieldCollection>();
            container.wakeCategoryMappers = std::make_unique<WakeCategoryMapperCollection>();
            container.pluginSettingsProviders =
                std::make_unique<PluginSettingsProviderCollection>(*container.pluginUserSettingHandler);
        }

        ApproachBootstrapProvider provider;
    };

    TEST_F(ApproachBootstrapProviderTest, ItRegistersTheOptionsLoader)
    {
        this->RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.pluginSettingsProviders->Count());
    }

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

    TEST_F(ApproachBootstrapProviderTest, ItRegistersCallsignPopupList)
    {
        this->RunBootstrapRadarScreen(provider);
        EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("Toggle sequencer callsign selector"));
    }

    TEST_F(ApproachBootstrapProviderTest, ItRegistersTargetPopupList)
    {
        this->RunBootstrapRadarScreen(provider);
        EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("Toggle sequencer target selector"));
    }
} // namespace UKControllerPluginTest::Approach
