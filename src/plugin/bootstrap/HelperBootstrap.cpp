#include "HelperBootstrap.h"
#include "PersistenceContainer.h"
#include "api/ApiConfigurationMenuItem.h"
#include "api/ApiFactory.h"
#include "api/ApiHelper.h"
#include "api/CurlApiRequestPerformerFactory.h"
#include "api/LocateApiSettings.h"
#include "curl/CurlApi.h"
#include "euroscope/CallbackFunction.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "setting/SettingRepository.h"
#include "setting/SettingRepositoryFactory.h"
#include "task/TaskRunner.h"

using UKControllerPlugin::Api::ApiConfigurationMenuItem;
using UKControllerPlugin::Api::ApiHelper;
using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Curl::CurlApi;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPlugin::Setting::SettingRepositoryFactory;
using UKControllerPlugin::TaskManager::TaskRunner;
using UKControllerPluginUtils::Api::ApiFactory;
using UKControllerPluginUtils::Api::CurlApiRequestPerformerFactory;

namespace UKControllerPlugin::Bootstrap {

    /*
        Create the various helpers we use and put them in the persistence structure.
    */
    void HelperBootstrap::Bootstrap(PersistenceContainer& persistence)
    {
        persistence.settingsRepository = SettingRepositoryFactory::Create(*persistence.windows);

        // Prompt for a settings file, if one isn't there.
        Api::LocateApiSettings(*persistence.windows, *persistence.settingsRepository);

        persistence.apiFactory = std::make_unique<ApiFactory>(
            std::make_unique<CurlApiRequestPerformerFactory>(std::make_unique<CurlApi>()), true);

        ApiRequestBuilder requestBuilder(
            persistence.settingsRepository->GetSetting("api-url", "https://ukcp.vatsim.uk"),
            persistence.settingsRepository->GetSetting("api-key"));
        persistence.api = std::make_unique<ApiHelper>(*persistence.curl, requestBuilder);

        persistence.taskRunner = std::make_shared<TaskRunner>(3);
        SetTaskRunner(persistence.taskRunner);
    }

    /*
        Create the ApiConfigurationMenuItem and bootstrap it in.
    */
    void HelperBootstrap::BootstrapApiConfigurationItem(
        const PersistenceContainer& persistence, ConfigurableDisplayCollection& configurableDisplays)
    {
        unsigned int callbackId = persistence.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        std::shared_ptr<ApiConfigurationMenuItem> menuItem =
            std::make_shared<ApiConfigurationMenuItem>(*persistence.windows, callbackId);

        CallbackFunction menuItemSelectedCallback(
            callbackId, // NOLINT
            "API Configuration Menu Item Selected",
            [menuItem](int functionId, std::string subject, RECT screenObjectArea) {
                menuItem->Configure(functionId, std::move(subject), screenObjectArea);
            });

        persistence.pluginFunctionHandlers->RegisterFunctionCall(menuItemSelectedCallback);
        configurableDisplays.RegisterDisplay(menuItem);
    }
} // namespace UKControllerPlugin::Bootstrap
