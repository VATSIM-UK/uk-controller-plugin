#include "ApiBootstrap.h"
#include "ApiFactory.h"
#include "ApiHelper.h"
#include "curl/CurlApi.h"

using UKControllerPlugin::Api::ApiHelper;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Curl::CurlInterface;

namespace UKControllerPluginUtils::Api {
    std::unique_ptr<ApiInterface> Bootstrap(ApiFactory& factory, CurlInterface& curl)
    {
        return std::make_unique<ApiHelper>(curl, factory.LegacyRequestBuilder());
    }
} // namespace UKControllerPluginUtils::Api
