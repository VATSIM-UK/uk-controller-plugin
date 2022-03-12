#pragma once
#include "bootstrap/BootstrapProviderInterface.h"

namespace UKControllerPlugin::Approach {
    class ApproachBootstrapProvider : public Bootstrap::BootstrapProviderInterface
    {
        public:
        ApproachBootstrapProvider() = default;
        void BootstrapPlugin(Bootstrap::PersistenceContainer& container) override;

        void BootstrapRadarScreen(
            const Bootstrap::PersistenceContainer& container,
            RadarScreen::RadarRenderableCollection& radarRenderables,
            RadarScreen::ConfigurableDisplayCollection& configurables,
            Euroscope::AsrEventHandlerCollection& asrHandlers,
            const RadarScreen::MenuToggleableDisplayFactory& toggleableDisplayFactory) override;
    };
} // namespace UKControllerPlugin::Approach
