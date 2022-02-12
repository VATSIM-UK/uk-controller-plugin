#pragma once

namespace UKControllerPlugin {
    namespace Aircraft {
        class CallsignSelectionListFactory;
    } // namespace Aircraft
    namespace Dialog {
        class DialogManager;
    } // namespace Dialog
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Navaids {
        class NavaidCollection;
    } // namespace Navaids
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {
        class HoldDisplay;
        class HoldManager;
        class PublishedHoldCollection;

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
            [[nodiscard]] auto Create(std::string navaid) const
                -> std::unique_ptr<UKControllerPlugin::Hold::HoldDisplay>;

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
