#include "ApiHeaderApplicator.h"
#include "ApiSettings.h"
#include "curl/CurlRequest.h"

using UKControllerPlugin::Curl::CurlRequest;

namespace UKControllerPluginUtils::Api {

    ApiHeaderApplicator::ApiHeaderApplicator(const ApiSettings& settings) : settings(settings)
    {
    }

    void ApiHeaderApplicator::ApplyHeaders(CurlRequest& request) const
    {
        request.AddHeader("Authorization", "Bearer " + settings.Key());
        request.AddHeader("Accept", "application/json");
        request.AddHeader("Content-Type", "application/json");
    }
} // namespace UKControllerPluginUtils::Api
