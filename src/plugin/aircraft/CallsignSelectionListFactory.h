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

    [[nodiscard]] auto Create(
        std::shared_ptr<CallsignSelectionProviderInterface> provider,
        Plugin::FunctionCallEventHandler& functionHandler,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        const std::string& description) -> std::shared_ptr<CallsignSelectionList>;
} // namespace UKControllerPlugin::Aircraft
