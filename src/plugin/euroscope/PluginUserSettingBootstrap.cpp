#include "pch/pch.h"
#include "euroscope/PluginUserSettingBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin {
    namespace Euroscope {

        void PluginUserSettingBootstrap::BootstrapPlugin(PersistenceContainer & container)
        {
            container.pluginUserSettingHandler.reset(new UserSetting(*container.plugin));
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
