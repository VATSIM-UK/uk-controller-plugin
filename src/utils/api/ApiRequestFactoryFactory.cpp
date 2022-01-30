#include "ApiCurlRequestFactory.h"
#include "ApiHeaderApplicator.h"
#include "ApiRequestPerformer.h"
#include "ApiRequestFactory.h"
#include "ApiRequestFactoryFactory.h"
#include "ApiSettings.h"
#include "ApiSettingsFactory.h"
#include "ApiUrlBuilder.h"

using UKControllerPlugin::Curl::CurlInterface;
using UKControllerPlugin::Setting::SettingRepository;

namespace UKControllerPluginUtils::Api {

    std::shared_ptr<ApiSettings> apiSettings;
    std::unique_ptr<ApiUrlBuilder> urlBuilder;
    std::unique_ptr<ApiHeaderApplicator> headerApplicator;
    std::unique_ptr<ApiCurlRequestFactory> curlFactory;

    auto Make(const SettingRepository& settings, CurlInterface& curl, bool async) -> std::unique_ptr<ApiRequestFactory>
    {
        apiSettings = Make(settings);
        urlBuilder = std::make_unique<ApiUrlBuilder>(*apiSettings);
        headerApplicator = std::make_unique<ApiHeaderApplicator>(*apiSettings);
        curlFactory = std::make_unique<ApiCurlRequestFactory>(*urlBuilder, *headerApplicator);
        return std::make_unique<ApiRequestFactory>(std::make_shared<ApiRequestPerformer>(curl, *curlFactory), async);
    }
} // namespace UKControllerPluginUtils::Api
