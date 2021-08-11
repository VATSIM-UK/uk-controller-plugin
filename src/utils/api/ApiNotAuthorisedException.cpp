#include "pch/pch.h"
#include "api/ApiNotAuthorisedException.h"

namespace UKControllerPlugin::Api {
    ApiNotAuthorisedException::ApiNotAuthorisedException(std::string message)
        : ApiException("ApiNotAuthorisedException: " + message)
    {
    }
} // namespace UKControllerPlugin::Api
