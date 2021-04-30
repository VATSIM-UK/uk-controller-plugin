#include "pch/stdafx.h"
#include "plugin/PluginInformationBootstrap.h"
#include "plugin/PluginInformationMessage.h"
#include "plugin/PluginHelpPage.h"
#include "plugin/PluginChangelog.h"
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
            // Create the plugin help page toggle
            int helpCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            std::shared_ptr<PluginHelpPage> helpPage = std::make_shared<PluginHelpPage>(
                *container.windows,
                helpCallbackId
                );

            // Create callback
            CallbackFunction helpCallback(
                helpCallbackId,
                "Plugin Help Page",
                std::bind(
                &PluginHelpPage::Configure,
                helpPage,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3
            )
            );

            // Register with handlers
            container.pluginFunctionHandlers->RegisterFunctionCall(helpCallback);
            container.commandHandlers->RegisterHandler(helpPage);
            displays.RegisterDisplay(helpPage);

            // Create the plugin help page toggle
            int changelogCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            std::shared_ptr<PluginChangelog> changeLog = std::make_shared<PluginChangelog>(
                *container.windows,
                changelogCallbackId
            );

            // Create callback
            CallbackFunction changelogCallback(
                changelogCallbackId,
                "Plugin Changelog",
                std::bind(
                    &PluginChangelog::Configure,
                    changeLog,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );

            // Register with handlers
            container.pluginFunctionHandlers->RegisterFunctionCall(changelogCallback);
            container.commandHandlers->RegisterHandler(changeLog);
            displays.RegisterDisplay(changeLog);

            // Create the plugin information message box
            int informationCallback = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            std::shared_ptr<PluginInformationMessage> infoMessage = std::make_shared<PluginInformationMessage>(
                *container.windows,
                informationCallback
            );

            // Create callback
            CallbackFunction configureCallback(
                informationCallback,
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
