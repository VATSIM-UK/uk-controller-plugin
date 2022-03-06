#include "ApiRequest.h"
#include "ApiRequestException.h"
#include "ApiRequestPerformerInterface.h"
#include "ChainableRequest.h"

namespace UKControllerPluginUtils::Api {

    ApiRequest::ApiRequest(
        const ApiRequestData& data,
        ApiRequestPerformerInterface& performer,
        std::function<void(void)> onCompletionHandler)
        : chain(std::make_shared<ChainableRequest>(data, performer, onCompletionHandler))
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
                    "Unhandled API exception when making request to uri " + exception.Uri() + ". Status code was " +
                    std::to_string(static_cast<uint64_t>(exception.StatusCode())));
            } else if (exception.InvalidJson()) {
                LogError(
                    "Unhandled API exception when making request to uri " + exception.Uri() +
                    ". Response was invalid "
                    "JSON");
            } else {
                LogError("Unknown unhandled API Exception when making request to uri " + exception.Uri());
            }
        });

        if (!async) {
            try {
                chain->Await();
            } catch (...) {
                // No rethrow
            }
        }
    }

    auto ApiRequest::Then(const std::function<void(Response)>& function) -> ApiRequest&
    {
        chain->Then(function);
        return *this;
    }

    auto ApiRequest::Then(const std::function<void(void)>& function) -> ApiRequest&
    {
        chain->Then(function);
        return *this;
    }

    auto ApiRequest::Catch(const std::function<void(const ApiRequestException&)>& function) -> ApiRequest&
    {
        chain->Catch(function);
        return *this;
    }

    void ApiRequest::Await()
    {
        this->async = false;
    }
} // namespace UKControllerPluginUtils::Api
