#pragma once
#include "bootstrap/BootstrapProviderInterface.h"

namespace UKControllerPlugin {
    namespace Windows {
        struct GdiplusBrushes;
    } // namespace Windows
}

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

        private:
        const int GLIDESLOPE_DEVIATION_TAG_ITEM_ID = 132;
    };
} // namespace UKControllerPlugin::Approach
