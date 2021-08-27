#include "PluginUserSettingBootstrap.h"
#include "UserSetting.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin::Euroscope {

    void PluginUserSettingBootstrap::BootstrapPlugin(PersistenceContainer& container)
    {
        container.pluginUserSettingHandler = std::make_unique<UserSetting>(*container.plugin);
    }
} // namespace UKControllerPlugin::Euroscope
