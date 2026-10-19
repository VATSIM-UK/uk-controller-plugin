#include "ThemeUserSettingHandler.h"
#include "Palette.h"
#include "ThemeManager.h"
#include "ThemeSettings.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin::Theme {

    ThemeUserSettingHandler::ThemeUserSettingHandler(std::shared_ptr<ThemeSettings> settings_) : settings(settings_)
    {
        UserSettingsUpdated();
    }

    void ThemeUserSettingHandler::UserSettingsUpdated(Euroscope::UserSetting&)
    {
        UserSettingsUpdated();
    }

    void ThemeUserSettingHandler::UserSettingsUpdated()
    {
        auto palette = Palette::GetPalette(settings->Palette().c_str());
        ThemeManager::Instance().ApplyPalette(palette);
    }

} // namespace UKControllerPlugin::Theme
