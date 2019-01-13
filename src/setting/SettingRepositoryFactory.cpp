#include "pch/stdafx.h"
#include "setting/SettingRepositoryFactory.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {
    namespace Setting {
        UKControllerPlugin::Setting::SettingRepository SettingRepositoryFactory::Create(
            UKControllerPlugin::Windows::WinApiInterface & winApi
        )
        {
            UKControllerPlugin::Setting::SettingRepository repo(winApi);
            repo.AddSettingsFromJsonFile("api-settings.json");
            return repo;
        }
    }  // namespace Setting
}  // namespace UKControllerPlugin
