#pragma once
#include "hold/HoldDisplay.h"

namespace UKControllerPlugin {
    namespace Aircraft {
        class CallsignSelectionListFactory;
    } // namespace Aircraft
    namespace Hold {
        class HoldManager;
        class PublishedHoldCollection;
    } // namespace Hold
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Navaids {
        class NavaidCollection;
    } // namespace Navaids
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Creates HoldDisplay instances for use by the HoldRenderer
        */
        class HoldDisplayFactory
        {
            public:
            HoldDisplayFactory(
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin,
                UKControllerPlugin::Hold::HoldManager& holdManager,
                const UKControllerPlugin::Navaids::NavaidCollection& navaids,
                const UKControllerPlugin::Hold::PublishedHoldCollection& holds,
                const UKControllerPlugin::Dialog::DialogManager& dialogManager,
                const Aircraft::CallsignSelectionListFactory& addAircraftListFactory);
            std::unique_ptr<UKControllerPlugin::Hold::HoldDisplay> Create(std::string navaid) const;

            private:
            // The plugin instance
            UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin;

            // The hold manager
            UKControllerPlugin::Hold::HoldManager& holdManager;

            // All the navaids for holding
            const UKControllerPlugin::Navaids::NavaidCollection& navaids;

            // Published holds
            const UKControllerPlugin::Hold::PublishedHoldCollection& holds;

            // Dialog manager
            const UKControllerPlugin::Dialog::DialogManager& dialogManager;

            // For creating the callsign selection lists
            const Aircraft::CallsignSelectionListFactory& addAircraftListFactory;
        };
    } // namespace Hold
} // namespace UKControllerPlugin
