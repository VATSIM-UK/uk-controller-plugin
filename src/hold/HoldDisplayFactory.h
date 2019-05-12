#pragma once
#include "hold/HoldDisplay.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldManager;
    }  // namespace Hold
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Creates HoldDisplay instances for use by the HoldRenderer
        */
        class HoldDisplayFactory
        {
            public:
                HoldDisplayFactory(
                    const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    UKControllerPlugin::Hold::HoldManager & holdManager
                );
                std::unique_ptr<UKControllerPlugin::Hold::HoldDisplay> Create(unsigned int holdId) const;

            private:

                // The plugin instance
                const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // The hold manager
                UKControllerPlugin::Hold::HoldManager & holdManager;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
