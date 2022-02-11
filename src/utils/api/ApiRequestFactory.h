#pragma once
#include "ApiRequest.h"

namespace UKControllerPluginUtils::Api {
    class ApiRequestPerformerInterface;

    class ApiRequestFactory
    {
        public:
        ApiRequestFactory(ApiRequestPerformerInterface& requestPerformer);
        [[nodiscard]] auto Get(std::string uri) -> ApiRequest;
        [[nodiscard]] auto Post(std::string uri, nlohmann::json body) -> ApiRequest;
        [[nodiscard]] auto Put(std::string uri, nlohmann::json body) -> ApiRequest;
        [[nodiscard]] auto Delete(std::string uri) -> ApiRequest;
        [[nodiscard]] auto Patch(std::string uri, nlohmann::json body) -> ApiRequest;
        void AwaitRequestCompletion();

        private:
        [[nodiscard]] auto OnCompletionFunction() -> std::function<void(void)>;
        void BeginRequest();

        // Helps with performing requests
        ApiRequestPerformerInterface& requestPerformer;

        // How many requests are in progress
        int requestsInProgress;

        // A lock for the progress variable
        std::mutex requestsInProgressLock;
    };
} // namespace UKControllerPluginUtils::Api
