#include "ApiFactory.h"
#include "ApiRequestFactory.h"
#include "AbstractApiRequestPerformerFactory.h"
#include "ApiSettings.h"
#include "setting/SettingRepository.h"

namespace UKControllerPluginUtils::Api {

    ApiFactory::ApiFactory(std::unique_ptr<AbstractApiRequestPerformerFactory> requestPerformerFactory, bool async)
        : requestPerformerFactory(std::move(requestPerformerFactory)), async(async)
    {
    }

    auto ApiFactory::Settings(const UKControllerPlugin::Setting::SettingRepository& settings) -> ApiSettings&
    {
        if (apiSettings == nullptr) {
            apiSettings = std::make_unique<ApiSettings>(
                settings.GetSetting("api-url", "https://ukcp.vatsim.uk"), settings.GetSetting("api-key"));
        }

        return *apiSettings;
    }

    auto ApiFactory::RequestFactory(
        UKControllerPlugin::Curl::CurlInterface& curl, const UKControllerPlugin::Setting::SettingRepository& settings)
        -> const ApiRequestFactory&
    {
        if (requestFactory == nullptr) {
            requestFactory =
                std::make_unique<ApiRequestFactory>(requestPerformerFactory->Make(Settings(settings)), async);
        }

        return *requestFactory;
    }
} // namespace UKControllerPluginUtils::Api
