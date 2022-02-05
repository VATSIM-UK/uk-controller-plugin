#include "ApiBootstrap.h"
#include "ApiFactory.h"
#include "ApiHelper.h"
#include "ApiSettings.h"
#include "ConfigApiSettingsProvider.h"
#include "CurlApiRequestPerformerFactory.h"
#include "curl/CurlApi.h"
#include "setting/SettingRepository.h"
#include "setting/JsonFileSettingProvider.h"

using UKControllerPlugin::Api::ApiHelper;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::SetApiRequestFactory;
using UKControllerPlugin::Curl::CurlApi;
using UKControllerPlugin::Curl::CurlInterface;
using UKControllerPlugin::Setting::JsonFileSettingProvider;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPluginUtils::Api {

    /**
     * Bootstrap the "new" way of doing the API
     */
    auto Bootstrap(SettingRepository& settingRepository, WinApiInterface& windows) -> std::shared_ptr<ApiFactory>
    {
        settingRepository.AddProvider(std::make_shared<JsonFileSettingProvider>(
            L"api-settings.json", std::set<std::string>{"api-key", "api-url"}, windows));

        auto factory = std::make_shared<ApiFactory>(
            std::make_shared<ConfigApiSettingsProvider>(settingRepository, windows),
            std::make_shared<CurlApiRequestPerformerFactory>(std::make_unique<CurlApi>()),
            true);

        SetApiRequestFactory(factory);
        return factory;
    }

    /**
     * Bootstrap the "legacy" APIInterface
     */
    auto BootstrapLegacy(ApiFactory& factory, CurlInterface& curl) -> std::unique_ptr<ApiInterface>
    {
        return std::make_unique<ApiHelper>(curl, factory.LegacyRequestBuilder());
    }
} // namespace UKControllerPluginUtils::Api
