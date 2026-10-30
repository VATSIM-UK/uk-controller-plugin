#include "ThemeModule.h"
#include "ThemeManager.h"
#include "ThemeSettings.h"
#include "ThemeUserSettingHandler.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/UserSettingAwareCollection.h"

namespace UKControllerPlugin::Theme {

    void ThemeModule::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        ThemeManager::Instance();

        auto settings = std::make_shared<ThemeSettings>(*container.pluginUserSettingHandler);
        auto handler = std::make_shared<ThemeUserSettingHandler>(settings);

        container.userSettingHandlers->RegisterHandler(handler);
        container.themeSettings = settings;
    }

} // namespace UKControllerPlugin::Theme
