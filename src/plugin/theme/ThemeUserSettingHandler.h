#pragma once

#include "euroscope/UserSettingAwareInterface.h"

namespace UKControllerPlugin::Theme {

    class ThemeSettings;

    /*
        Bootstraps the theming module.
    */
    class ThemeUserSettingHandler : public Euroscope::UserSettingAwareInterface
    {
        public:
        explicit ThemeUserSettingHandler(std::shared_ptr<ThemeSettings> settings);
        void UserSettingsUpdated(Euroscope::UserSetting& userSettings) override;
        void UserSettingsUpdated();

        private:
        std::shared_ptr<ThemeSettings> settings;
    };

} // namespace UKControllerPlugin::Theme
