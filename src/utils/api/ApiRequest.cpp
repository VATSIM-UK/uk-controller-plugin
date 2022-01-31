#include "ApiException.h"
#include "ApiRequest.h"
#include "ApiRequestPerformerInterface.h"
#include "ChainableRequest.h"

using UKControllerPlugin::Api::ApiException;

namespace UKControllerPluginUtils::Api {

    ApiRequest::ApiRequest(const ApiRequestData& data, ApiRequestPerformerInterface& performer, bool async)
        : chain(std::make_shared<ChainableRequest>(data, performer)), async(async)
    {
    }

    ApiRequest::~ApiRequest()
    {
        if (!async) {
            chain->Await();
        }
    }

    auto ApiRequest::Then(const std::function<Response(Response)>& function) -> ApiRequest
    {
        chain->Then(function);
        return std::move(*this);
    }

    auto ApiRequest::Catch(const std::function<void(std::exception_ptr exception)>& function) -> ApiRequest
    {
        chain->Catch(function);
        return std::move(*this);
    }

    void ApiRequest::Await()
    {
        async = false;
    }
} // namespace UKControllerPluginUtils::Api
