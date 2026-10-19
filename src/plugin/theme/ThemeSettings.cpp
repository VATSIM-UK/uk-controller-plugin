#include "ThemeSettings.h"
#include "Palette.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin::Theme {

    static std::string PALETTE_SETTING = "colourPalette";
    static std::string PALETTE_DESCRIPTION = "Colour palette";

    ThemeSettings::ThemeSettings(Euroscope::UserSetting& userSettings_) : userSettings(userSettings_)
    {
    }

    auto ThemeSettings::Palette() const -> std::string
    {
        return userSettings.GetStringEntry(PALETTE_SETTING, Palette::DEFAULT);
    }

    void ThemeSettings::SetPalette(std::string id)
    {
        userSettings.Save(PALETTE_SETTING, PALETTE_DESCRIPTION, std::move(id));
    }

} // namespace UKControllerPlugin::Theme
