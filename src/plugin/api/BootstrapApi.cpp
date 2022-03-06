#include "ApiConfigurationMenuItem.h"
#include "BootstrapApi.h"
#include "api/ApiBootstrap.h"
#include "api/ApiFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/CallbackFunction.h"
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
    }

    void BootstrapConfigurationMenuItem(
        const PersistenceContainer& container, RadarScreen::ConfigurableDisplayCollection& configurableDisplays)
    {
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
