#include "ApiAuthorisationRequestFactory.h"
#include "ApiRequestFactory.h"

namespace UKControllerPluginUtils::Api {

    ApiAuthorisationRequestFactory::ApiAuthorisationRequestFactory(const ApiRequestFactory& requestFactory)
        : requestFactory(requestFactory)
    {
    }

    auto ApiAuthorisationRequestFactory::AuthorisationRequest() const -> ApiRequest
    {
        return requestFactory.Get("authorise");
    }
} // namespace UKControllerPluginUtils::Api
