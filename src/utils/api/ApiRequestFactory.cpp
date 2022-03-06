#include "ApiRequestData.h"
#include "ApiRequestFactory.h"
#include "ApiRequestPerformerInterface.h"
#include "http/HttpMethod.h"

namespace UKControllerPluginUtils::Api {

    ApiRequestFactory::ApiRequestFactory(ApiRequestPerformerInterface& requestPerformer)
        : requestPerformer(requestPerformer), requestsInProgress(0)
    {
    }

    auto ApiRequestFactory::Get(std::string uri) -> ApiRequest
    {
        this->BeginRequest();
        return {ApiRequestData(std::move(uri), Http::HttpMethod::Get()), requestPerformer, OnCompletionFunction()};
    }

    auto ApiRequestFactory::Post(std::string uri, nlohmann::json body) -> ApiRequest
    {
        this->BeginRequest();
        return {
            ApiRequestData(std::move(uri), Http::HttpMethod::Post(), body), requestPerformer, OnCompletionFunction()};
    }

    auto ApiRequestFactory::Put(std::string uri, nlohmann::json body) -> ApiRequest
    {
        this->BeginRequest();
        return {
            ApiRequestData(std::move(uri), Http::HttpMethod::Put(), body), requestPerformer, OnCompletionFunction()};
    }

    auto ApiRequestFactory::Patch(std::string uri, nlohmann::json body) -> ApiRequest
    {
        this->BeginRequest();
        return {
            ApiRequestData(std::move(uri), Http::HttpMethod::Patch(), body), requestPerformer, OnCompletionFunction()};
    }

    auto ApiRequestFactory::Delete(std::string uri) -> ApiRequest
    {
        this->BeginRequest();
        return {ApiRequestData(std::move(uri), Http::HttpMethod::Delete()), requestPerformer, OnCompletionFunction()};
    }

    auto ApiRequestFactory::OnCompletionFunction() -> std::function<void(void)>
    {
        return [this]() {
            auto lock = std::lock_guard(this->requestsInProgressLock);
            this->requestsInProgress--;
        };
    }

    void ApiRequestFactory::BeginRequest()
    {
        auto lock = std::lock_guard(this->requestsInProgressLock);
        this->requestsInProgress++;
    }

    /**
     * This method is for shutdown / tests, wait for all requests to complete.
     */
    void ApiRequestFactory::AwaitRequestCompletion(const std::chrono::seconds& seconds)
    {
        auto startTime = std::chrono::system_clock::now();
        while (true) {
            if (std::chrono::system_clock::now() > startTime + seconds) {
                throw std::exception("Timeout whilst waiting for API request completion");
            }

            if (this->ApiRequestsCompleted()) {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    auto ApiRequestFactory::ApiRequestsCompleted() -> bool
    {
        auto lock = std::lock_guard(this->requestsInProgressLock);
        return this->requestsInProgress == 0;
    }
} // namespace UKControllerPluginUtils::Api
