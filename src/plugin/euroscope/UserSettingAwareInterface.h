#pragma once

namespace UKControllerPlugin::Euroscope {

    class UserSetting;

    /*
        An interface to be implemented when the class
        wishes to be notified that the main user settings have been updated.
    */
    class UserSettingAwareInterface
    {
        public:
        UserSettingAwareInterface();
        virtual ~UserSettingAwareInterface();
        UserSettingAwareInterface(const UserSettingAwareInterface&);
        UserSettingAwareInterface(UserSettingAwareInterface&&) noexcept;
        [[nodiscard]] auto operator=(const UserSettingAwareInterface&) -> UserSettingAwareInterface&;
        [[nodiscard]] auto operator=(UserSettingAwareInterface&&) noexcept -> UserSettingAwareInterface&;
        /*
            Should be called whenever user settings have been updated.
        */
        virtual void UserSettingsUpdated(UKControllerPlugin::Euroscope::UserSetting& userSettings) = 0;
    };
} // namespace UKControllerPlugin::Euroscope
