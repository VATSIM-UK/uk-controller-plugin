#pragma once

namespace UKControllerPlugin::Euroscope {
    class UserSettingProviderInterface;

    /*
        A class for saving data to the Euroscope settings file, be it plugin settings or ASR.
        This takes values of different types and converts them to standard formats.
    */
    class UserSetting
    {
        public:
        explicit UserSetting(UKControllerPlugin::Euroscope::UserSettingProviderInterface& userSettingProvider);
        auto GetBooleanEntry(std::string key, bool defaultValue = false) -> bool;
        auto GetColourEntry(std::string key, COLORREF defaultValue = RGB(255, 255, 255)) -> COLORREF;
        auto GetFloatEntry(std::string key, float defaultValue = 0.0F) -> double;
        auto GetUnsignedIntegerEntry(std::string key, unsigned int defaultValue = 0) -> unsigned int;
        auto GetIntegerEntry(std::string key, int defaultValue = 0) -> int;
        auto GetStringEntry(std::string key, const std::string& defaultValue = "") -> std::string;
        auto GetStringListEntry(std::string key, std::vector<std::string> defaultValue = {})
            -> std::vector<std::string>;
        auto HasEntry(std::string key) -> bool;
        void Save(std::string name, std::string description, std::string data);
        void Save(std::string name, std::string description, int data);
        void Save(std::string name, std::string description, unsigned int data);
        void Save(std::string name, std::string description, bool data);
        void Save(std::string name, std::string description, double data);
        void Save(std::string name, std::string description, COLORREF data);
        void Save(std::string name, std::string description, std::vector<std::string> data);
        void Save(std::string name, std::string description, long data);

        private:
        static auto ValidBooleanEntry(const std::string& value) -> bool;
        static auto ValidColourEntry(std::string value) -> bool;
        static auto ValidFloatEntry(std::string value) -> bool;
        static auto ValidIntegerEntry(std::string value) -> bool;
        static auto ValidStringEntry(const std::string& value) -> bool;

        // A link to the ES class that actually provides the settings interface
        UKControllerPlugin::Euroscope::UserSettingProviderInterface& userSettingProvider;
    };
} // namespace UKControllerPlugin::Euroscope
