#include "ForceUpdate.h"
#include "FunctionCallEventHandler.h"
#include "OpenDataFolder.h"
#include "OpenLogsFolder.h"
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

        // Register with handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(
            RadarScreen::CreateConfigurableDisplayCallback(helpCallbackId, "Plugin Help Page", helpPage));
        container.commandHandlers->RegisterHandler(helpPage);
        displays.RegisterDisplay(helpPage);

        // Create the open data folder link
        int openDataFolderCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<OpenDataFolder> openDataFolder =
            std::make_shared<OpenDataFolder>(*container.windows, openDataFolderCallbackId);

        // Register with handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(RadarScreen::CreateConfigurableDisplayCallback(
            openDataFolderCallbackId, "Open Data Folder", openDataFolder));
        displays.RegisterDisplay(openDataFolder);

        // Create the open logs folder link
        int openLogsFolderCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<OpenLogsFolder> openLogsFolder =
            std::make_shared<OpenLogsFolder>(*container.windows, openLogsFolderCallbackId);

        // Register with handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(RadarScreen::CreateConfigurableDisplayCallback(
            openLogsFolderCallbackId, "Open Logs Folder", openLogsFolder));
        displays.RegisterDisplay(openLogsFolder);

        // Create the force update toggle
        int forceUpdateCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<ForceUpdate> forceUpdate =
            std::make_shared<ForceUpdate>(*container.windows, forceUpdateCallbackId);

        // Register with handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(
            RadarScreen::CreateConfigurableDisplayCallback(forceUpdateCallbackId, "Plugin Force Update", forceUpdate));
        container.commandHandlers->RegisterHandler(forceUpdate);
        displays.RegisterDisplay(forceUpdate);

        // Create the plugin help page toggle
        int changelogCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<PluginChangelog> changeLog =
            std::make_shared<PluginChangelog>(*container.windows, changelogCallbackId);

        // Register with handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(
            RadarScreen::CreateConfigurableDisplayCallback(changelogCallbackId, "Plugin Changelog", changeLog));
        container.commandHandlers->RegisterHandler(changeLog);
        displays.RegisterDisplay(changeLog);

        // Create the plugin information message box
        int informationCallback = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<PluginInformationMessage> infoMessage =
            std::make_shared<PluginInformationMessage>(*container.windows, informationCallback);

        // Register with handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(RadarScreen::CreateConfigurableDisplayCallback(
            informationCallback, "Plugin Information Message", infoMessage));
        container.commandHandlers->RegisterHandler(infoMessage);
        displays.RegisterDisplay(infoMessage);
    }
} // namespace UKControllerPlugin::Plugin
