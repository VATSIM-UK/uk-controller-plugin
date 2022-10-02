#pragma once

namespace UKControllerPlugin {
    namespace Setting {
        typedef struct SettingValue
        {
            std::string setting;
            std::string value;
            std::string storagePath;
        } SettingValue;
    } // namespace Setting
} // namespace UKControllerPlugin
