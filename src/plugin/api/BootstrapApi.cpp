#include "ApiConfigurationMenuItem.h"
#include "BootstrapApi.h"
#include "ReplaceApiKeyDialog.h"
#include "api/ApiBootstrap.h"
#include "api/ApiFactory.h"
#include "api/ApiKeyReceivedEvent.h"
#include "api/ApiKeyRedirectUrlBuilder.h"
#include "api/ApiSettingsProviderInterface.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/CallbackFunction.h"
#include "eventhandler/EventBus.h"
#include "dialog/DialogManager.h"
#include "eventhandler/EventHandlerFlags.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::CallbackFunction;

namespace UKControllerPlugin::Api {
    void BootstrapApi(PersistenceContainer& container)
    {
        container.apiFactory =
            UKControllerPluginUtils::Api::Bootstrap(*container.settingsRepository, *container.windows);
        container.api = UKControllerPluginUtils::Api::BootstrapLegacy(*container.apiFactory, *container.curl);

        // Register dialog
        auto replaceDialog = std::make_shared<ReplaceApiKeyDialog>(
            std::make_unique<UKControllerPluginUtils::Api::ApiKeyRedirectUrlBuilder>(
                container.apiFactory->SettingsProvider()->Get()),
            *container.windows);
        container.dialogManager->AddDialog(
            {IDD_API_KEY_REPLACE,
             "Replace API Key",
             reinterpret_cast<DLGPROC>(replaceDialog->WndProc), // NOLINT
             reinterpret_cast<LPARAM>(replaceDialog.get()),     // NOLINT
             replaceDialog});
        UKControllerPluginUtils::EventHandler::EventBus::Bus()
            .AddHandler<UKControllerPluginUtils::Api::ApiKeyReceivedEvent>(
                replaceDialog, UKControllerPluginUtils::EventHandler::EventHandlerFlags::Async);
    }

    void BootstrapConfigurationMenuItem(
        const PersistenceContainer& container, RadarScreen::ConfigurableDisplayCollection& configurableDisplays)
    {
        // Register menu item
        unsigned int callbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<ApiConfigurationMenuItem> menuItem = std::make_shared<ApiConfigurationMenuItem>(
            *container.apiFactory->SettingsProvider(), *container.windows, callbackId);

        CallbackFunction menuItemSelectedCallback(
            callbackId, // NOLINT
            "API Configuration Menu Item Selected",
            [menuItem](int functionId, std::string subject, RECT screenObjectArea) {
                menuItem->Configure(functionId, std::move(subject), screenObjectArea);
            });

        container.pluginFunctionHandlers->RegisterFunctionCall(menuItemSelectedCallback);
        configurableDisplays.RegisterDisplay(menuItem);
    }
} // namespace UKControllerPlugin::Api
