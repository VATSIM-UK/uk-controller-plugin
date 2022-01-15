#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Plugin {
        class FunctionCallEventHandler;
    } // namespace Plugin
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Aircraft {
    class CallsignSelectionProviderInterface;
    class CallsignSelectionList;
    
    class CallsignSelectionListFactory
    {
        public:
        CallsignSelectionListFactory(Plugin::FunctionCallEventHandler& functionHandler, Euroscope::EuroscopePluginLoopbackInterface& plugin);
        [[nodiscard]] auto Create(
            std::shared_ptr<CallsignSelectionProviderInterface> provider,
            const std::string& description) -> std::shared_ptr<CallsignSelectionList>;
        
        private:
        
        // For registering the callback function
        Plugin::FunctionCallEventHandler& functionHandler;
        
        // To pass to the list
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };
} // namespace UKControllerPlugin::Aircraft
