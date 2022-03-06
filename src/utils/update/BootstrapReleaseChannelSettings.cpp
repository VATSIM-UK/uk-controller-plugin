#include "BootstrapReleaseChannelSettings.h"
#include "setting/JsonFileSettingProvider.h"
#include "setting/SettingRepository.h"

using UKControllerPlugin::Setting::JsonFileSettingProvider;
using UKControllerPlugin::Setting::SettingRepository;

namespace UKControllerPluginUtils::Update {

    void
    BootstrapReleaseChannelSettings(SettingRepository& settings, UKControllerPlugin::Windows::WinApiInterface& windows)
    {
        settings.AddProvider(std::make_shared<JsonFileSettingProvider>(
            L"release-channel.json", std::set<std::string>{"release_channel"}, windows));
    }
} // namespace UKControllerPluginUtils::Update
