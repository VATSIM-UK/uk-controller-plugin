#include "pch/stdafx.h"
#include "bootstrap/HelperBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "setting/SettingRepository.h"
#include "setting/SettingRepositoryFactory.h"
#include "task/TaskRunner.h"
#include "api/ApiHelper.h"
#include "bootstrap/LocateApiSettings.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPlugin::Setting::SettingRepositoryFactory;
using UKControllerPlugin::Api::ApiHelper;
using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::TaskManager::TaskRunner;

namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Create the various helpers we use and put them in the persistence structure.
        */
        void HelperBootstrap::Bootstrap(PersistenceContainer & persistence)
        {
            SettingRepository settings = SettingRepositoryFactory::Create(*persistence.windows);

            // Prompt for a settings file, if one isn't there.
            UKControllerPlugin::Bootstrap::LocateApiSettings(*persistence.windows, settings);

            ApiRequestBuilder requestBuilder(settings.GetSetting("api-url"), settings.GetSetting("api-key"));
            persistence.api.reset(
                new ApiHelper(*persistence.curl, requestBuilder, *persistence.windows)
            );
            persistence.taskRunner.reset(new TaskRunner(2, 0));
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
