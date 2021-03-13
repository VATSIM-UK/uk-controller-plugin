#include "utils/pch.h"
#include "setting/SettingRepositoryFactory.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Setting::SettingRepository;

namespace UKControllerPlugin
{
    namespace Setting
    {
        std::unique_ptr<SettingRepository> SettingRepositoryFactory::Create(
            WinApiInterface& winApi
        )
        {
            std::unique_ptr<SettingRepository> repo = std::make_unique<SettingRepository>(winApi);
            repo->AddSettingsFromJsonFile("api-settings.json");
            return repo;
        }
    } // namespace Setting
} // namespace UKControllerPlugin
