#include "pch/stdafx.h"
#include "plugin/PluginInformationBootstrap.h"
#include "plugin/PluginInformationMessage.h"
#include "euroscope/CallbackFunction.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Euroscope::CallbackFunction;

namespace UKControllerPlugin {
    namespace Plugin {

        void BootstrapPluginInformationMessage(
            const PersistenceContainer& container,
            ConfigurableDisplayCollection& displays
        ) {
            // Create the plugin information message box
            int toggleCallbackFunction = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            std::shared_ptr<PluginInformationMessage> infoMessage = std::make_shared<PluginInformationMessage>(
                *container.windows,
                toggleCallbackFunction
            );

            // Create callback
            CallbackFunction configureCallback(
                toggleCallbackFunction,
                "Plugin Information Message",
                std::bind(
                    &PluginInformationMessage::Configure,
                    infoMessage,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );

            // Register with handlers
            container.pluginFunctionHandlers->RegisterFunctionCall(configureCallback);
            container.commandHandlers->RegisterHandler(infoMessage);
            displays.RegisterDisplay(infoMessage);
        }
    }  // namespace Plugin
}  // namespace UKControllerPlugin
