#pragma once
#include "hold/HoldDisplayV2.h"
#include "navaids/NavaidCollection.h"

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
        class HoldDisplayFactoryV2
        {
            public:
                HoldDisplayFactoryV2(
                    const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    UKControllerPlugin::Hold::HoldManager & holdManager,
                    const UKControllerPlugin::Navaids::NavaidCollection& navaids
                );
                std::unique_ptr<UKControllerPlugin::Hold::HoldDisplayV2> Create(std::string navaid) const;

            private:

                // The plugin instance
                const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // The hold manager
                UKControllerPlugin::Hold::HoldManager & holdManager;

                // All the navaids for holding
                const UKControllerPlugin::Navaids::NavaidCollection& navaids;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
