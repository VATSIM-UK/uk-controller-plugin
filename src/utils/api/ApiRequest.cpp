#include "ApiRequest.h"
#include "ApiRequestException.h"
#include "ApiRequestPerformerInterface.h"
#include "ChainableRequest.h"

namespace UKControllerPluginUtils::Api {

    ApiRequest::ApiRequest(const ApiRequestData& data, ApiRequestPerformerInterface& performer, bool async)
        : chain(std::make_shared<ChainableRequest>(data, performer)), async(async)
    {
    }

    ApiRequest::~ApiRequest()
    {
        // Not dealing with an exception means we end up in an "everything blows up" situation.
        // So have a default handler for API exceptions.
        chain->Catch([](const ApiRequestException& exception) {
            if (exception.StatusCode() != Http::HttpStatusCode::Ok &&
                exception.StatusCode() != Http::HttpStatusCode::Created &&
                exception.StatusCode() != Http::HttpStatusCode::NoContent) {
                LogError(
                    "Unhandled API exception when making request to " + exception.Uri() + ". Status code was " +
                    std::to_string(static_cast<uint64_t>(exception.StatusCode())));
            } else if (exception.InvalidJson()) {
                LogError(
                    "Unhandled API exception when making request to " + exception.Uri() +
                    ". Response was invalid "
                    "JSON");
            } else {
                LogError("Unknown unhandled API Exception");
            }
        });

        if (!async) {
            chain->Await();
        }
    }

    auto ApiRequest::Then(const std::function<Response(Response)>& function) -> ApiRequest
    {
        chain->Then(function);
        return std::move(*this);
    }

    auto ApiRequest::Then(const std::function<void(Response)>& function) -> ApiRequest
    {
        chain->Then(function);
        return std::move(*this);
    }

    auto ApiRequest::Then(const std::function<void(void)>& function) -> ApiRequest
    {
        chain->Then(function);
        return std::move(*this);
    }

    auto ApiRequest::Catch(const std::function<void(const ApiRequestException&)>& function) -> ApiRequest
    {
        chain->Catch(function);
        return std::move(*this);
    }

    void ApiRequest::Await()
    {
        async = false;
    }
} // namespace UKControllerPluginUtils::Api
