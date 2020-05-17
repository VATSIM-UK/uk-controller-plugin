#pragma once
#include "hold/HoldDisplay.h"
#include "hold/PublishedHoldCollection.h"
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
        class HoldDisplayFactory
        {
            public:
                HoldDisplayFactory(
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    UKControllerPlugin::Hold::HoldManager & holdManager,
                    const UKControllerPlugin::Navaids::NavaidCollection& navaids,
                    const UKControllerPlugin::Hold::PublishedHoldCollection& holds
                );
                std::unique_ptr<UKControllerPlugin::Hold::HoldDisplay> Create(std::string navaid) const;

            private:

                // The plugin instance
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // The hold manager
                UKControllerPlugin::Hold::HoldManager & holdManager;

                // All the navaids for holding
                const UKControllerPlugin::Navaids::NavaidCollection& navaids;

                // Published holds
                const UKControllerPlugin::Hold::PublishedHoldCollection& holds;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
