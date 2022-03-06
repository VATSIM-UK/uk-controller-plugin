#include "SettingRepositoryFactory.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin::Setting {
    std::unique_ptr<SettingRepository> SettingRepositoryFactory::Create()
    {
        return std::make_unique<SettingRepository>();
    }
} // namespace UKControllerPlugin::Setting
