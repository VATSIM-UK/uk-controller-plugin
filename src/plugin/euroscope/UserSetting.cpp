#include "pch/pch.h"
#include "euroscope/UserSetting.h"
#include "euroscope/UserSettingProviderInterface.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Euroscope::UserSettingProviderInterface;

namespace UKControllerPlugin {
    namespace Euroscope {

        UserSetting::UserSetting(UserSettingProviderInterface & userSettingProvider)
            : userSettingProvider(userSettingProvider)
        {
        }

        /*
            Returns a boolean value for the given ASR key. False if the key isn't a boolean.
        */
        bool UserSetting::GetBooleanEntry(std::string key, bool defaultValue)
        {
            std::string value = this->userSettingProvider.GetKey(key);
            return this->ValidBooleanEntry(value) ? value == "1" : defaultValue;
        }

        /*
            Return an RGB colour value for the given ASR key. Returns white if not a colour.
        */
        COLORREF UserSetting::GetColourEntry(std::string key, COLORREF defaultValue)
        {
            std::string value = this->userSettingProvider.GetKey(key);
            return this->ValidColourEntry(value) ? HelperFunctions::GetColourFromSettingString(value) : defaultValue;
        }

        /*
            Returns an double value for the given ASR key. 0.0 if the key isn't a double.
        */
        double UserSetting::GetFloatEntry(std::string key, float defaultValue)
        {
            std::string value = this->userSettingProvider.GetKey(key);
            return this->ValidFloatEntry(value) ? std::stod(value) : defaultValue;
        }

        unsigned int UserSetting::GetUnsignedIntegerEntry(std::string key, unsigned int defaultValue)
        {
            std::string value = this->userSettingProvider.GetKey(key);
            return this->ValidIntegerEntry(value) ? std::stoul(value) : defaultValue;
        }

        /*
            Returns an integer value for the given ASR key. 0 if the key isn't a integer.
        */
        int UserSetting::GetIntegerEntry(std::string key, int defaultValue)
        {
            std::string value = this->userSettingProvider.GetKey(key);
            return this->ValidIntegerEntry(value) ? std::stoi(value) : defaultValue;
        }

        /*
            Returns the string value of a given ASR key. Empty string if the key doesn't exist.
        */
        std::string UserSetting::GetStringEntry(std::string key, std::string defaultValue)
        {
            std::string value = this->userSettingProvider.GetKey(key);
            return this->ValidStringEntry(value) ? value : defaultValue;
        }

        /*
            Returns a list of strings
        */
        std::vector<std::string> UserSetting::GetStringListEntry(std::string key, std::vector<std::string> defaultValue)
        {
            std::string value = this->userSettingProvider.GetKey(key);

            if (!this->ValidStringEntry(value)) {
                return defaultValue;
            }

            std::vector<std::string> list = HelperFunctions::TokeniseString(';', value);

            return list.empty() ? defaultValue : list;
        }

        /*
            Returns true if there is some entry of some form for a given key.
        */
        bool UserSetting::HasEntry(std::string key)
        {
            return this->userSettingProvider.KeyExists(key);
        }

        /*
            Returns true if a key exists and its value is a boolean.
        */
        bool UserSetting::ValidBooleanEntry(std::string value)
        {
            return value == "1" || value == "0";
        }

        /*
            Returns true if a key exists and its value is an RGB colour.
        */
        bool UserSetting::ValidColourEntry(std::string value)
        {
            return HelperFunctions::IsValidSettingsColour(value);
        }

        /*
            Returns true if a key exists and its value is a float.
        */
        bool UserSetting::ValidFloatEntry(std::string value)
        {
            return HelperFunctions::IsFloat(value);
        }

        /*
            Returns true if a key exists and its value is an integer.
        */
        bool UserSetting::ValidIntegerEntry(std::string value)
        {
            return HelperFunctions::IsAnInteger(value);
        }

        /*
            Returns true if a key exists in the ASR - all keys come back as strings.
        */
        bool UserSetting::ValidStringEntry(std::string value)
        {
            return value != "";
        }

        /*
            Returns true if a key exists and its value is an unsigned integer
        */
        bool UserSetting::ValidUnsignedIntegerEntry(std::string value)
        {
            return HelperFunctions::IsAnInteger(value);
        }

        /*
            Save string data to the ASR.
        */
        void UserSetting::Save(std::string name, std::string description, std::string data)
        {
            this->userSettingProvider.SetKey(name, description, data);
        }

        /*
            Save int data to the ASR.
        */
        void UserSetting::Save(std::string name, std::string description, int data)
        {
            this->userSettingProvider.SetKey(name, description, std::to_string(data));
        }

        /*
            Save unsigned int data to the ASR
        */
        void UserSetting::Save(std::string name, std::string description, unsigned int data)
        {
            this->userSettingProvider.SetKey(name, description, std::to_string(data));
        }

        /*
            Save boolean data to the ASR.
        */
        void UserSetting::Save(std::string name, std::string description, bool data)
        {
            this->userSettingProvider.SetKey(name, description, std::to_string((data) ? 1 : 0));
        }

        /*
            Save float data to the ASR.
        */
        void UserSetting::Save(std::string name, std::string description, double data)
        {
            this->userSettingProvider.SetKey(name, description, std::to_string(data));
        }

        /*
            Save RGB colour data to the ASR.
        */
        void UserSetting::Save(std::string name, std::string description, COLORREF data)
        {
            this->userSettingProvider.SetKey(name, description, HelperFunctions::GetColourString(data));
        }

        /*
            Save string vector data
        */
        void UserSetting::Save(std::string name, std::string description, std::vector<std::string> data)
        {
            this->userSettingProvider.SetKey(name, description, HelperFunctions::VectorToDelimetedString(data, ";"));
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
