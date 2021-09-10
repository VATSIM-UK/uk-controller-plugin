#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope
    namespace Hold {
        class HoldManager;
        class HoldDisplayManager;
    } // namespace Hold
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Hold {
    class HoldSelectionMenu
    {
        public:
        HoldSelectionMenu(
            UKControllerPlugin::Hold::HoldManager& holdManager,
            UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin,
            unsigned int callbackId);
        void AddDisplayManager(std::shared_ptr<const UKControllerPlugin::Hold::HoldDisplayManager> manager);
        [[nodiscard]] auto CountDisplayManagers() const -> size_t;
        void DisplayMenu(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            std::string context,
            const POINT& mousePos);
        void MenuItemClicked(int functionId, std::string context);

        private:
        // A set of all the open ASR's display managers.
        std::set<std::shared_ptr<const UKControllerPlugin::Hold::HoldDisplayManager>> displays;

        // The hold manager
        UKControllerPlugin::Hold::HoldManager& holdManager;

        // The plugin, used to display menus
        UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The callback id of the first hold
        const unsigned int callbackId;

        const std::string noHold = "--";
    };
} // namespace UKControllerPlugin::Hold
