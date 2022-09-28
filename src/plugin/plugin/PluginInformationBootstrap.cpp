#include "ForceUpdate.h"
#include "FunctionCallEventHandler.h"
#include "OpenDataFolder.h"
#include "PluginChangelog.h"
#include "PluginHelpPage.h"
#include "PluginInformationBootstrap.h"
#include "PluginInformationMessage.h"
#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "euroscope/CallbackFunction.h"
#include "radarscreen/ConfigurableDisplayCallbackFactory.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;

namespace UKControllerPlugin::Plugin {

    void
    BootstrapPluginInformationMessage(const PersistenceContainer& container, ConfigurableDisplayCollection& displays)
    {
        // Create the plugin help page toggle
        int helpCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<PluginHelpPage> helpPage = std::make_shared<PluginHelpPage>(*container.windows, helpCallbackId);

        // Create callback
        CallbackFunction helpCallback(
            helpCallbackId, "Plugin Help Page", [helpPage](int functionId, std::string subject, RECT screenObjectArea) {
                helpPage->Configure(functionId, std::move(subject), screenObjectArea);
            });

        // Register with handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(helpCallback);
        container.commandHandlers->RegisterHandler(helpPage);
        displays.RegisterDisplay(helpPage);

        // Create the open data folder link
        int openDataFolderCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<OpenDataFolder> openDataFolder =
            std::make_shared<OpenDataFolder>(*container.windows, openDataFolderCallbackId);

        CallbackFunction openDataFolderCallback(
            openDataFolderCallbackId,
            "Open Data Folder",
            [openDataFolder](int functionId, std::string subject, RECT screenObjectArea) {
                openDataFolder->Configure(functionId, std::move(subject), screenObjectArea);
            });

        // Register with handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(RadarScreen::CreateConfigurableDisplayCallback(
            openDataFolderCallbackId, "Open Data Folder", openDataFolder));
        displays.RegisterDisplay(openDataFolder);

        // Create the force update toggle
        int forceUpdateCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<ForceUpdate> forceUpdate =
            std::make_shared<ForceUpdate>(*container.windows, forceUpdateCallbackId);

        // Create callback
        CallbackFunction forceUpdateCallback(
            forceUpdateCallbackId,
            "Plugin Force Update",
            [forceUpdate](int functionId, std::string subject, RECT screenObjectArea) {
                forceUpdate->Configure(functionId, std::move(subject), screenObjectArea);
            });

        // Register with handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(forceUpdateCallback);
        container.commandHandlers->RegisterHandler(forceUpdate);
        displays.RegisterDisplay(forceUpdate);

        // Create the plugin help page toggle
        int changelogCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<PluginChangelog> changeLog =
            std::make_shared<PluginChangelog>(*container.windows, changelogCallbackId);

        // Create callback
        CallbackFunction changelogCallback(
            changelogCallbackId,
            "Plugin Changelog",
            [changeLog](int functionId, std::string subject, RECT screenObjectArea) {
                changeLog->Configure(functionId, std::move(subject), screenObjectArea);
            });

        // Register with handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(changelogCallback);
        container.commandHandlers->RegisterHandler(changeLog);
        displays.RegisterDisplay(changeLog);

        // Create the plugin information message box
        int informationCallback = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<PluginInformationMessage> infoMessage =
            std::make_shared<PluginInformationMessage>(*container.windows, informationCallback);

        // Create callback
        CallbackFunction configureCallback(
            informationCallback,
            "Plugin Information Message",
            [infoMessage](int functionId, std::string subject, RECT screenObjectArea) {
                infoMessage->Configure(functionId, std::move(subject), screenObjectArea);
            });

        // Register with handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(configureCallback);
        container.commandHandlers->RegisterHandler(infoMessage);
        displays.RegisterDisplay(infoMessage);
    }
} // namespace UKControllerPlugin::Plugin
