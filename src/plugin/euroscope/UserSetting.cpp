#include "pch/pch.h"

#include "euroscope/UserSetting.h"
#include "euroscope/UserSettingProviderInterface.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Euroscope::UserSettingProviderInterface;

namespace UKControllerPlugin::Euroscope {

    UserSetting::UserSetting(UserSettingProviderInterface& userSettingProvider)
        : userSettingProvider(userSettingProvider)
    {
    }

    /*
        Returns a boolean value for the given ASR key. False if the key isn't a boolean.
    */
    auto UserSetting::GetBooleanEntry(std::string key, bool defaultValue) -> bool
    {
        std::string value = this->userSettingProvider.GetKey(std::move(key));
        return UKControllerPlugin::Euroscope::UserSetting::ValidBooleanEntry(value) ? value == "1" : defaultValue;
    }

    /*
        Return an RGB colour value for the given ASR key. Returns white if not a colour.
    */
    auto UserSetting::GetColourEntry(std::string key, COLORREF defaultValue) -> COLORREF
    {
        std::string value = this->userSettingProvider.GetKey(std::move(key));
        return UKControllerPlugin::Euroscope::UserSetting::ValidColourEntry(value)
                   ? HelperFunctions::GetColourFromSettingString(value)
                   : defaultValue;
    }

    /*
        Returns an double value for the given ASR key. 0.0 if the key isn't a double.
    */
    auto UserSetting::GetFloatEntry(std::string key, float defaultValue) -> double
    {
        std::string value = this->userSettingProvider.GetKey(std::move(key));
        return UKControllerPlugin::Euroscope::UserSetting::ValidFloatEntry(value) ? std::stod(value) : defaultValue;
    }

    auto UserSetting::GetUnsignedIntegerEntry(std::string key, unsigned int defaultValue) -> unsigned int
    {
        std::string value = this->userSettingProvider.GetKey(std::move(key));
        return UKControllerPlugin::Euroscope::UserSetting::ValidIntegerEntry(value) ? std::stoul(value) : defaultValue;
    }

    /*
        Returns an integer value for the given ASR key. 0 if the key isn't a integer.
    */
    auto UserSetting::GetIntegerEntry(std::string key, int defaultValue) -> int
    {
        std::string value = this->userSettingProvider.GetKey(std::move(key));
        return UKControllerPlugin::Euroscope::UserSetting::ValidIntegerEntry(value) ? std::stoi(value) : defaultValue;
    }

    /*
        Returns the string value of a given ASR key. Empty string if the key doesn't exist.
    */
    auto UserSetting::GetStringEntry(std::string key, const std::string& defaultValue) -> std::string
    {
        std::string value = this->userSettingProvider.GetKey(std::move(key));
        return UKControllerPlugin::Euroscope::UserSetting::ValidStringEntry(value) ? value : defaultValue;
    }

    /*
        Returns a list of strings
    */
    auto UserSetting::GetStringListEntry(std::string key, std::vector<std::string> defaultValue)
        -> std::vector<std::string>
    {
        std::string value = this->userSettingProvider.GetKey(std::move(key));

        if (!UKControllerPlugin::Euroscope::UserSetting::ValidStringEntry(value)) {
            return defaultValue;
        }

        std::vector<std::string> list = HelperFunctions::TokeniseString(';', value);

        return list.empty() ? defaultValue : list;
    }

    /*
        Returns true if there is some entry of some form for a given key.
    */
    auto UserSetting::HasEntry(std::string key) -> bool
    {
        return this->userSettingProvider.KeyExists(std::move(key));
    }

    /*
        Returns true if a key exists and its value is a boolean.
    */
    auto UserSetting::ValidBooleanEntry(const std::string& value) -> bool
    {
        return value == "1" || value == "0";
    }

    /*
        Returns true if a key exists and its value is an RGB colour.
    */
    auto UserSetting::ValidColourEntry(std::string value) -> bool
    {
        return HelperFunctions::IsValidSettingsColour(std::move(value));
    }

    /*
        Returns true if a key exists and its value is a float.
    */
    auto UserSetting::ValidFloatEntry(std::string value) -> bool
    {
        return HelperFunctions::IsFloat(std::move(value));
    }

    /*
        Returns true if a key exists and its value is an integer.
    */
    auto UserSetting::ValidIntegerEntry(std::string value) -> bool
    {
        return HelperFunctions::IsAnInteger(std::move(value));
    }

    /*
        Returns true if a key exists in the ASR - all keys come back as strings.
    */
    auto UserSetting::ValidStringEntry(const std::string& value) -> bool
    {
        return !value.empty();
    }

    /*
        Save string data to the ASR.
    */
    void UserSetting::Save(std::string name, std::string description, std::string data)
    {
        this->userSettingProvider.SetKey(std::move(name), std::move(description), std::move(data));
    }

    /*
        Save int data to the ASR.
    */
    void UserSetting::Save(std::string name, std::string description, int data)
    {
        this->userSettingProvider.SetKey(std::move(name), std::move(description), std::to_string(data));
    }

    /*
        Save unsigned int data to the ASR
    */
    void UserSetting::Save(std::string name, std::string description, unsigned int data)
    {
        this->userSettingProvider.SetKey(std::move(name), std::move(description), std::to_string(data));
    }

    /*
        Save boolean data to the ASR.
    */
    void UserSetting::Save(std::string name, std::string description, bool data)
    {
        this->userSettingProvider.SetKey(std::move(name), std::move(description), std::to_string((data) ? 1 : 0));
    }

    /*
        Save float data to the ASR.
    */
    void UserSetting::Save(std::string name, std::string description, double data)
    {
        this->userSettingProvider.SetKey(std::move(name), std::move(description), std::to_string(data));
    }

    /*
        Save RGB colour data to the ASR.
    */
    void UserSetting::Save(std::string name, std::string description, COLORREF data)
    {
        this->userSettingProvider.SetKey(
            std::move(name), std::move(description), HelperFunctions::GetColourString(data));
    }

    /*
        Save string vector data
    */
    void UserSetting::Save(std::string name, std::string description, std::vector<std::string> data)
    {
        this->userSettingProvider.SetKey(
            std::move(name), std::move(description), HelperFunctions::VectorToDelimetedString(std::move(data), ";"));
    }

    void UserSetting::Save(std::string name, std::string description, long data)
    {
        this->userSettingProvider.SetKey(std::move(name), std::move(description), std::to_string(data));
    }
} // namespace UKControllerPlugin::Euroscope
