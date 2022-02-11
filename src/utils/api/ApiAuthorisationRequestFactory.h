#pragma once
#include "ApiRequest.h"

namespace UKControllerPluginUtils::Api {
    class ApiRequestFactory;

    class ApiAuthorisationRequestFactory
    {
        public:
        ApiAuthorisationRequestFactory(ApiRequestFactory& requestFactory);
        [[nodiscard]] auto AuthorisationRequest() const -> ApiRequest;

        private:
        ApiRequestFactory& requestFactory;
    };
} // namespace UKControllerPluginUtils::Api
