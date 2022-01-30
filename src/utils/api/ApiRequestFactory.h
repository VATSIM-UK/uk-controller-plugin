#pragma once
#include "ApiRequest.h"
#include "ApiRequestFactoryFactory.h"

namespace UKControllerPluginUtils::Api {
    class ApiRequestPerformerInterface;

    class ApiRequestFactory
    {
        public:
        ApiRequestFactory(std::shared_ptr<ApiRequestPerformerInterface> requestPerformer, bool async);
        [[nodiscard]] auto Get(std::string uri) const -> ApiRequest;
        [[nodiscard]] auto Post(std::string uri, nlohmann::json body) const -> ApiRequest;
        [[nodiscard]] auto Put(std::string uri, nlohmann::json body) const -> ApiRequest;
        [[nodiscard]] auto Delete(std::string uri) const -> ApiRequest;
        [[nodiscard]] auto Patch(std::string uri, nlohmann::json body) const -> ApiRequest;

        private:
        // Helps with performing requests
        std::shared_ptr<ApiRequestPerformerInterface> requestPerformer;
        
        // Do we run requests async
        bool async;
    };
} // namespace UKControllerPluginUtils::Api
