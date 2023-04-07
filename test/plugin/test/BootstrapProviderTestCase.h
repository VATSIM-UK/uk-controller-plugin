#pragma once
#include "bootstrap/BootstrapProviderInterface.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/MenuToggleableDisplayFactory.h"
#include "radarscreen/RadarRenderableCollection.h"

namespace UKControllerPluginTest {
    class BootstrapProviderTestCase : public testing::Test
    {
        public:
        BootstrapProviderTestCase();
        void RunBootstrapPlugin(UKControllerPlugin::Bootstrap::BootstrapProviderInterface& provider);
        void RunBootstrapRadarScreen(UKControllerPlugin::Bootstrap::BootstrapProviderInterface& provider);

        PersistenceContainer container;
        AsrEventHandlerCollection asrHandlers;
        RadarRenderableCollection renderers;
        ConfigurableDisplayCollection configurableDisplays;
        MenuToggleableDisplayFactory displayFactory;

        private:
        [[nodiscard]] auto MakeContainer() -> PersistenceContainer;
    };
} // namespace UKControllerPluginTest
