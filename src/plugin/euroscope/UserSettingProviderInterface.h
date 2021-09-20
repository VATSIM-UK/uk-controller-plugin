#pragma once

namespace UKControllerPlugin::Euroscope {

    /*
        An interface for any class that provides direct access to a settings file.
        This may be an ASR or the EuroScope settings.
    */
    class UserSettingProviderInterface
    {
        public:
        UserSettingProviderInterface();
        virtual ~UserSettingProviderInterface();
        UserSettingProviderInterface(const UserSettingProviderInterface&);
        UserSettingProviderInterface(UserSettingProviderInterface&&) = delete;
        [[nodiscard]] auto operator=(const UserSettingProviderInterface&) -> UserSettingProviderInterface&;
        [[nodiscard]] auto operator=(UserSettingProviderInterface&&) -> UserSettingProviderInterface& = delete;
        /*
            Returns the value of a key in the settings file, empty string if not exists.

            Will always return empty string if no value is stored.
        */
        virtual auto GetKey(std::string key) -> std::string = 0;

        /*
            Returns true if a key exists in the settings file.
        */
        virtual auto KeyExists(std::string key) -> bool = 0;

        /*
            Sets the value of a key in the UserSetting, with a description to display to the user.
        */
        virtual void SetKey(std::string key, std::string description, std::string value) = 0;
    };
} // namespace UKControllerPlugin::Euroscope
