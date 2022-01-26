#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Plugin {
        class FunctionCallEventHandler;
    } // namespace Plugin
} // namespace UKControllerPlugin

namespace UKControllerPlugin::List {
    class PopupListInterface;
    class ListItemProviderInterface;

    /**
     * For building popup lists.
     */
    class PopupListFactory
    {
        public:
        PopupListFactory(
            Plugin::FunctionCallEventHandler& functionHandler, Euroscope::EuroscopePluginLoopbackInterface& plugin);
        [[nodiscard]] auto
        Create(std::shared_ptr<ListItemProviderInterface> provider, const std::string& description) const
            -> std::shared_ptr<PopupListInterface>;

        private:
        // For registering the callback function
        Plugin::FunctionCallEventHandler& functionHandler;

        // To pass to the list
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };
} // namespace UKControllerPlugin::List
