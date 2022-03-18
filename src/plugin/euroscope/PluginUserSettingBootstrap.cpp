#include "PluginUserSettingBootstrap.h"
#include "PluginSettingsProviderCollection.h"
#include "UserSetting.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin::Euroscope {

    void PluginUserSettingBootstrap::BootstrapPlugin(PersistenceContainer& container)
    {
        container.pluginUserSettingHandler = std::make_unique<UserSetting>(static_cast<UKPlugin&>(*container.plugin));
        container.pluginSettingsProviders =
            std::make_unique<PluginSettingsProviderCollection>(*container.pluginUserSettingHandler);
    }
} // namespace UKControllerPlugin::Euroscope
