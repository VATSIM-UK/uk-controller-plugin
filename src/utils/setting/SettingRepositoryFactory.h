#pragma once
#include "setting/SettingRepository.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Setting {

        /*
            Class for creating the Setting Repository and making
            sure the correct files are considered.
        */
        class SettingRepositoryFactory
        {
            public:
                static std::unique_ptr<UKControllerPlugin::Setting::SettingRepository> Create();
        };
    }  // namespace Setting
}  // namespace UKControllerPlugin
