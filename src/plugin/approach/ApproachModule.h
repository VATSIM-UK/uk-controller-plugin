#pragma once
#include "bootstrap/BootstrapProviderInterface.h"

namespace UKControllerPlugin::Approach {
    class ApproachModule : Bootstrap::BootstrapProviderInterface
    {
        public:
        void BootstrapPlugin(Bootstrap::PersistenceContainer& container) override;
        
        void BootstrapRadarScreen(
            const Bootstrap::PersistenceContainer& container,
            RadarScreen::RadarRenderableCollection& radarRenderables,
            RadarScreen::ConfigurableDisplayCollection& configurables,
            Euroscope::AsrEventHandlerCollection& asrHandlers) override;
    };
} // namespace UKControllerPlugin::Approach
