#include "SettingRepositoryFactory.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin::Setting {
    std::unique_ptr<SettingRepository> SettingRepositoryFactory::Create(WinApiInterface& winApi)
    {
        std::unique_ptr<SettingRepository> repo = std::make_unique<SettingRepository>(winApi);
        repo->AddSettingsFromJsonFile("api-settings.json");
        repo->AddSettingsFromJsonFile("release-channel.json");
        return repo;
    }
} // namespace UKControllerPlugin::Setting
