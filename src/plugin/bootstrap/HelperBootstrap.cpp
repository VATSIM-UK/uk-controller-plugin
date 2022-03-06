#include "HelperBootstrap.h"
#include "PersistenceContainer.h"
#include "curl/CurlApi.h"
#include "euroscope/CallbackFunction.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "setting/SettingRepository.h"
#include "setting/SettingRepositoryFactory.h"
#include "task/TaskRunner.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Curl::CurlApi;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPlugin::Setting::SettingRepositoryFactory;
using UKControllerPlugin::TaskManager::TaskRunner;

namespace UKControllerPlugin::Bootstrap {

    /*
        Create the various helpers we use and put them in the persistence structure.
    */
    void HelperBootstrap::Bootstrap(PersistenceContainer& persistence)
    {
        persistence.settingsRepository = SettingRepositoryFactory::Create();
        persistence.taskRunner = std::make_shared<TaskRunner>(3);
        SetTaskRunner(persistence.taskRunner);
    }
} // namespace UKControllerPlugin::Bootstrap
