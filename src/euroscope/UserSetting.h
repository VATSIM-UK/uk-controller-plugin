#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class UserSettingProviderInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            A class for saving data to the Euroscope settings file, be it plugin settings or ASR.
            This takes values of different types and converts them to standard formats.
        */
        class UserSetting
        {
            public:
                explicit UserSetting(
                    UKControllerPlugin::Euroscope::UserSettingProviderInterface & userSettingProvider
                );
                bool GetBooleanEntry(std::string key, bool defaultValue = false);
                COLORREF GetColourEntry(std::string key, COLORREF defaultValue = RGB(255, 255, 255));
                double GetFloatEntry(std::string key, float defaultValue = 0.0f);
                int GetIntegerEntry(std::string key, int defaultValue = 0);
                std::string GetStringEntry(std::string key, std::string defaultValue = "");
                void Save(std::string name, std::string description, std::string data);
                void Save(std::string name, std::string description, int data);
                void Save(std::string name, std::string description, bool data);
                void Save(std::string name, std::string description, double data);
                void Save(std::string name, std::string description, COLORREF data);

            private:
                bool ValidBooleanEntry(std::string value);
                bool ValidColourEntry(std::string value);
                bool ValidFloatEntry(std::string value);
                bool ValidIntegerEntry(std::string value);
                bool ValidStringEntry(std::string value);

                // A link to the ES class that actually provides the settings interface
                UKControllerPlugin::Euroscope::UserSettingProviderInterface & userSettingProvider;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
