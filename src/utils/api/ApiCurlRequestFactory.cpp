#include "ApiCurlRequestFactory.h"
#include "ApiHeaderApplicator.h"
#include "ApiRequestData.h"
#include "ApiUrlBuilder.h"

using UKControllerPlugin::Curl::CurlRequest;

namespace UKControllerPluginUtils::Api {

    ApiCurlRequestFactory::ApiCurlRequestFactory(
        const ApiUrlBuilder& urlBuilder, const ApiHeaderApplicator& headerApplicator)
        : urlBuilder(urlBuilder), headerApplicator(headerApplicator)
    {
    }

    auto ApiCurlRequestFactory::BuildCurlRequest(const ApiRequestData& data) const -> CurlRequest
    {
        CurlRequest request(urlBuilder.BuildUrl(data), data.Method());
        if (!data.Body().empty()) {
            request.SetBody(data.Body().dump());
        }

        headerApplicator.ApplyHeaders(request);
        return request;
    }
} // namespace UKControllerPluginUtils::Api
