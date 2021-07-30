#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            An interface for any class that provides direct access to a settings file.
            This may be an ASR or the EuroScope settings.
        */
        class UserSettingProviderInterface
        {
            public:

                /*
                    Returns the value of a key in the settings file, empty string if not exists.

                    Will always return empty string if no value is stored.
                */
                virtual std::string GetKey(std::string key) = 0;

                /*
                    Returns true if a key exists in the settings file.
                */
                virtual bool KeyExists(std::string key) = 0;

                /*
                    Sets the value of a key in the UserSetting, with a description to display to the user.
                */
                virtual void SetKey(std::string key, std::string description, std::string value) = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
