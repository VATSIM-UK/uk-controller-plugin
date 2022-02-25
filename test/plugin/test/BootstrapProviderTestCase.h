#pragma once
#include "bootstrap/BootstrapProviderInterface.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "radarscreen/RadarRenderableCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

namespace UKControllerPluginTest {
    class BootstrapProviderTestCase : public testing::Test
    {
        public:
        void RunBootstrapPlugin(UKControllerPlugin::Bootstrap::BootstrapProviderInterface& provider)
        {
            provider.BootstrapPlugin(container);
        }

        void RunBootstrapRadarScreen(UKControllerPlugin::Bootstrap::BootstrapProviderInterface& provider)
        {
            provider.BootstrapRadarScreen(container, renderers, configurableDisplays, asrHandlers);
        }

        PersistenceContainer container;
        AsrEventHandlerCollection asrHandlers;
        RadarRenderableCollection renderers;
        ConfigurableDisplayCollection configurableDisplays;
    };
} // namespace UKControllerPluginTest
