#include "pch.h"
#include "updater/UKControllerPluginUpdater.h"
#include "updater/PerformUpdates.h"
#include "windows/WinApiInterface.h"
#include "windows/WinApiBootstrap.h"
#include "api/ApiBootstrap.h"
#include "setting/SettingRepositoryFactory.h"
#include "curl/CurlApi.h"

HINSTANCE dllInstance;

std::unique_ptr<UKControllerPlugin::Windows::WinApiInterface> windows;

void PerformUpdates(void)
{
    // Boot up the windows API, create the root folder and create the logger
    windows = UKControllerPlugin::Windows::Bootstrap(nullptr);
    UKControllerPlugin::Log::LoggerBootstrap::Bootstrap(*windows, L"updater");

    // Bootstrap the API, download the updater if we don't have it already and run it
    UKControllerPlugin::Curl::CurlApi curl;
    std::unique_ptr<UKControllerPlugin::Setting::SettingRepository> settings =
        UKControllerPlugin::Setting::SettingRepositoryFactory::Create(*windows);
    std::unique_ptr<UKControllerPlugin::Api::ApiInterface> api = UKControllerPlugin::Api::Bootstrap(
        *settings,
        curl
    );
    CheckForUpdates();
}
