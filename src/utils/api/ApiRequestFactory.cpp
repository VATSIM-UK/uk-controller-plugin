#include "ApiRequestData.h"
#include "ApiRequestFactory.h"
#include "ApiRequestPerformerInterface.h"
#include "http/HttpMethod.h"

namespace UKControllerPluginUtils::Api {

    ApiRequestFactory::ApiRequestFactory(std::shared_ptr<ApiRequestPerformerInterface> requestPerformer, bool async)
        : requestPerformer(requestPerformer), async(async)
    {
    }

    auto ApiRequestFactory::Get(std::string uri) const -> ApiRequest
    {
        return ApiRequest(ApiRequestData(uri, Http::HttpMethod::Get()), *requestPerformer, async);
    }

    auto ApiRequestFactory::Post(std::string uri, nlohmann::json body) const -> ApiRequest
    {
        return ApiRequest(ApiRequestData(uri, Http::HttpMethod::Post(), body), *requestPerformer, async);
    }

    auto ApiRequestFactory::Put(std::string uri, nlohmann::json body) const -> ApiRequest
    {
        return ApiRequest(ApiRequestData(uri, Http::HttpMethod::Put(), body), *requestPerformer, async);
    }

    auto ApiRequestFactory::Patch(std::string uri, nlohmann::json body) const -> ApiRequest
    {
        return ApiRequest(ApiRequestData(uri, Http::HttpMethod::Patch(), body), *requestPerformer, async);
    }

    auto ApiRequestFactory::Delete(std::string uri) const -> ApiRequest
    {
        return ApiRequest(ApiRequestData(uri, Http::HttpMethod::Delete()), *requestPerformer, async);
    }
} // namespace UKControllerPluginUtils::Api
