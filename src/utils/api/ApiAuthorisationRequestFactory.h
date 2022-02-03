#pragma once
#include "ApiRequest.h"

namespace UKControllerPluginUtils::Api {
    class ApiRequestFactory;

    class ApiAuthorisationRequestFactory
    {
        public:
        ApiAuthorisationRequestFactory(const ApiRequestFactory& requestFactory);
        [[nodiscard]] auto AuthorisationRequest() const -> ApiRequest;

        private:
        const ApiRequestFactory& requestFactory;
    };
} // namespace UKControllerPluginUtils::Api
